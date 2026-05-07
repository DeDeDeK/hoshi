#ifndef KAR_H_TOPRIDE
#define KAR_H_TOPRIDE

#include "datatypes.h"

// Top Ride Kirby (player) system — completely separate from 3D mode Rider/Machine.
// Top Ride does NOT use Player_Create, Rider_Create, Machine_Create, stc_playerdata,
// RiderData, or MachineData. It uses minor 19 (not 18), so On3DLoadEnd doesn't fire.
// See docs/topride-system.md for full documentation.

// Charge component — inline sub-object starting at TopRideKirby+0x80.
// Initialized by TopRide_KirbyChargeInit (0x802d1fe8).
// Per-frame update in TopRide_ChargeUpdate (0x802df900).
typedef struct TopRideChargeComponent
{
    void *kirby_ptr;          // 0x00, pointer back to TopRideKirby base
    u8 x04[0x04];            // 0x04
    Vec3 position;            // 0x08
    Vec3 facing_dir;          // 0x14
    Vec3 velocity;            // 0x20
    u8 is_charging;           // 0x2C, 1 = A button held (charging), 0 = idle
    u8 charge_ready;          // 0x2D, 1 = charge depleted to 0.0, can accumulate again
    u8 x2E[0x02];            // 0x2E
    float speed_factor;       // 0x30, per-frame speed scaling
    float charge_value;       // 0x34, current charge level (0.0 to ~1.0)
    float prev_charge;        // 0x38, previous frame's charge value
    float charge_at_release;  // 0x3C, snapshot of charge_value at moment of A release
    float angular_velocity;   // 0x40, rotation rate from steering
} TopRideChargeComponent;

// Per-player Kirby object. Vtable at 0x804d2304, RTTI name "Kirby".
// Created by TopRide_KirbyInit (0x802d4d64). ~0x500+ bytes total.
typedef struct TopRideKirby
{
    void *vtable;                       // 0x00
    void *session_data;                 // 0x04
    u8 x08[0x04];                       // 0x08
    u8 player_slot;                     // 0x0C, controller slot 0..3 — pass to TopRide_GetPlayerKind for HMN/CPU/NONE
    u8 char_type;                       // 0x0D
    u8 start_position;                  // 0x0E, Fisher-Yates shuffled grid position (0..3) — NOT a CPU level. Set per-round in TopRide_FielderInit from KirbyMgr+0x4024+i.
    u8 x0F;                             // 0x0F
    u8 is_active;                       // 0x10, set on race start; stays 0 in Time Attack and Free Run even while playing — don't gate on this in solo modes
    u8 x11[0x3B];                       // 0x11
    Vec3 position;                      // 0x4C, spawn / default pos — NOT tracked per frame. For actual in-world position use charge.position (0x88).
    Vec3 target_pos;                    // 0x58
    int angles[3];                      // 0x64
    u8 x70[0x0C];                       // 0x70
    void *state_handler;                // 0x7C, input/state handler (charge state machine)
    TopRideChargeComponent charge;      // 0x80, inline charge component
} TopRideKirby;

// KirbyMgr singleton — top-level manager for all Top Ride players.
// Created by TopRide_FielderInit (0x802dafb4). ~0x4080 bytes total.
typedef struct TopRideKirbyMgr
{
    void *vtable;                       // 0x00
    void *game_config;                  // 0x04
    TopRideKirby *kirbys[4];            // 0x08, per-player Kirby pointers (NULL if slot unused)
    u8 x18[0x4028 - 0x18];              // 0x18
    u8 round_state;                     // 0x4028, 0=pre-init, 1=countdown, 2=race active. Master gate for per-frame physics, item spawning, and TopRideItem_Update.
} TopRideKirbyMgr;

// KirbyMgr singleton pointer. NULL when not in Top Ride gameplay.
static TopRideKirbyMgr **stc_topride_kirbymgr = (TopRideKirbyMgr **)(0x805dd0e0 + 0xA64); // 0x805ddb44

// Per-slot player kind, stored at GameData[slot*9 + 0xD20]. Discriminates
// human/CPU/empty for each of the 4 controller slots in a TR session.
typedef enum TopRidePlayerKind
{
    TR_PKIND_HMN = 0,
    TR_PKIND_CPU = 1,
    TR_PKIND_NONE = 2,
} TopRidePlayerKind;

