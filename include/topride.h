#ifndef KAR_H_TOPRIDE
#define KAR_H_TOPRIDE

#include "datatypes.h"

// Top Ride Kirby (player) system - completely separate from 3D mode Rider/Machine.
// Top Ride does NOT use Player_Create, Rider_Create, Machine_Create, stc_playerdata,
// RiderData, or MachineData. It uses minor 19 (not 18), so On3DLoadEnd doesn't fire.
// See docs/topride-system.md for full documentation.

// Charge component - inline sub-object starting at TopRideKirby+0x80.
// Initialized by TopRide_KirbyChargeInit (0x802d1fe8).
// Per-frame update in TopRide_ChargeUpdate (0x802df900).
//
// This is the large body of the Kirby object - it holds the in-world movement
// state, the visual-effects sub-object, the rumble/sound controllers, and the
// model/animation JObjs. Offsets are listed from the component base; add 0x80
// for the absolute TopRideKirby offset. TopRideKirby.session_data (+0x04)
// points at this component, which is how the model thinker reaches model_jobj /
// model_scale via session_data[+0x460] / session_data[+0x4A4].
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
    float model_scale;        // 0x4A4 (kirby+0x524), init 1.0; multiplied into the
                              //   model JObj scale every frame by TopRide_KirbyModelThink,
                              //   so a write persists until the kirby is recreated.
    u8 x4A8[0x04];            // 0x4A8
    void *anim_controller;    // 0x4AC, animation state controller
} TopRideChargeComponent;     // mapped through 0x4B0; the object continues past this

// Per-player Kirby object. Vtable at 0x804d2304, RTTI name "Kirby".
// Created by TopRide_KirbyInit (0x802d4d64). Object is >0x1400 bytes total
// (Absorber sub-object at +0xD00, etc.); this struct maps the head + the inline
// charge component, which covers the 0x00..~0x530 region.
typedef struct TopRideKirby
{
    void *vtable;                       // 0x00
    void *session_data;                 // 0x04, points at the inline charge component (kirby+0x80)
    u8 x08[0x04];                       // 0x08
    u8 player_slot;                     // 0x0C, controller slot 0..3 - pass to TopRide_GetPlayerKind for HMN/CPU/NONE
    u8 char_type;                       // 0x0D
    u8 start_position;                  // 0x0E, Fisher-Yates shuffled grid position (0..3) - NOT a CPU level. Set per-round in TopRide_KirbyMgrInit from KirbyMgr+0x4024+i.
    u8 place;                           // 0x0F, current race placement / finish rank, written each frame by the ranking pass in TopRide_KirbyMgrUpdate (0 while still racing); gated as "== 0" for "not yet finished".
    u8 is_active;                       // 0x10, final standings byte set by the same ranking pass on race start; stays 0 in Time Attack and Free Run even while playing - don't gate on this in solo modes
    u8 x11[0x03];                       // 0x11, x11 init 0xFF
    int lap_progress;                   // 0x14, accumulates the per-frame CheckLine cross result (init -1); going positive completes a lap/segment
    u8 lap_pending;                     // 0x18, set when a checkpoint is crossed backward; gates the lap-completion branch
    u8 x19[0x03];                       // 0x19
    u32 finish_time;                    // 0x1C, total frame counter; latched to the master race timer (KirbyMgr+0x402C) when the kirby finishes (finished flag set)
    u32 prev_lap_frames;                // 0x20, snapshot of cur_lap_frames at lap completion
    u32 cur_lap_frames;                 // 0x24, current-lap frame counter (reset to 0 on lap completion, incremented every frame)
    u8 x28[0x04];                       // 0x28
    float mass;                         // 0x2C, per-character mass / scale base (read constantly in physics)
    float gravity;                      // 0x30, gravity / vertical accel base
    float accel_param;                  // 0x34, frame-scaled acceleration parameter
    float decel_param;                  // 0x38, frame-scaled deceleration parameter
    u8 x3C;                             // 0x3C, init 0
    u8 x3D;                             // 0x3D, init 1
    u8 finished;                        // 0x3E, set to 1 when the kirby crosses the finish line; gates the per-frame counter increments in TopRide_KirbyPhysUpdate
    u8 x3F;                             // 0x3F, init 1
    u8 direction_sign;                  // 0x40, movement-direction sign flag, refreshed each frame from the run mode
    u8 x41;                             // 0x41
    u16 screen_w;                       // 0x42, init 320 (viewport width)
    u16 screen_h;                       // 0x44, init 240 (viewport height)
    u16 x46;                            // 0x46
    void *input_reader;                 // 0x48, controller / input source object (vt+0x14 = poll stick)
    Vec3 position;                      // 0x4C, spawn / default pos - NOT tracked per frame. For actual in-world position use charge.position (0x88).
    Vec3 target_pos;                    // 0x58, initial camera target / lookat
    u8 history[0x18];                   // 0x64, 10-entry circular history ring (head index + paired byte values), pushed by TopRide_KirbyHistoryPush (0x80311f88) / queried by TopRide_KirbyHistoryQuery (0x80312000) for the anti-jitter snap
    void *state_handler;                // 0x7C, input/state handler (charge state machine)
    TopRideChargeComponent charge;      // 0x80, inline charge component
} TopRideKirby;

