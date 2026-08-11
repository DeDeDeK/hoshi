#ifndef KAR_H_TOPRIDE
#define KAR_H_TOPRIDE

#include "datatypes.h"

// Top Ride Kirby (player) system - completely separate from 3D mode Rider/Machine.
// Top Ride does NOT use Player_Create, Rider_Create, Machine_Create, stc_playerdata,
// RiderData, or MachineData. It uses minor 19 (not 18), so On3DLoadEnd doesn't fire.

// Charge component - inline sub-object at TopRideKirby+0x80, and the bulk of the
// Kirby object: in-world movement state, the visual-effects sub-object, the
// rumble/sound controllers, and the model/animation JObjs. Offsets below are
// from the component base; add 0x80 for the absolute TopRideKirby offset.
// Initialized by TopRide_KirbyChargeInit (0x802d1fe8), updated per frame by
// TopRide_ChargeUpdate (0x802df900).
typedef struct TopRideChargeComponent
{
    void *kirby_ptr;          // 0x00, pointer back to TopRideKirby base
    u8 x04[0x04];             // 0x04
    Vec3 position;            // 0x08, current in-world 2D position
    Vec3 facing_dir;          // 0x14, facing-direction unit vector
    Vec3 velocity;            // 0x20, current velocity vector (mod code zeroes this for trap states)
    u8 is_charging;           // 0x2C, 1 = A button held (charging), 0 = idle
    u8 charge_ready;          // 0x2D, 1 = charge depleted to 0.0, can accumulate again
    u8 x2E[0x02];             // 0x2E
    float speed_factor;       // 0x30, per-frame speed scaling
    float charge_value;       // 0x34, current charge level (0.0 to ~1.0)
    float prev_charge;        // 0x38, previous frame's charge value
    float charge_at_release;  // 0x3C, snapshot of charge_value at moment of A release
    float angular_velocity;   // 0x40, rotation rate from steering
    u8 x44[0x10];             // 0x44
    float boost_speed;        // 0x54, calculated boost speed from charge + angle tables
    u32 total_frames;         // 0x58, increments every frame (per-kirby frame counter)
    u32 frame_counter_1;      // 0x5C, swapped with frame_counter_2 on charge start
    u32 frame_counter_2;      // 0x60, stores previous frame_counter_1
    u32 frame_counter_3;      // 0x64, cleared on max charge
    u8 x68[0x04];             // 0x68
    u32 aerial_frames;        // 0x6C, frames since airborne
    u8 x70[0x2C];             // 0x70
    float distance_traveled;  // 0x9C, accumulated distance this run
    float wobble_scale_x;     // 0xA0, spring-damped visual oscillation (X)
    float wobble_scale_z;     // 0xA4, spring-damped visual oscillation (Z)
    u8 xA8[0x38];             // 0xA8
    u8 effects_system[0x2B0]; // 0xE0, visual-effects sub-object (slide-blur etc.)
    u8 x390[0x3C];            // 0x390
    void *rumble_controller;  // 0x3CC, rumble feedback controller
    u8 x3D0[0x54];            // 0x3D0
    void *charge_sfx_ctrl;    // 0x424, charge sound-effect controller
    u8 x428[0x28];            // 0x428
    Vec3 position_offset;     // 0x450, position adjustment, cleared each frame
    u8 x45C[0x04];            // 0x45C
    void *model_jobj;         // 0x460 (kirby+0x4E0), root JObj of the Kirby+star model
    u8 x464[0x04];            // 0x464
    void *arrow_jobj;         // 0x468, direction-arrow JObj
    u8 x46C[0x14];            // 0x46C
    void *charge_anim_1;      // 0x480, AC_S_CHARGE animation object
    u8 x484[0x0C];            // 0x484
    void *charge_anim_2;      // 0x490, second charge animation object
    u8 x494[0x10];            // 0x494
    float model_scale;        // 0x4A4 (kirby+0x524), init 1.0. Multiplied into the model JObj
                              //   scale every frame, so a write persists until the kirby is recreated.
    u8 x4A8[0x04];            // 0x4A8
    void *anim_controller;    // 0x4AC, animation state controller
} TopRideChargeComponent;     // mapped through 0x4B0; the object continues past this