// Get/set the kind for a given TR controller slot (0..3). Backed by the
// 9-byte-stride per-slot config block at GameData+0xD20.
TopRidePlayerKind TopRide_GetPlayerKind(int slot); // 0x8000bd6c
void TopRide_SetPlayerKind(int slot, TopRidePlayerKind kind); // 0x8000bda8

// State ID returned by state_handler->vt[+0x0C]() for the current Kirby state.
// See docs/topride-kirby-states.md for the full state machine and class table.
typedef enum TopRideKirbyStateId
{
    TR_KSTATE_DAMAGE_BASE   = 0,   // KirbyDamage abstract base — should not be observed live
    TR_KSTATE_NORMAL        = 1,
    TR_KSTATE_PRESS         = 2,
    TR_KSTATE_CRUSH         = 3,
    TR_KSTATE_EXPLODE       = 4,
    TR_KSTATE_STRIKE        = 5,
    TR_KSTATE_SPIN          = 6,   // also KirbySandSpin (same ID, KirbySpin subclass)
    TR_KSTATE_NUMB          = 7,
    TR_KSTATE_ELEC          = 8,
    TR_KSTATE_WHIRLPOOL     = 9,
    TR_KSTATE_BURN          = 10,
    TR_KSTATE_FREEZE        = 11,
    TR_KSTATE_CONFUSE       = 12,
    TR_KSTATE_DOODLEBUG_OUT = 13,  // sentinel: blocks Numb/Elec/Confuse re-entry
    TR_KSTATE_GRIND         = 14,
    TR_KSTATE_SHORTCUT      = 15,
    TR_KSTATE_TRANSPARENT   = 16,
    TR_KSTATE_SPEEDUP       = 17,
    TR_KSTATE_SPEEDDOWN     = 18,
} TopRideKirbyStateId;

// Read the current state ID via state_handler->vt[+0x0C](). Safe once
// round_state == 2; state_handler is NULL / partially wired before that.
static inline TopRideKirbyStateId TopRide_KirbyGetStateId(TopRideKirby *kirby)
{
    void **state_vt = *(void ***)kirby->state_handler;
    int (*get_id)(void *) = (int (*)(void *))state_vt[3]; // vtable byte offset 0x0C
    return (TopRideKirbyStateId)get_id(kirby->state_handler);
}

// === Kirby state-transition helpers ===
//
// Each helper invokes a non-virtual method on the Kirby class (vtable at
// 0x804d2304) that drives a transition in kirby->state_handler. All are safe
// to call once round_state == 2 (race active). See docs/topride-kirby-states.md
// for the full state machine, vanilla effector callers, and arg semantics.
//
// All helpers pass zero args (and a zero knockback Vec3 where applicable),
// which produces a static stun-style transition: the new state's animation
// plays in place with default duration and no knockback impulse. For
// vanilla-quality knockback you'd synthesize a source position and reuse
// the per-effector math — not needed for trap effects.

// KirbyExplode ("tumble") — vanilla bomb / landmine state.
// Wrapper: TopRide_KirbyExplodeMethod at 0x802d5834 (vtable[59], +0xEC).
// Vanilla caller: EffectorExplode_ApplyToKirby at 0x802e6898.
static inline void TopRide_KirbyExplode(TopRideKirby *kirby)
{
    Vec3 zero = {0.0f, 0.0f, 0.0f};
    typedef void (*Method)(TopRideKirby *, u16, Vec3 *, u16, u16);
    ((Method)(((void **)kirby->vtable)[59]))(kirby, 0, &zero, 0, 0);
}

// KirbyCrush — heavy-machine collision state. The setter installs AC_TOBASARE
// (knockback launch), the same animation as Explode/Strike — Crush is NOT the
// squish/flatten visual despite the EffectorCrush_ApplyToKirby name; the
// pancake animation lives in KirbyPress (AC_FLAT_START).
// Wrapper at 0x802d5760 (vtable[58], +0xE8).
// Vanilla caller: EffectorCrush_ApplyToKirby at ~0x802e6630.
static inline void TopRide_KirbyCrush(TopRideKirby *kirby)
{
    Vec3 zero = {0.0f, 0.0f, 0.0f};
    typedef void (*Method)(TopRideKirby *, u16, Vec3 *);
    ((Method)(((void **)kirby->vtable)[58]))(kirby, 0, &zero);
}