// model_scale lives inside the inline charge component (kirby->charge.model_scale,
// i.e. TopRideKirby+0x524). Kept as a raw-offset accessor for back-compat with
// existing call sites; equivalent to &kirby->charge.model_scale.
static inline float *TopRide_KirbyModelScalePtr(TopRideKirby *kirby)
{
    return &kirby->charge.model_scale;
}

// === Top Ride CPU rider AI (source: a2d_cpu_kirby.cpp) ===
//
// Top Ride's input is polymorphic: every kirby holds an `input_reader` at
// TopRideKirby+0x48. Human slots get a pad reader (vtable 0x804d25e0); CPU slots
// get the CPU brain reader (vtable 0x804d8710, this struct). TopRide_KirbyPhysUpdate
// polls the reader each frame via vt[0x14] (the *Poll fns), which dispatch to
// vt[0x0c] (the *Read/*Think fns) to produce a steer float + a press/charge byte,
// then pack them into the kirby's history ring. For CPU slots vt[0x0c] is
// TopRide_CpuInputThink (0x802eee90) - the per-frame steering brain.
//
// Difficulty (0..4) is read at construction from the per-slot config byte
// game_config[slot*9 + 0x5b] (= GameData[slot*9 + 0xD23], the "+3" byte set by
// TopRide_SetHandicap) and cached at +0x1c. That byte is the CSS per-slot skill
// control - shown as "Handicap" for human slots and "CPU Level" for CPU slots (one
// shared field; CSS 1..5 -> internal 0..4). **The "+2" byte (config +0x5a /
// GameData[slot*9+0xD22], TopRide_SetCpuLevel) is inert - it stays 0 and is NOT
// read by the brain, despite its name.** Level scales steering gain, reaction
// frames (60->0 as level 0->4), and commit thresholds via the per-level tables at
// 0x804d80d0 / 0x804d8058 / 0x804d80bc (indexed by +0x1c).
//
// Allocated as a ~0x80-byte object; confirmed fields below, gaps are scratch/uncertain.
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
    int  prev_state;         // 0x68, the CPU's own current Kirby state ID (TopRideKirbyStateId, read via kirby vt[0x28]); the brain's switch dispatches on THIS, not an invented maneuver enum
    int  reseed_timer;       // 0x6c, frames until the next steer_noise reseed
    int  lowspeed_frames;    // 0x70, frames spent below the speed threshold (stuck detect)
    TopRideKirby *kirby_dup; // 0x74, second copy of the kirby pointer
} TopRideCpuInputReader;

// Per-frame Top Ride CPU steering brain. Reached as the CPU input-reader's
// vt[0x0c]; CPU-gated by construction. Writes *steer_out (lateral stick) and
// returns the press/charge byte. The cleanest mod hook for replacing or
// post-adjusting CPU steering (analogous to _Rider_UpdateCPU for City Trial /
// Air Ride). Reach the kirby via reader->kirby (+0x04).
char TopRide_CpuInputThink(TopRideCpuInputReader *reader, float *steer_out); // 0x802eee90
char TopRide_CpuInputPoll(TopRideCpuInputReader *reader, float *sx, float *sy); // 0x80291dec, CPU vt[0x14]
u8   TopRide_KirbyGetSlot(TopRideKirby *kirby);                              // 0x802d4d5c, returns kirby+0x0c

// CPU brain helpers, all reached from TopRide_CpuInputThink. TopRide_CpuPerceive
// fills a stack "blackboard" (sector look-ahead, nearest rival/item/obstacle, per-
// difficulty weights) that is handed to the four situation detectors. The detectors
// run in order; the FIRST that commits short-circuits the rest and suppresses the
// route-followers. If none commit, one of the two route-followers steers the racing
// line. All write reader->heading (+0x08) + a press/charge flag. (blackboard arg is
// a float* stack scratch; signatures approximate.)
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

// Convert a TopRideMachineKind to the corresponding MachineKind (VCKIND)
// for indexing into `APSave.machine_unlocked_mask` (via the global `ap_save`).
#define TOPRIDE_MACHINE_TO_VCKIND(tr) ((tr) == TR_MACHINE_FREE ? VCKIND_FREE : VCKIND_STEER)