// Per-player Kirby object. Vtable at 0x804d2304, RTTI name "Kirby". Created by
// TopRide_KirbyInit (0x802d4d64). The object is >0x1400 bytes (Absorber
// sub-object at +0xD00, etc.); this struct maps 0x00..~0x530.
typedef struct TopRideKirby
{
    void *vtable;                       // 0x00
    void *session_data;                 // 0x04, points at the inline charge component (kirby+0x80)
    u8 x08[0x04];                       // 0x08
    u8 player_slot;                     // 0x0C, controller slot 0..3
    u8 char_type;                       // 0x0D
    u8 start_position;                  // 0x0E, shuffled starting grid position (0..3) - not a CPU level
    u8 place;                           // 0x0F, race placement / finish rank; 0 while still racing
    u8 is_active;                       // 0x10, final standings byte, set on race start. Stays 0 in Time
                                        //       Attack and Free Run, so don't gate on it in solo modes.
    u8 active_item_kind;                // 0x11, held TopRideItemKind; 0xFF = none. Not reset on natural expiry.
    u8 x12[0x02];                       // 0x12
    int lap_progress;                   // 0x14, accumulated CheckLine cross result (init -1); positive completes a lap
    u8 lap_pending;                     // 0x18, set on a backward checkpoint cross; gates lap completion
    u8 x19[0x03];                       // 0x19
    u32 finish_time;                    // 0x1C, latched from the master race timer (KirbyMgr+0x402C) on finish
    u32 prev_lap_frames;                // 0x20, snapshot of cur_lap_frames at lap completion
    u32 cur_lap_frames;                 // 0x24, current-lap frame counter, reset on lap completion
    u8 x28[0x04];                       // 0x28
    float mass;                         // 0x2C, per-character mass / scale base
    float gravity;                      // 0x30, gravity / vertical accel base
    float accel_param;                  // 0x34, frame-scaled acceleration parameter
    float decel_param;                  // 0x38, frame-scaled deceleration parameter
    u8 x3C;                             // 0x3C, init 0
    u8 x3D;                             // 0x3D, init 1
    u8 finished;                        // 0x3E, set on crossing the finish line; gates the per-frame counters
    u8 x3F;                             // 0x3F, init 1
    u8 direction_sign;                  // 0x40, movement-direction sign, refreshed each frame from the run mode
    u8 x41;                             // 0x41
    u16 screen_w;                       // 0x42, init 320 (viewport width)
    u16 screen_h;                       // 0x44, init 240 (viewport height)
    u16 x46;                            // 0x46
    void *input_reader;                 // 0x48, controller / input source object (vt+0x14 = poll stick)
    Vec3 position;                      // 0x4C, spawn / default pos only - live position is charge.position
    Vec3 target_pos;                    // 0x58, initial camera target / lookat
    u8 history[0x18];                   // 0x64, 10-entry circular stick-input ring used by the quick-spin query
    void *state_handler;                // 0x7C, input/state handler (charge state machine)
    TopRideChargeComponent charge;      // 0x80, inline charge component
} TopRideKirby;

// Top Ride input is polymorphic: every kirby holds an input_reader at
// TopRideKirby+0x48. Human slots get a pad reader (vtable 0x804d25e0), CPU slots
// this one (vtable 0x804d8710). Each frame the reader is polled via vt[0x14],
// which dispatches to vt[0x0c] - TopRide_CpuInputThink for CPUs - to produce a
// steer float plus a press/charge byte.
//
// Difficulty comes from the per-slot config byte GameData[slot*9 + 0xD23]
// (TopRide_SetHandicap) and scales steering gain, reaction frames (60->0 across
// levels 0->4), and commit thresholds through the per-level tables at
// 0x804d80d0 / 0x804d8058 / 0x804d80bc. A ~0x80-byte object; gaps are scratch.
typedef struct TopRideCpuInputReader
{
    void *vtable;            // 0x00, 0x804d8710 (CPU); human readers use 0x804d25e0
    TopRideKirby *kirby;     // 0x04, back-pointer to the owning kirby
    Vec3 heading;            // 0x08, desired heading vector (crossed with the kirby facing to steer)
    int  reaction_budget;    // 0x14, DAT_804d7f90[+0x18]; frames of reaction lag
    int  x18;                // 0x18, index into DAT_804d7f90 (0 in observed races)
    int  difficulty;         // 0x1c, AI skill 0..4 (= handicap byte); indexes the per-level tables
    float steer_noise;       // 0x20, HSD_Randf-seeded steering jitter magnitude
    u8   x24[0x04];          // 0x24
    int  hold_frames;        // 0x28, counts down a committed steer/charge hold
    u8   x2c[0x04];          // 0x2c
    u8   debug_draw;         // 0x30, when set, writes a heading arrow to EnemyMgr+0x3da0
    u8   x31[0x03];          // 0x31
    float steer_hist[9];     // 0x34, ring of recent |steer delta| (oscillation damping)
    float last_steer;        // 0x58, previous frame's steer output
    int  hist_index;         // 0x5c, steer_hist write cursor (wraps at 9)
    u8   last_charge;        // 0x60, previous frame's press/charge byte
    u8   x61[0x03];          // 0x61
    int  x64;                // 0x64
    int  prev_state;         // 0x68, the CPU's own Kirby state ID; the brain's switch dispatches on this
    int  reseed_timer;       // 0x6c, frames until the next steer_noise reseed
    int  lowspeed_frames;    // 0x70, frames spent below the speed threshold (stuck detect)
    TopRideKirby *kirby_dup; // 0x74, second copy of the kirby pointer
} TopRideCpuInputReader;