// KirbyStrike — generic hit reaction.
// Wrapper at 0x802d5900 (vtable[60], +0xF0).
static inline void TopRide_KirbyStrike(TopRideKirby *kirby)
{
    Vec3 zero = {0.0f, 0.0f, 0.0f};
    typedef void (*Method)(TopRideKirby *, u16, Vec3 *, u16, u16);
    ((Method)(((void **)kirby->vtable)[60]))(kirby, 0, &zero, 0, 0);
}

// KirbyBurn — fire damage-over-time state. Blocked if currently in KirbyElec.
// Wrapper at 0x802d55c0 (vtable[56], +0xE0). Arg2 is a u32* dereferenced at
// 0x802d5674 (`lwz r0, 0(r30)`) before being copied onto the setter's stack
// frame; passing a literal 0 here causes a DSI on null. Pass a stack-local
// zero so the dereference is safe.
static inline void TopRide_KirbyBurn(TopRideKirby *kirby)
{
    u32 zero = 0;
    typedef void (*Method)(TopRideKirby *, u32, u32 *, u32);
    ((Method)(((void **)kirby->vtable)[56]))(kirby, 0, &zero, 0);
}

// KirbyFreeze — frozen solid.
// Wrapper at 0x802d56bc (vtable[57], +0xE4).
static inline void TopRide_KirbyFreeze(TopRideKirby *kirby)
{
    typedef void (*Method)(TopRideKirby *);
    ((Method)(((void **)kirby->vtable)[57]))(kirby);
}

// KirbyPress — pressed / squeezed.
// Wrapper at 0x802d54ec (vtable[55], +0xDC).
static inline void TopRide_KirbyPress(TopRideKirby *kirby)
{
    typedef void (*Method)(TopRideKirby *);
    ((Method)(((void **)kirby->vtable)[55]))(kirby);
}

// KirbySpin — spin-out. KirbySandSpin variant lives at vtable[62] / +0xF8.
// Wrapper at 0x802d59cc (vtable[61], +0xF4). The setter (0x802f7718) feeds
// arg2 to PSVECMagnitude at 0x802f7998 to size the spin knockback; passing
// a literal 0 in that slot crashes inside PSVECMagnitude on null deref.
// Pass a stack-local zero Vec3 so the magnitude evaluates to 0 and the
// knockback path is skipped, matching the static stun pattern used by the
// Explode/Strike wrappers.
static inline void TopRide_KirbySpin(TopRideKirby *kirby)
{
    Vec3 zero = {0.0f, 0.0f, 0.0f};
    typedef void (*Method)(TopRideKirby *, u32, Vec3 *, u32);
    ((Method)(((void **)kirby->vtable)[61]))(kirby, 0, &zero, 0);
}

// KirbyNumb — paralysis. Blocked if state ID == 13 (KirbyDoodlebugOut).
// Wrapper at 0x802d5b74 (vtable[64], +0x100).
static inline void TopRide_KirbyNumb(TopRideKirby *kirby)
{
    typedef void (*Method)(TopRideKirby *);
    ((Method)(((void **)kirby->vtable)[64]))(kirby);
}

// KirbyElec — electrified. Blocked if state ID == 13 (KirbyDoodlebugOut).
// Wrapper at 0x802d5be4 (vtable[65], +0x104).
static inline void TopRide_KirbyElec(TopRideKirby *kirby)
{
    typedef void (*Method)(TopRideKirby *, u32);
    ((Method)(((void **)kirby->vtable)[65]))(kirby, 0);
}

// KirbyConfuse — controls reversed / scrambled (AC_PANIC animation).
// Blocked if state ID == 13 (KirbyDoodlebugOut).
// Wrapper at 0x802d5c64 (vtable[66], +0x108).
static inline void TopRide_KirbyConfuse(TopRideKirby *kirby)
{
    typedef void (*Method)(TopRideKirby *, u32);
    ((Method)(((void **)kirby->vtable)[66]))(kirby, 0);
}

// KirbySpeedDown — speed debuff (matches the SpeedDown TR item).
// Has an extra predicate guard at state_handler->vt[+0x48].
// Wrapper at 0x802d5da4 (vtable[68], +0x110).
static inline void TopRide_KirbySpeedDown(TopRideKirby *kirby)
{
    typedef void (*Method)(TopRideKirby *, u32);
    ((Method)(((void **)kirby->vtable)[68]))(kirby, 0);
}

#endif // KAR_H_TOPRIDE