// Per-slot config accessors. Backed by the 9-byte-stride config block at
// GameData[slot*9 + 0xD20], committed from the lobby fields at TR scene-exit
// (see TopRide_PreGameThink and TopRide_OnCourseSelect for the writer paths).
TopRidePlayerKind TopRide_GetPlayerKind(int slot);                   // 0x8000bd6c, reads byte +0
void TopRide_SetPlayerKind(int slot, TopRidePlayerKind kind);        // 0x8000bda8, writes byte +0
u8   TopRide_GetColor(int slot);                                     // 0x8000bdf0, reads byte +1
void TopRide_SetColor(int slot, u8 color);                           // 0x8000be2c, writes byte +1
void TopRide_SetCpuLevel(int slot, u8 level);                        // 0x8000be74, writes byte +2 (0..4). NOTE: inert - NOT the CSS "CPU Level"; that control is the +3 byte below. Stays 0 in normal play, not read by the CPU AI.
void TopRide_SetHandicap(int slot, u8 handicap);                     // 0x8000bf04, writes byte +3 (0..4). The CSS per-slot skill control ("Handicap" for humans / "CPU Level" for CPUs); this byte drives the Top Ride CPU steering AI (see TopRideCpuInputReader.difficulty).
void TopRide_SetControllerPort(int slot, u8 port);                   // 0x8000bebc, writes byte +6
TopRideMachineKind TopRide_GetMachineKind(int slot);                 // 0x8000bf4c, reads byte +8
void TopRide_SetMachineKind(int slot, TopRideMachineKind machine);   // 0x8000bf8c, writes byte +8

// TR lobby (player-select) state-machine entrypoints. Called per-frame per-slot
// from TopRide_PreGameThink (0x8002c06c) based on the slot's `ply_state`:
//   ply_state == 0 (selecting / just-joined) -> TopRide_CSS_SelectingThink
//   ply_state == 1 (ready / confirmed)        -> TopRide_CSS_ReadyThink
//   ply_state == 2 (in-panel editing)         -> TopRide_CSS_PanelThink
// All three operate on `GameData.topride_select_ply` (see game.h). The L/R
// machine cycler lives in `TopRide_CSS_PanelThink` at 0x8002be20..0x8002be94
// (sub-cursor row 1 = "Control Type"), reading/writing `panel_machine[slot]`.
//
// These are the **multiplayer race** lobby thinks, dispatched from
// TopRide_PreGameThink (TopRide_GetMode() == 0). The solo Free Run / Time
// Attack lobby uses a completely separate think - see TopRide_SoloPanelThink.
void TopRide_CSS_SelectingThink(int slot);  // 0x8002ac68
void TopRide_CSS_ReadyThink(int slot);      // 0x8002b094
void TopRide_CSS_PanelThink(int slot);      // 0x8002b8a8

// Solo lobby panel-editing think for Free Run / Time Attack, dispatched from
// TopRide_OnCourseSelect (0x8002cc30) for slots with ply_state != 1. This is
// the solo counterpart to TopRide_CSS_PanelThink and carries its OWN copy of
// the "Control Type" L/R machine cycler at 0x8002cb88..0x8002cbec
// (reads/writes panel_machine[panel] at lobby offset 0x2f, tests RIGHT
// 0x80002 / LEFT 0x40001 - identical bits to the race cycler). Because solo
// never routes through TopRide_CSS_PanelThink, this cycler needs its own
// unlock gate (see gate_machines.c, hook at 0x8002cb98).
void TopRide_SoloPanelThink(int slot);      // 0x8002ca80

// Reinitializes the TR lobby block (memsets 0x39 bytes at GameData+0x197 then
// writes per-slot defaults: panel_machine=0 (Free Star), color=slot,
// handicap=2, panel_field_d=0xff, etc.).
void TopRide_InitSelectData(void);                                   // 0x8002cfd8

// State ID returned by state_handler->vt[+0x0C]() for the current Kirby state.
// See docs/topride-kirby-states.md for the full state machine and class table.
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

// Read the current state ID via state_handler->vt[+0x0C](). Safe once
// round_state == 2; state_handler is NULL / partially wired before that.
//
// **Caveat:** only some states override the get_state_id slot - KirbyNormal
// and the abstract KirbyDamage base inherit a stub (0x802e4a44) that returns
// 0, so the return value is unreliable for "am I in state X?" checks. (Note:
// KirbyBurn DOES override it - its slot at 0x802fcc7c returns 10.) Use
// TopRide_KirbyHasStateVtable for reliable state identification instead.
static inline TopRideKirbyStateId TopRide_KirbyGetStateId(TopRideKirby *kirby)
{
    void **state_vt = *(void ***)kirby->state_handler;
    int (*get_id)(void *) = (int (*)(void *))state_vt[3]; // vtable byte offset 0x0C
    return (TopRideKirbyStateId)get_id(kirby->state_handler);
}