// Per-frame CPU steering brain, the reader's vt[0x0c]. Writes *steer_out
// (lateral stick) and returns the press/charge byte - the hook point for
// replacing or post-adjusting CPU steering.
char TopRide_CpuInputThink(TopRideCpuInputReader *reader, float *steer_out); // 0x802eee90
char TopRide_CpuInputPoll(TopRideCpuInputReader *reader, float *sx, float *sy); // 0x80291dec, CPU vt[0x14]
u8   TopRide_KirbyGetSlot(TopRideKirby *kirby);                              // 0x802d4d5c, returns kirby+0x0c

// Voluntary quick spin - the L/R stick-flick spin attack, distinct from the
// KirbySpin hazard spin-out (vtable[61]). TopRide_KirbyPhysUpdate pushes the
// stick input into the kirby's history ring each frame, then Query sums the
// ring's per-entry |delta| and reports a flick above 200. A nonzero result sets
// charge.angular_velocity and enters the spin-attack state, so the Query call at
// 0x802d5f90 is the single gate for the whole move.
void TopRide_KirbyHistoryPush(float x, float y, int *history); // 0x80311f88, history = &kirby->history
int  TopRide_KirbyHistoryQuery(int *history);                  // 0x80312000, 0 = no flick, +/-1 = flick dir

// CPU brain helpers, all reached from TopRide_CpuInputThink. Perceive fills a
// stack blackboard (sector look-ahead, nearest rival/item/obstacle, per-difficulty
// weights) for the four detectors, which run in order - the first to commit
// short-circuits the rest and suppresses the route-followers. If none commit, a
// route-follower steers the racing line. All write reader->heading plus a
// press/charge flag. Signatures are approximate.
int  TopRide_CpuPerceive(TopRideCpuInputReader *reader, void *blackboard);        // 0x802eb094
int  TopRide_CpuDetectHazard(TopRideCpuInputReader *reader, void *blackboard);    // 0x802ed434, incoming projectiles -> dodge swerve
int  TopRide_CpuDetectItem(TopRideCpuInputReader *reader, void *blackboard);      // 0x802ecc54, ItemMgr -> steer to good / away from bad
int  TopRide_CpuDetectRival(TopRideCpuInputReader *reader, void *blackboard);     // 0x802eda78, rival kirbys -> block / ram intercept
int  TopRide_CpuDetectObstacle(TopRideCpuInputReader *reader, void *blackboard);  // 0x802ee210, CpuObstacleMgr walls/air-currents -> avoid or boost-into
int  TopRide_CpuRouteFollowFull(TopRideCpuInputReader *reader, void *blackboard); // 0x802ebb98, perfect-line follow (reaction_budget==0; dead in normal play)
int  TopRide_CpuRouteFollowLagged(TopRideCpuInputReader *reader, void *blackboard);// 0x802ec890, reaction-lagged follow (the normal-play path)

// KirbyMgr singleton - top-level manager for all Top Ride players.
// Created by TopRide_KirbyMgrInit (0x802dafb4). ~0x4080 bytes total.
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

// TR has only two playable "Control Type" machines, selected by L/R on the
// middle row of each lobby panel. Maps 1:1 to the global MachineKind values
// VCKIND_FREE / VCKIND_STEER for unlock-mask checks.
typedef enum TopRideMachineKind
{
    TR_MACHINE_FREE  = 0,   // Free Star - analog stick freely steers (VCKIND_FREE)
    TR_MACHINE_STEER = 1,   // Steer Star - left/right only (VCKIND_STEER)
    TR_MACHINE_NUM,
} TopRideMachineKind;