// State class vtable addresses (from docs/topride-kirby-states.md "State
// Classes"). Compare against state_handler->vtable to reliably identify the
// kirby's current state - this is what dynamic_cast does inside the Group A
// wrappers and doesn't depend on per-state get_state_id overrides.
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

// True if the kirby's current state instance has the given vtable. Reliable
// way to ask "is this kirby in state X right now?" - the wrapper at
// kirby->vtable[+offset] uses the same comparison via dynamic_cast.
static inline int TopRide_KirbyHasStateVtable(TopRideKirby *kirby, void *state_vt)
{
    if (!kirby || !kirby->state_handler)
        return 0;
    return *(void **)kirby->state_handler == state_vt;
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
// the per-effector math - not needed for trap effects.

// KirbyExplode ("tumble") - vanilla bomb / landmine state.
// Wrapper: TopRide_KirbyExplodeMethod at 0x802d5834 (vtable[59], +0xEC).
// Vanilla caller: EffectorExplode_ApplyToKirby at 0x802e6898.
static inline void TopRide_KirbyExplode(TopRideKirby *kirby)
{
    Vec3 zero = {0.0f, 0.0f, 0.0f};
    typedef void (*Method)(TopRideKirby *, u16, Vec3 *, u16, u16);
    ((Method)(((void **)kirby->vtable)[59]))(kirby, 0, &zero, 0, 0);
}

// KirbyCrush - heavy-machine collision state. The setter installs AC_TOBASARE
// (knockback launch), the same animation as Explode/Strike - Crush is NOT the
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

// KirbyStrike - generic hit reaction.
// Wrapper at 0x802d5900 (vtable[60], +0xF0).
static inline void TopRide_KirbyStrike(TopRideKirby *kirby)
{
    Vec3 zero = {0.0f, 0.0f, 0.0f};
    typedef void (*Method)(TopRideKirby *, u16, Vec3 *, u16, u16);
    ((Method)(((void **)kirby->vtable)[60]))(kirby, 0, &zero, 0, 0);
}

// KirbyBurn - fire damage-over-time state. Blocked if currently in KirbyElec.
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

// KirbyFreeze - frozen solid.
// Wrapper at 0x802d56bc (vtable[57], +0xE4).
static inline void TopRide_KirbyFreeze(TopRideKirby *kirby)
{
    typedef void (*Method)(TopRideKirby *);
    ((Method)(((void **)kirby->vtable)[57]))(kirby);
}

// KirbyPress - pressed / squeezed.
// Wrapper at 0x802d54ec (vtable[55], +0xDC).
static inline void TopRide_KirbyPress(TopRideKirby *kirby)
{
    typedef void (*Method)(TopRideKirby *);
    ((Method)(((void **)kirby->vtable)[55]))(kirby);
}

// KirbySpin - spin-out. KirbySandSpin variant lives at vtable[62] / +0xF8.
// Wrapper at 0x802d59cc (vtable[61], +0xF4). The setter (0x802f7718) feeds
// arg2 to the PSVECMagnitude call at 0x802f7998 to size the spin knockback; passing
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

// KirbyNumb - paralysis. Blocked if state ID == 13 (KirbyDoodlebugOut).
// Wrapper at 0x802d5b74 (vtable[64], +0x100).
static inline void TopRide_KirbyNumb(TopRideKirby *kirby)
{
    typedef void (*Method)(TopRideKirby *);
    ((Method)(((void **)kirby->vtable)[64]))(kirby);
}

// KirbyElec - electrified. Blocked if state ID == 13 (KirbyDoodlebugOut).
// Wrapper at 0x802d5be4 (vtable[65], +0x104).
static inline void TopRide_KirbyElec(TopRideKirby *kirby)
{
    typedef void (*Method)(TopRideKirby *, u32);
    ((Method)(((void **)kirby->vtable)[65]))(kirby, 0);
}

// KirbyConfuse - controls reversed / scrambled (AC_PANIC animation).
// Blocked if state ID == 13 (KirbyDoodlebugOut).
// Wrapper at 0x802d5c64 (vtable[66], +0x108).
static inline void TopRide_KirbyConfuse(TopRideKirby *kirby)
{
    typedef void (*Method)(TopRideKirby *, u32);
    ((Method)(((void **)kirby->vtable)[66]))(kirby, 0);
}

// KirbySpeedDown - speed debuff (matches the SpeedDown TR item).
// Has an extra predicate guard at state_handler->vt[+0x48].
// Wrapper at 0x802d5da4 (vtable[68], +0x110).
static inline void TopRide_KirbySpeedDown(TopRideKirby *kirby)
{
    typedef void (*Method)(TopRideKirby *, u32);
    ((Method)(((void **)kirby->vtable)[68]))(kirby, 0);
}

#endif // KAR_H_TOPRIDE