// Convert a TopRideMachineKind to the corresponding MachineKind (VCKIND),
// e.g. for indexing a per-machine unlock mask.
#define TOPRIDE_MACHINE_TO_VCKIND(tr) ((tr) == TR_MACHINE_FREE ? VCKIND_FREE : VCKIND_STEER)

// Per-slot config accessors, backed by the 9-byte-stride config block at
// GameData[slot*9 + 0xD20] and committed from the lobby fields at TR scene-exit.
TopRidePlayerKind TopRide_GetPlayerKind(int slot);                   // 0x8000bd6c, reads byte +0
void TopRide_SetPlayerKind(int slot, TopRidePlayerKind kind);        // 0x8000bda8, writes byte +0
u8   TopRide_GetColor(int slot);                                     // 0x8000bdf0, reads byte +1
void TopRide_SetColor(int slot, u8 color);                           // 0x8000be2c, writes byte +1
// Inert despite the name: stays 0 and is never read by the CPU AI. The CSS "CPU
// Level" control is the +3 handicap byte below.
void TopRide_SetCpuLevel(int slot, u8 level);                        // 0x8000be74, writes byte +2 (0..4)
// The CSS per-slot skill control - "Handicap" for humans, "CPU Level" for CPUs -
// and the byte that drives the CPU steering AI.
void TopRide_SetHandicap(int slot, u8 handicap);                     // 0x8000bf04, writes byte +3 (0..4)
void TopRide_SetControllerPort(int slot, u8 port);                   // 0x8000bebc, writes byte +6
TopRideMachineKind TopRide_GetMachineKind(int slot);                 // 0x8000bf4c, reads byte +8
void TopRide_SetMachineKind(int slot, TopRideMachineKind machine);   // 0x8000bf8c, writes byte +8

// Multiplayer-race lobby thinks, dispatched per-frame per-slot from
// TopRide_PreGameThink (0x8002c06c) on the slot's ply_state: 0 = selecting,
// 1 = ready, 2 = in-panel editing. All three operate on
// GameData.topride_select_ply. The L/R "Control Type" machine cycler lives in
// PanelThink at 0x8002be20..0x8002be94, reading/writing panel_machine[slot].
void TopRide_CSS_SelectingThink(int slot);  // 0x8002ac68
void TopRide_CSS_ReadyThink(int slot);      // 0x8002b094
void TopRide_CSS_PanelThink(int slot);      // 0x8002b8a8

// Solo (Free Run / Time Attack) panel-editing think, dispatched from
// TopRide_OnCourseSelect for slots with ply_state != 1. It carries its own copy
// of the machine cycler at 0x8002cb88..0x8002cbec and never routes through
// TopRide_CSS_PanelThink, so machine-unlock gating must hook it separately (at
// 0x8002cb98).
void TopRide_SoloPanelThink(int slot);      // 0x8002ca80

// Reinitializes the TR lobby block (memsets 0x39 bytes at GameData+0x197 then
// writes per-slot defaults: panel_machine=0 (Free Star), color=slot,
// handicap=2, panel_field_d=0xff, etc.).
void TopRide_InitSelectData(void);                                   // 0x8002cfd8

// State ID returned by state_handler->vt[+0x0C]() for the current Kirby state.
typedef enum TopRideKirbyStateId
{
    TR_KSTATE_DAMAGE_BASE   = 0,   // KirbyDamage abstract base - should not be observed live
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

// State class vtables. Comparing against state_handler->vtable is the reliable
// way to identify the current state - it is what the wrappers' dynamic_cast
// does. The state's own get_state_id slot is not: KirbyNormal and the
// KirbyDamage base inherit a stub returning 0.
#define TR_KSTATE_VT_NORMAL      ((void *)0x804d6f5c)
#define TR_KSTATE_VT_DAMAGE_BASE ((void *)0x804da158)
#define TR_KSTATE_VT_PRESS       ((void *)0x804da070)
#define TR_KSTATE_VT_CRUSH       ((void *)0x804d9ee0)
#define TR_KSTATE_VT_EXPLODE     ((void *)0x804d9dd0)
#define TR_KSTATE_VT_STRIKE      ((void *)0x804d9cbc)
#define TR_KSTATE_VT_SPIN        ((void *)0x804d9a90)
#define TR_KSTATE_VT_SANDSPIN    ((void *)0x804d9bac)
#define TR_KSTATE_VT_NUMB        ((void *)0x804d9980)
#define TR_KSTATE_VT_ELEC        ((void *)0x804d9870)
#define TR_KSTATE_VT_WHIRLPOOL   ((void *)0x804d9760)
#define TR_KSTATE_VT_BURN        ((void *)0x804d964c)
#define TR_KSTATE_VT_FREEZE      ((void *)0x804d953c)
#define TR_KSTATE_VT_CONFUSE     ((void *)0x804d9434)
#define TR_KSTATE_VT_SHORTCUT    ((void *)0x804d90e8)
#define TR_KSTATE_VT_TRANSPARENT ((void *)0x804da304)
#define TR_KSTATE_VT_SPEEDUP     ((void *)0x804dbcf8)
#define TR_KSTATE_VT_SPEEDDOWN   ((void *)0x804dbac8)

// The kirby's current state class vtable, or NULL before the round wires
// state_handler. Compare it against the constants above to identify the state.
static inline void *TopRide_KirbyStateVtable(TopRideKirby *kirby)
{
    if (!kirby || !kirby->state_handler)
        return (void *)0;
    return *(void **)kirby->state_handler;
}

// Item-power state vtables installed by TopRide_KirbyApplyItem for the four
// ability-power items, Top Ride's analogs of copy abilities. While one is active
// the kirby's state_handler carries the matching vtable, so
// TopRide_KirbyStateVtable identifies a held ability power.
#define TR_ITEMPOWER_VT_FIRE       ((void *)0x804db288) // Fire item (TRITEM_FIRE, 11)
#define TR_ITEMPOWER_VT_FREEZE_FAN ((void *)0x804dc6e4) // Freeze Fan item (TRITEM_FREEZE_FAN, 9)
#define TR_ITEMPOWER_VT_BOMB       ((void *)0x804db088) // Bomb item (TRITEM_BOMB, 13)
#define TR_ITEMPOWER_VT_WALKY      ((void *)0x804dc150) // Walky item (TRITEM_WALKY, 16)

// KirbyNormal - force the kirby back to neutral. Exits the current state via its
// vt[2] teardown (removing an item power's aura/model/effects), the same revert
// the engine runs when one item power replaces another. Wrapper at 0x802da0f4
// (vtable[50]). Safe once round_state == 2.
static inline void TopRide_KirbyNormal(TopRideKirby *kirby)
{
    typedef void (*Method)(TopRideKirby *);
    ((Method)(((void **)kirby->vtable)[50]))(kirby);
}

// Each helper below invokes a Kirby-class method (vtable 0x804d2304) that drives
// a transition in kirby->state_handler, and is safe once round_state == 2. All
// pass zero args, giving a static stun-style transition: the state's animation
// plays in place with default duration and no knockback impulse. The remaining
// damage states have wrappers in the same vtable range but no helper here.

// KirbyFreeze - frozen solid. Wrapper at 0x802d56bc (vtable[57]).
static inline void TopRide_KirbyFreeze(TopRideKirby *kirby)
{
    typedef void (*Method)(TopRideKirby *);
    ((Method)(((void **)kirby->vtable)[57]))(kirby);
}

// KirbyPress - pressed / squeezed. Wrapper at 0x802d54ec (vtable[55]).
static inline void TopRide_KirbyPress(TopRideKirby *kirby)
{
    typedef void (*Method)(TopRideKirby *);
    ((Method)(((void **)kirby->vtable)[55]))(kirby);
}

// KirbyNumb - paralysis; blocked in KirbyDoodlebugOut. Wrapper at 0x802d5b74 (vtable[64]).
static inline void TopRide_KirbyNumb(TopRideKirby *kirby)
{
    typedef void (*Method)(TopRideKirby *);
    ((Method)(((void **)kirby->vtable)[64]))(kirby);
}

// KirbyConfuse - controls scrambled (AC_PANIC); blocked in KirbyDoodlebugOut.
// Wrapper at 0x802d5c64 (vtable[66]).
static inline void TopRide_KirbyConfuse(TopRideKirby *kirby)
{
    typedef void (*Method)(TopRideKirby *, u32);
    ((Method)(((void **)kirby->vtable)[66]))(kirby, 0);
}

#endif // KAR_H_TOPRIDE
