#ifndef KAR_H_RIDER
#define KAR_H_RIDER

#include "datatypes.h"
#include "obj.h"
#include "hurt.h"
#include "collision.h"
#include "machine.h"
#include "camera.h"
#include "trigger.h"
#include "color.h"

typedef enum RiderKind
{
    RDKIND_KIRBY,
    RDKIND_DEDEDE,
    RDKIND_METAKNIGHT,
    RDKIND_NUM,
} RiderKind;

typedef enum RiderPri
{
    RDPRI_0,
    RDPRI_ANIM,
    RDPRI_INPUT,
    RDPRI_3,
    RDPRI_PHYS,
    RDPRI_ENVCOLL,
    RDPRI_6,
    RDPRI_7,
    RDPRI_8,
    RDPRI_HITCOLL,
    RDPRI_DMGAPPLY,
    RDPRI_13 = 13,
    RDPRI_15 = 15,
} RiderPri;

// RiderData.status. RiderStateChange (0x8018e580) takes states below
// RDSTATE_COMMON_NUM from the common table and the rest from the rider kind's own
// table. From RDSTATE_READY on the values are Kirby's: Dedede matches only through
// RDSTATE_SPINTURNEND. Meta Knight's ACCELN..ACCELFEND sit one lower and his
// PUSHSTART..SPINTURNEND one higher.
typedef enum RiderStatus
{
    RDSTATE_WAIT,
    RDSTATE_BOARD,
    RDSTATE_BOARDEND,
    RDSTATE_SLEEPSTART,
    RDSTATE_SLEEP,
    RDSTATE_SLEEPEND,
    RDSTATE_DAMAGE,
    RDSTATE_DAMAGEFIRE,
    RDSTATE_DAMAGECUTUP,
    RDSTATE_DAMAGESPIN,
    RDSTATE_DAMAGEELEC,
    RDSTATE_DAMAGENUMB,
    RDSTATE_DAMAGEICE,
    RDSTATE_GETOFF_DAMAGE,
    RDSTATE_GETOFF_DAMAGEFIRE,
    RDSTATE_GETOFF_DAMAGECUTUP,
    RDSTATE_GETOFF_DAMAGESPIN,
    RDSTATE_GETOFF_DAMAGEELEC,
    RDSTATE_GETOFF_DAMAGENUMB,
    RDSTATE_GETOFF_DAMAGEICE,
    RDSTATE_GETOFF_DAMAGEFALL,
    RDSTATE_GETOFF_DOWNBOUND,
    RDSTATE_GETOFF_DOWNWAIT,
    RDSTATE_GETOFF_DOWNMOVE,
    RDSTATE_GETOFF_FALLDEATH,
    RDSTATE_GETOFF_FALLDEATHWAIT,
    RDSTATE_FREEMOVE,
    RDSTATE_SELECT,
    RDSTATE_DEATHRETURN,
    RDSTATE_COMMON_NUM,
    RDSTATE_READY = RDSTATE_COMMON_NUM,
    RDSTATE_READYPUSHSTART,
    RDSTATE_READYPUSH,
    RDSTATE_READYPUSHEND,
    RDSTATE_RUN,
    RDSTATE_RUN2,
    RDSTATE_FLY,
    RDSTATE_ACCELN,
    RDSTATE_ACCELNEND,
    RDSTATE_ACCELF,
    RDSTATE_ACCELFEND,
    RDSTATE_PUSHSTART,
    RDSTATE_PUSH,
    RDSTATE_PUSHEND,
    RDSTATE_PUSHFORWARD,
    RDSTATE_QUICKSPINTURN,
    RDSTATE_SPINTURN,
    RDSTATE_SPINTURNEND,
    RDSTATE_DRAWSTART,
    RDSTATE_DRAW,
    RDSTATE_DRAWEND,
    RDSTATE_HOLD,
    RDSTATE_SPIT,
    RDSTATE_SWALLOW,
    RDSTATE_SLEEPGET,
    RDSTATE_CP_SLEEPSTART,
    RDSTATE_CP_SLEEP,
    RDSTATE_CP_SLEEPEND,
    RDSTATE_FIREGET,
    RDSTATE_SWORDGET,
    RDSTATE_BOMBGET,
    RDSTATE_PLASMAGET,
    RDSTATE_NEEDLEGET,
    RDSTATE_MIKEGET,
    RDSTATE_ICEGET,
    RDSTATE_TORNADOGET,
    RDSTATE_TIREGET,
    RDSTATE_BIRDGET,
    RDSTATE_CRACKERGET,
    RDSTATE_CRACKERRUN,
    RDSTATE_CRACKERPUSHSTART,
    RDSTATE_CRACKERPUSH,
    RDSTATE_CRACKERPUSHEND,
    RDSTATE_SENSORBOMBGET,
    RDSTATE_SENSORBOMBRUN,
    RDSTATE_SENSORBOMBPUSHSTART,
    RDSTATE_SENSORBOMBPUSH,
    RDSTATE_GORDOGET,
    RDSTATE_GORDORUN,
    RDSTATE_GORDOPUSHSTART,
    RDSTATE_GORDOPUSH,
    RDSTATE_PANICSPINSTART,
    RDSTATE_PANICSPINLOOP,
    RDSTATE_PANICSPINEND,
    RDSTATE_SWORDATTACK1START,      // entered by Rider_FindAutoAttackTarget
    RDSTATE_SWORDATTACK1,
    RDSTATE_SWORDATTACK1END,        // a target found here chains SWORDATTACK2
    RDSTATE_SWORDATTACK2,
    RDSTATE_SWORDATTACK2END,        // and here SWORDATTACK3
    RDSTATE_SWORDATTACK3,
    RDSTATE_SWORDATTACK3END,
    RDSTATE_BOMBRUN,                // bomb held; A enters BOMBPUSHSTART
    RDSTATE_BOMBPUSHSTART,
    RDSTATE_BOMBPUSH,               // throw charge builds; releasing A throws
    RDSTATE_BOMBTHROW,
    RDSTATE_MIKESING = 97,          // singing blast (Effect 0x5a5a2 / SFX 0x2006b)
    RDSTATE_MIKEEND,
    RDSTATE_LOSEABILITY = 104,
    RDSTATE_LANDOK,
    RDSTATE_LANDGREAT,
    RDSTATE_LANDBAD,
    RDSTATE_EXITSTAR = 110,
    RDSTATE_LAND,
    RDSTATE_FALL,
    RDSTATE_WALKWAIT,
    RDSTATE_DASH,
    RDSTATE_GETONSTAR,
    RDSTATE_RUNBRAKE,
    RDSTATE_DASHTURN,
    RDSTATE_JUMPSQUAT,
    RDSTATE_JUMP,
    RDSTATE_DASHINIT,
    RDSTATE_SWIMWAIT,
    RDSTATE_SWIMMOVE,
    RDSTATE_SWIMTURN,
    RDSTATE_JUMP2,
    RDSTATE_STANDUP,
    RDSTATE_LEGENDARYASSEMBLY = 130,
    RDSTATE_NUM,
} RiderStatus;

// RiderData.mstatus, the motion (animation) id a state's action resolves to. Values are
// base Kirby's; ability bits at RiderData+0xa30 remap them.
typedef enum RiderMotionStatus
{
    RD_MSTATUS_PUSHSTART = 100,
    RD_MSTATUS_PUSH,
    RD_MSTATUS_PUSHTURNL,
    RD_MSTATUS_PUSHTURNR,
    RD_MSTATUS_PUSHEND,
    RD_MSTATUS_PUSHFORWARD,
} RiderMotionStatus;

typedef enum CopyKind
{
    COPYKIND_NONE = -1,
    COPYKIND_FIRE = 0,
    COPYKIND_TIRE,
    COPYKIND_SLEEP,
    COPYKIND_SWORD,
    COPYKIND_BOMB,
    COPYKIND_PLASMA,
    COPYKIND_NEEDLE,
    COPYKIND_MIKE,
    COPYKIND_ICE,
    COPYKIND_TORNADO,
    COPYKIND_BIRD,
    COPYKIND_NUM,
} CopyKind;

static const char *const CopyKind_Names[COPYKIND_NUM] = {
    [COPYKIND_FIRE]    = "Fire",
    [COPYKIND_TIRE]    = "Wheel",
    [COPYKIND_SLEEP]   = "Sleep",
    [COPYKIND_SWORD]   = "Sword",
    [COPYKIND_BOMB]    = "Bomb",
    [COPYKIND_PLASMA]  = "Plasma",
    [COPYKIND_NEEDLE]  = "Needle",
    [COPYKIND_MIKE]    = "Mic",
    [COPYKIND_ICE]     = "Freeze",
    [COPYKIND_TORNADO] = "Tornado",
    [COPYKIND_BIRD]    = "Wing",
};

// Direct byte values stored in RiderData.color_idx and player-select color[]
// arrays. Order is the game's: 0-3 are default (P1-P4), 4-7 are checklist
// unlocks.
typedef enum KirbyColor
{
    KIRBYCOLOR_PINK   = 0,
    KIRBYCOLOR_YELLOW = 1,
    KIRBYCOLOR_BLUE   = 2,
    KIRBYCOLOR_RED    = 3,
    KIRBYCOLOR_GREEN  = 4,
    KIRBYCOLOR_PURPLE = 5,
    KIRBYCOLOR_BROWN  = 6,
    KIRBYCOLOR_WHITE  = 7,
    KIRBYCOLOR_NUM    = 8,
} KirbyColor;

static const char *const KirbyColor_Names[KIRBYCOLOR_NUM] = {
    [KIRBYCOLOR_PINK]   = "Pink",
    [KIRBYCOLOR_YELLOW] = "Yellow",
    [KIRBYCOLOR_BLUE]   = "Blue",
    [KIRBYCOLOR_RED]    = "Red",
    [KIRBYCOLOR_GREEN]  = "Green",
    [KIRBYCOLOR_PURPLE] = "Purple",
    [KIRBYCOLOR_BROWN]  = "Brown",
    [KIRBYCOLOR_WHITE]  = "White",
};

typedef enum PowerUpKind
{
    POWERUPKIND_NONE = -1,
    POWERUPKIND_FIRECRACKER = 0,
    POWERUPKIND_SENSORBOMB,
    POWERUPKIND_GORDO,
    POWERUPKIND_PANICSPIN,
    POWERUPKIND_NUM,
} PowerUpKind;

typedef struct rdDataKirby
{
    void *attr; // 0x0
    struct
    {
        JOBJDesc *jobjdesc;
        MatAnimJointDesc *matanimjointdesc;
        void *high_poly_table;
        void *mid_poly_table;
        void *low_poly_table;
        void *texture_table;
    } *model;  // 0x4
    void *x8;  // 0x8
    void *xc;  // 0xc
    void *x10; // 0x10
    struct
    {
        float radius;  // 0x0, size of kirby's collision sphere
        float x4;      // 0x4
        float radius2; // 0x8, radius again?
        float xc;      // 0xc
    } *coll;           // 0x14
    struct
    {
        int bone_idx; // 0x0,
        int x4;       // 0x4
        float radius; // 0x8, radius
        Vec3 offset;  // 0xC
    } *jostle;        // 0x18, sphere that detects walking nudge collision
} rdDataKirby;

// One patch_drop_mode's throw ranges. Each pair is lerp(lo, hi, HSD_Randf()): A is
// the throw speed, B the elevation angle in degrees, C the forward offset from the
// hand bone to the spawn point.
typedef struct PatchDropModeParams
{
    f32 lo_a;  // 0x00
    f32 hi_a;  // 0x04
    f32 lo_b;  // 0x08
    f32 hi_b;  // 0x0c
    f32 lo_c;  // 0x10
    f32 hi_c;  // 0x14
} PatchDropModeParams;

// Rider-wide tuning, the first member of RdCommon.dat's "rdDataCommon". The input
// thresholds at the top and the fields past the patch-drop block are unmapped.
typedef struct RiderCommonParam
{
    u8 x0[0x1a8];                                 // 0x0
    f32 quickspin_stick_x;                        // 0x1a8, lstick.X a side counts as held at
    f32 quickspin_x1ac;                           // 0x1ac, a side stops counting as held once RiderData+0x4b4 reaches this
    f32 quickspin_flick_window;                   // 0x1b0, frames since the other side was held for a flick to spin
    f32 x1b4;                                     // 0x1b4
    f32 patch_drop_damage_min;                    // 0x1b8, a hit whose truncated damage exceeds this drops patches
    int patch_drop_mode0_count;                   // 0x1bc, drop count for mode 0, which ignores the stat array
    int patch_drop_throw_flag;                    // 0x1c0, CityItem_Throw's flag, stored at item+0x248
    f32 patch_drop_spawn_y_bias;                  // 0x1c4, added to spawn position Y in both sub-handlers
    f32 patch_drop_mode2_factor;                  // 0x1c8, multiplied with sum-of-positive-stats to size mode-2 drops
    f32 patch_drop_mode1_factor;                  // 0x1cc, multiplied with sum-of-positive-stats to size mode-1 drops
    f32 patch_drop_throw_spread;                  // 0x1d0, max throw-spread half-angle in degrees; scaled by
                                                  //        a random factor whose sign alternates with the count
    PatchDropModeParams patch_drop_mode0_params;  // 0x1d4
    PatchDropModeParams patch_drop_mode1_params;  // 0x1ec
    PatchDropModeParams patch_drop_mode2_params;  // 0x204
    int patch_drop_cooldown_init;                 // 0x21c, frames until the next spawn after a successful one
    int patch_drop_burst_threshold;               // 0x220, when patch_drop_progress reaches this, switch from sequential to burst
    int patch_drop_allup_rng_max;                 // 0x224, mode-0 only: HSD_Randi ceiling for the all-up RNG roll
    u8 x228[0x274 - 0x228];                       // 0x228, read up to 0x270; full size unknown
} RiderCommonParam;

// Root of RdCommon.dat, loaded to the stay heap by fn_rdLoadCommon (0x80190418).
typedef struct rdDataCommon
{
    RiderCommonParam *param; // 0x0, mirrored to stc_rider_param
    void *x4;                // 0x4
    void *x8;                // 0x8
    void *xc;                // 0xc
    void *x10;               // 0x10
} rdDataCommon;

// CPU rider AI state, RiderData.cpu (NULL for humans). Its leading fields are the
// virtual pad RiderGObj_InputThink reads back through Rider_GetCPU*. ai_state is a
// profile fixed at init; maneuver emits a command stream into cmd_buffer only while
// the command VM is idle, and Rider_CPUProcessCmd plays it into the pad fields.

// CpuData.ai_state profiles; 4 and values above 10 share the CRUISE_PLUS handler.
typedef enum CpuAIState
{
    CPUSTATE_CRUISE = 1,            // racing line, combat muted
    CPUSTATE_CRUISE_PLUS = 2,       // racing line with item/attack desires
    CPUSTATE_NAVIGATE = 3,          // City Trial city path-finding
    CPUSTATE_ROUTE_FOLLOW = 5,      // steer at Rider_CPUScanRouteGoal's pick (inhalable enemies), no look-ahead
    CPUSTATE_ROUTE_FOLLOW_CITY = 6, // ROUTE_FOLLOW plus city objects
    CPUSTATE_CHARGE = 7,            // drive to a charge anchor and charge
    CPUSTATE_ATTACK = 8,            // chase and spin-attack the nearest rival, no look-ahead
    CPUSTATE_REPOSITION = 9,        // stage-specific position nudges
    CPUSTATE_PATROL = 10,           // timed toggle between nav target and rival
} CpuAIState;

typedef struct CpuData
{
    int buttons;           // 0x00, synthesized button mask -> RiderData.held (0x3d8)
    s16 stick_x;           // 0x04, synthesized stick X; Rider_GetCPUStickX truncates it into RiderData.stickX
    s16 stick_y;           // 0x06, synthesized stick Y; Rider_GetCPUStickY truncates it into RiderData.stickY
    int ai_state;          // 0x08, CpuAIState profile (0 asserts) set once at init, dispatched by Rider_CPUDecideState
    u8  machine_is_bike;   // 0x0c, is_bike of RiderData.machine_gobj, -1 with none (refreshed each perceive)
    u8  machine_kind;      // 0x0d, its MachineGObj_GetAbsoluteKind, -1 with none; only Rider_CPUEmitSteerStick
                           //       reads it, comparing against Hydra, Winged and Jet
    u8  city_kind;         // 0x0e, Gm_GetCityKind() captured at init (selects the AI profile)
    u8  stage_kind;        // 0x0f, stGetCurrentStageKind() captured at init (selects the AI profile)
    int maneuver;          // 0x10, TACTICAL maneuver (0..0x15), dispatched by Rider_ProcessCPUManeuver
    int base_maneuver;     // 0x14, fallback maneuver a strategic state parks on (set to 1 or 2); handlers return here via `maneuver = base_maneuver`
    int scratch_18;        // 0x18, cleared at the top of each decide pass but never read (vestigial)
    uint desire_flags;     // 0x1c, INHIBITOR bits (set = suppress a reaction), seeded from the status
                           //       and cleared each decide pass. 0x100 no-ram-press, 0x200 no-avoidance,
                           //       0x400 no-dodge/attack-scan, 0x1000000 no-charge/intercept
    u8  suppress_timer;    // 0x20, countdown; while > 0 the perceive stage forces target_secondary = -1
    u8  x21;               // 0x21
    s16 difficulty_level;  // 0x22, AI skill level 0..8 (asserts above 8); Rider_CPUDifficultyScale scales every personality roll by it
    float random_seed;     // 0x24, per-CPU jitter seed = HSD_Randf() at Rider_CPUInit; read by route/targeting helpers
    int frame_counter;     // 0x28, ++ every perceive pass
    u8  behavior_flags;    // 0x2c, ENABLE bits rewritten per strategic state. 0x01 opportunistic-action,
                           //       0x02 extra-hazard-pass, 0x08 target-steer, 0x20 predictive-lead,
                           //       0x40 rival-pursuit, 0x80 forward-lookahead (0x04/0x10 unused)
    u8  status_flags;      // 0x2d, bit 0x02 = velocity-stuck, bit 0x04 = position-stuck (set by perceive)
    u8  x2e;               // 0x2e
    u8  x2f;               // 0x2f
    u16 vel_stuck_timer;   // 0x30, frames moving too slow / against facing (anti-stuck)
    s16 maneuver_step;     // 0x32, multi-step sequencer counter (charge-stutter press/hold cycle); gated by status bit 0x02
    s16 route_scratch_34;  // 0x34, route/target scratch (init -1)
    s16 ramcharge_phase;   // 0x36, RamCharge (maneuver 3) phase counter, read modulo-N
    int target_primary;    // 0x38, primary nav target id (-1 = none); resolved via Rider_CPUResolveTargetPos
    float target_lead;     // 0x3c, the target node's arc-length [0,1] position, not a predict time
    float secondary_lead;  // 0x40, signed secondary-target lead scalar (+/-1); a sign flip clears target_secondary
    int target_secondary;  // 0x44, secondary nav target id (-1 = none); suppressed while suppress_timer > 0
    int target_secondary_flag; // 0x48, modifies the secondary target's lead time
    int  blocked_node_id;  // 0x4c, anti-stuck: cached unreachable nav-node id (-1 = none); set by the stuck-recovery sweep (states 3/6)
    Vec3 escape_offset;    // 0x50, anti-stuck: escape-direction offset added to position when re-pathing out of a stuck spot
    u8  blocked_counter;   // 0x5c, consecutive-blocked counter (wraps >8) from the maneuver-feasibility probe
    u8  path_retry_counter; // 0x5d, path-retry / stuck counter (wraps >4), states 3/6
    s16 wander_timer;      // 0x5e, Patrol (state 10) wander/oscillation countdown (init 0x4b0; reloads 900)
    Vec3 recorded_pos;     // 0x60, anti-stuck reference position (compared against pos each frame)
    int pos_stuck_timer;   // 0x6c, frames spent within range of recorded_pos (anti-stuck)
    s16 rival_ply;         // 0x70, target rival's ply (5 = none); written by the rival selector, read by attack/patrol via Ply_GetPosition
    s16 rival_reselect_timer; // 0x72, frames until the rival is re-picked (HSD_Randi(0x3c)+0x3c)
    void *item_target;     // 0x74, cached item / chase-object GObj* (0 = none); set by the item-target scan (states 3/8)
    Vec3 item_target_pos;  // 0x78, cached world position of item_target
    void *city_object;     // 0x84, cached city-prop / actor GObj* (state 3); 0 = none
    Vec3 city_object_pos;  // 0x88, cached world position of city_object
    void *route_goal;      // 0x94, cached highest-scored route-goal GObj* (states 5/6); 0 = none
    Vec3 route_goal_pos;   // 0x98, cached world position of route_goal
    Vec3 *nav_target_ptr;  // 0xa4, -> the steering target position (track look-ahead point), or NULL
    void *interaction_target; // 0xa8, priority interaction target; overrides item_target in the command VM
    void *path_point;      // 0xac, secondary path-point ptr (state 3 city look-ahead)
    void *charge_anchor;   // 0xb0, state 7 (Charge) fixed-anchor ptr
    int  charge_anchor_id; // 0xb4, state 7 resolved anchor node id
    Vec3 nav_target_pos;   // 0xb8, resolved navigation target, copied from *nav_target_ptr (the raw target)
    Vec3 steer_target_pos; // 0xc4, what maneuvers steer toward: nav_target_pos after the city-object override
    int  xd0;              // 0xd0, x00 of the CpuForwardTarget the arbiter picked for RamCharge/PursueLOS
    Vec3 ramcharge_target_pos; // 0xd4, that target's position, steered at by maneuvers 3/4
    void *xe0;             // 0xe0, current path/spline object pointer
    u8  route_header;      // 0xe4, packed route cache header (bit7 = valid, bits 2..5 = entry count)
    u8  xe5[3];            // 0xe5
    struct { int id; int flag; } route_entries[5]; // 0xe8, upcoming nav-node route cache
    int cmd_timer;         // 0x110, command countdown; reads next opcode when it hits 0
    u8 *cmd_read_ptr;      // 0x114, command VM playback position within cmd_buffer (0 = idle)
    u8 *cmd_write_ptr;     // 0x118, where maneuver handlers append opcodes (reset to cmd_buffer each maneuver)
    u8  cmd_buffer[0x80];  // 0x11c, command opcode stream
} CpuData;


// CpuData.maneuver, committed by Rider_CPUArbitrateManeuver.
typedef enum CpuManeuver
{
    CPUMAN_COAST = 0x00,
    CPUMAN_RECOVER_FORWARD = 0x01,
    CPUMAN_STEER_TO_NAV = 0x02,
    CPUMAN_RAM_CHARGE = 0x03,
    CPUMAN_PURSUE_LOS = 0x04,
    CPUMAN_APPROACH_WAYPOINT = 0x05, // 5 and 6 share a handler
    CPUMAN_APPROACH_WAYPOINT_CITY = 0x06,
    CPUMAN_CHARGE_HOLD = 0x07,
    CPUMAN_AVOID_OBSTACLE = 0x08,
    CPUMAN_DODGE_WEAPON = 0x09,
    CPUMAN_CHARGE_RELEASE = 0x0a,
    CPUMAN_STEER_TARGET_WIGGLE = 0x0b,
    CPUMAN_STEER_TARGET_ADVANCE = 0x0c,
    CPUMAN_CHARGE_CENTERED = 0x0d,
    CPUMAN_NAV_STEER = 0x0e,
    CPUMAN_NAV_STEER_TAP = 0x0f,
    CPUMAN_CHARGED_NAV_STEER = 0x10,
    CPUMAN_TAP_ONCE = 0x12,
    CPUMAN_WIGGLE = 0x13,
    CPUMAN_BRAKE = 0x14,
    CPUMAN_CHARGE_STILL = 0x15,
} CpuManeuver;

// CpuData.desire_flags inhibitors.
typedef enum CpuDesireFlag
{
    CPUDESIRE_NO_RAM = 0x100,        // RamCharge's press
    CPUDESIRE_NO_DODGE = 0x400,      // the hazard Wiggle and Attack's spin burst
    CPUDESIRE_NO_CHARGE = 0x1000000, // ChargeCentered
} CpuDesireFlag;

// Per-frame scratch for the rider being updated, zeroed by Rider_ProcessCPUDistance.
typedef struct CpuHazard
{
    int x00;              // 0x00
    int x04;              // 0x04
    int x08;              // 0x08
    int x0c;              // 0x0c, x04..x0c all zero marks an empty entry
    u8 x10[0x28];         // 0x10
    float time_to_impact; // 0x38
    u8 imminent : 1;      // 0x3c, 0x80
    u8 x3c : 7;           // 0x3c
    u8 x3d[3];            // 0x3d
} CpuHazard;

typedef struct CpuHazardList
{
    CpuHazard entries[8]; // 0x000
    int num;              // 0x200
    u8 x204[0xc];         // 0x204
} CpuHazardList;


typedef struct CpuForwardTarget
{
    int x00;  // 0x00
    Vec3 pos; // 0x04
    int side; // 0x10, 1 = RamCharge candidate, 0 = PursueLOS candidate
} CpuForwardTarget;

typedef struct CpuForwardList
{
    CpuForwardTarget entries[8]; // 0x00
    int num;                     // 0xa0
    u8 xa4[0xc];                 // 0xa4
} CpuForwardList;


static CpuHazardList *stc_cpu_hazards = (CpuHazardList *)0x8055e698;      // Rider_CPUCollectHazards
static CpuForwardList *stc_cpu_forward = (CpuForwardList *)0x8055e8b4;    // Rider_CPUForwardLookahead

typedef enum CpuMachineCapFlag
{
    CPUMACHCAP_SKIP_PASSAGE_BRANCH = 0x08, // Machine Passage: no preferred route branch
    CPUMACHCAP_RAM_CHARGE = 0x10,
    CPUMACHCAP_CHARGE_HOLD = 0x20,
    CPUMACHCAP_BRAKE = 0x40,               // at difficulty > 3; CHARGE_HOLD needs it too
    CPUMACHCAP_SWAP = 0x80,                // may leave this machine for a better field machine
} CpuMachineCapFlag;

// One per machine kind, star and bike tables back to back.
typedef struct CpuMachineCaps
{
    int kind;             // 0x00, label only
    int swap_score;       // 0x04, desirability as a field machine; Rider_CPUScanCityObjects skips 0
    u8 flags;             // 0x08, CpuMachineCapFlag
    u8 x09[3];            // 0x09
    float charge_release; // 0x0c, a charge-holding CPU keeps holding while the gauge is at or under this
    float x10;            // 0x10, never read
} CpuMachineCaps;


// One per star kind. The bike table after it is never read: bikes and riders with no
// machine get the Warp Star's row.
typedef struct CpuMachineSteer
{
    int kind;             // 0x00, label only
    float align_cos_near; // 0x04, heading dot above this needs no correction
    float align_cos_far;  // 0x08, ...below this, the larger one
    float turn_tolerance; // 0x0c, radians, Rider_CPUEmitSteer
    float stuck_angle;    // 0x10, radians past which Rider_CPUTrackStuckProgress counts the rider stuck
} CpuMachineSteer;

// One per MachineKind, 25 entries, for the Air Glider and High Jump stadiums.
typedef struct CpuStadiumMachineParam
{
    float pitch;   // 0x00, radians
    float min_len; // 0x04, High Jump returns a pitch of 0 at or under this length; Air Glider never reads it
} CpuStadiumMachineParam;

static CpuMachineCaps *stc_cpu_machine_caps_star = (CpuMachineCaps *)0x804b8854;             // [VCSTAR_NUM]
static CpuMachineCaps *stc_cpu_machine_caps_bike = (CpuMachineCaps *)0x804b89d0;             // [VCWHEEL_NUM]
static CpuMachineSteer *stc_cpu_machine_steer_star = (CpuMachineSteer *)0x804b8f30;          // [VCSTAR_NUM]
static CpuStadiumMachineParam *stc_cpu_airglider_machine = (CpuStadiumMachineParam *)0x804b8a5c; // [25]
static CpuStadiumMachineParam *stc_cpu_highjump_machine = (CpuStadiumMachineParam *)0x804b8b24;  // [25]

// The constants MachineGObj_CPUGetChargeHoldGate and MachineGObj_CPUGetChargeReleaseOverride
// load for the kinds they switch on.
static float *stc_cpu_charge_gate_low = (float *)0x805e31a4; // second gate of Bulk, Rocket and Formula
static float *stc_cpu_charge_gate_bulk = (float *)0x805e31a8;
static float *stc_cpu_charge_gate_hydra_low = (float *)0x805e31ac;
static float *stc_cpu_charge_gate_hydra = (float *)0x805e31b0;
static float *stc_cpu_charge_gate_rocket = (float *)0x805e31b4;
static float *stc_cpu_charge_gate_formula = (float *)0x805e31b8;
static float *stc_cpu_charge_release_bulk = (float *)0x805e31bc;
static float *stc_cpu_charge_release_hydra = (float *)0x805e31c0;

// One status. RiderStateChange installs the callbacks and hands attack_log to
// Rider_AssignAttackLog.
typedef struct RiderStateDesc
{
    int mstatus;       // 0x00, -1 = none
    int attack_log;    // 0x04, 0 on states whose hitboxes credit no player
    void *callback[6]; // 0x08, anim, input, phys, envcoll, x7c4, x7c8
} RiderStateDesc;


typedef struct RiderData
{
    GOBJ *gobj;                           // 0x0, the rider's own GObj; Rider_InitData (0x8018ddc4) writes it back
    RiderKind kind;                       // 0x4
    u8 ply;                               // 0x8
    u8 controller_index;                  // 0x9
    u8 color_idx;                         // 0xa
    u8 xb;                                // 0xb
    int is_bike;                          // 0xc, PlayerData.is_bike at spawn; mount and respawn refresh it from
                                          //      the ridden machine, -1 with none. Kirby's bike pose follows it
    MachineKind machine_kind : 8;         // 0x10, class-relative PlayerData.machine_kind at spawn; never refreshed
    u8 team;                              // 0x11, PlayerData+0x90 at spawn
    u8 x12[2];                            // 0x12
    JOBJDesc *jobjdesc;                   // 0x14, body model joint, from rdDataKirby.model
    rdDataKirby *rdDataKirby;             // 0x18
    RiderStatus status;                   // 0x1c
    int common_state_num;                 // 0x20, RDSTATE_COMMON_NUM
    int state_frame;                      // 0x24
    RiderMotionStatus mstatus;            // 0x28, -1 = none
    RiderStateDesc *common_state_table;   // 0x2c, statuses below common_state_num
    RiderStateDesc *state_table;          // 0x30, the kind's own statuses, indexed status - common_state_num
    int x34;                              // 0x34
    int x38;                              // 0x38
    int x3c;                              // 0x3c
    int x40;                              // 0x40
    int x44;                              // 0x44
    int x48;                              // 0x48
    int x4c;                              // 0x4c
    int x50;                              // 0x50
    int x54;                              // 0x54
    int x58;                              // 0x58
    // 0x5c, the rider's three color-overlay slots and the render state they resolve into.
    // Slot 0 (0x5c) is the body tint every Rider_ApplyColAnim request lands in; slot 1 (0x108)
    // is the additive glow aura driven from copy-ability state code; slot 2 (0x1b4) is unused
    // by the rider code. Index 2 = intangibility flash, index 3 = invincibility flash.
    ColAnimState col_anim;                // 0x5c
    int x294;                             // 0x294
    int x298;                             // 0x298
    int x29c;                             // 0x29c
    int x2a0;                             // 0x2a0
    int x2a4;                             // 0x2a4
    int x2a8;                             // 0x2a8
    int x2ac;                             // 0x2ac
    void *model_parts;                    // 0x2b0, 0x10-byte entries led by a JOBJ*. Entry 0 is the body root,
                                          //        baked every frame by Rider_ApplyModelMatrix; entry 18 (+0x120)
                                          //        is the copy-ability hat (NULL with no ability), the weapon
                                          //        spawners' throw bone.
    int x2b4;                             // 0x2b4
    int x2b8;                             // 0x2b8
    int x2bc;                             // 0x2bc
    TOBJ **tobj_lookup_arr;               // 0x2c0, every TObj of the body's DObj/MObj chains, flattened (filled
                                          //        by 0x801968d0). The recolor functions seek each entry's aobj to
                                          //        pick a baked texture variant; body colors are texture swaps.
    float rider_scale;                    // 0x2c4, PlayerData.rider_scale at spawn
    float x2c8;                           // 0x2c8, rider_scale copied by Rider_InitWalk; Trigger_Init's scale
    int is_airborne;                      // 0x2cc, off the machine: the ground probe missed
    int x2d0;                             // 0x2d0
    int x2d4;                             // 0x2d4
    int x2d8;                             // 0x2d8
    Vec3 self_vel;                        // 0x2dc
    int x2e8;                             // 0x2e8
    int x2ec;                             // 0x2ec
    int x2f0;                             // 0x2f0
    int x2f4;                             // 0x2f4
    int x2f8;                             // 0x2f8
    int x2fc;                             // 0x2fc
    Vec3 pos;                             // 0x300
    int x30c;                             // 0x30c
    int x310;                             // 0x310
    int x314;                             // 0x314
    Vec3 hand_bone_pos;                   // 0x318, the held bomb's position and the Fire/Needle/Ice aura spawn point
    Vec3 forward;                         // 0x324, forward movement vector
    Vec3 up;                              // 0x330, up vector
    Vec3 x33c;                            // 0x33c
    float model_scale;                    // 0x348, Kirby model scale
    int x34c;                             // 0x34c
    int x350;                             // 0x350
    int x354;                             // 0x354
    int x358;                             // 0x358
    int x35c;                             // 0x35c
    int x360;                             // 0x360
    int x364;                             // 0x364
    int x368;                             // 0x368
    int x36c;                             // 0x36c
    int x370;                             // 0x370
    int x374;                             // 0x374
    int x378;                             // 0x378
    int x37c;                             // 0x37c
    int x380;                             // 0x380
    int x384;                             // 0x384
    int x388;                             // 0x388
    void *jump_param;                     // 0x38c
    HurtData *hurt_data;                  // 0x390
    struct
    {
        Vec2 lstick;        // 0x394
        Vec2 lstick_prev;   // 0x39c
        Vec2 x3a4;          // 0x3a4
        Vec2 rstick;        // 0x3ac
        Vec2 rstick_prev;   // 0x3b4
        Vec2 x3bc;          // 0x3bc
        float trigger;      // 0x3c4
        float trigger_prev; // 0x3c8
        float x3cc;         // 0x3cc
        float x3d0;         // 0x3d0
        int x3d4;           // 0x3d4
        int held;           // 0x3d8, effective buttons this frame. For CPU riders set from Rider_GetCPUButtons; replays from 3DReplay_GetInputs.
        int x3dc;           // 0x3dc
        int x3e0;           // 0x3e0
        int down;           // 0x3e4
        int x3e8;           // 0x3e8
        s8 stickX;          // 0x3ec, effective stick X (byte). For CPU riders set from Rider_GetCPUStickX; also used by replays.
        s8 stickY;          // 0x3ed, effective stick Y (byte). For CPU riders set from Rider_GetCPUStickY; also used by replays.
    } input;
    GOBJ *ability_gobj;        // 0x3f0, the copy ability's weapon GObj (Fire's aura), destroyed when it is lost
    GOBJ *machine_gobj;        // 0x3f4
    GOBJ *x3f8;                // 0x3f8
    int respawn_machine_id;    // 0x3fc, MachineData.exist_num cached at the last respawn; a different id on boarding is a machine change
    int x400;                  // 0x400
    int x404;                  // 0x404
    int x408;                  // 0x408
    int x40c;                  // 0x40c
    int x410;                  // 0x410
    int x414;                  // 0x414
    int x418;                  // 0x418
    int x41c;                  // 0x41c
    int x420;                  // 0x420
    int x424;                  // 0x424
    u8 x428;                   // 0x428
    u8 x429;                   // 0x429
    struct                     // 0x42a
    {                          //
        u8 x0;                 //
        u8 cur_mat_index;      // material index currently being used for this model part (changes if wing kirby or fire)
        u8 original_mat_index; // material index that describes the original color
    } model_part[3];           // probably more of these
    u8 x433;                   // 0x433
    int x434;                  // 0x434
    int x438;                  // 0x438
    int efgroup;               // 0x43c, EfGroup bucket most of this rider's effects spawn into
    int efgroup2;              // 0x440, second EfGroup bucket
    int x444;                  // 0x444
    int x448;                  // 0x448
    int x44c;                  // 0x44c
    CamInterest *x450;       // 0x450
    CopyKind copy_kind;        // 0x454
    int queued_ability_kind;   // 0x458
    PowerUpKind powerup_kind;  // 0x45c
    PowerUpKind queued_powerup_kind; // 0x460
    int x464;                  // 0x464
    int x468;                  // 0x468
    int x46c;                  // 0x46c
    int x470;                  // 0x470
    int track_spline_id;       // 0x474, course path/spline id (-1 = none); the CPU brain samples a
                               //        look-ahead point along it for navigation
    int x478;                  // 0x478
    float track_arc_pos;       // 0x47c, arc-length position along track_spline_id; the CPU brain offsets
                               //        from this for its steering look-ahead
    int x480;                  // 0x480
    int x484;                  // 0x484
    int x488;                  // 0x488
    int x48c;                  // 0x48c
    int x490;                  // 0x490
    int x494;                  // 0x494
    int x498;                  // 0x498
    int x49c;                  // 0x49c
    int x4a0;                  // 0x4a0
    int x4a4;                  // 0x4a4
    int x4a8;                  // 0x4a8
    int x4ac;                  // 0x4ac
    int x4b0;                  // 0x4b0
    int x4b4;                  // 0x4b4
    int x4b8;                  // 0x4b8
    int x4bc;                  // 0x4bc
    int x4c0;                  // 0x4c0
    void *homing_trackers;     // 0x4c4, list of weapon homing trackers locked onto this rider
    int x4c8;                  // 0x4c8
    AudioEmitter audio_emitter;// 0x4cc
    int audio_track;           // 0x4d0
    int x4d4;                  // 0x4d4
    int x4d8;                  // 0x4d8
    int x4dc;                  // 0x4dc
    int x4e0;                  // 0x4e0
    int x4e4;                  // 0x4e4
    int x4e8;                  // 0x4e8
    int x4ec;                  // 0x4ec
    float oob_clearance;       // 0x4f0 - calcDistanceFromOOB(&pos), refreshed by Rider_UpdateOOBDistance
    int x4f4;                  // 0x4f4
    int x4f8;                  // 0x4f8
    int x4fc;                  // 0x4fc
    int x500;                  // 0x500
    int x504;                  // 0x504
    int x508;                  // 0x508
    int x50c;                  // 0x50c
    int x510;                  // 0x510
    int x514;                  // 0x514
    int x518;                  // 0x518
    int x51c;                  // 0x51c
    int x520;                  // 0x520
    int x524;                  // 0x524
    int x528;                  // 0x528
    int x52c;                  // 0x52c
    int x530;                  // 0x530
    int x534;                  // 0x534
    int x538;                  // 0x538
    int x53c;                  // 0x53c
    int x540;                  // 0x540
    int x544;                  // 0x544
    int x548;                  // 0x548
    int x54c;                  // 0x54c
    int x550;                  // 0x550
    int x554;                  // 0x554
    int x558;                  // 0x558
    int x55c;                  // 0x55c
    int x560;                  // 0x560
    int x564;                  // 0x564
    int x568;                  // 0x568
    int x56c;                  // 0x56c
    int x570;                  // 0x570
    int x574;                  // 0x574
    int x578;                  // 0x578
    int x57c;                  // 0x57c
    int x580;                  // 0x580
    int x584;                  // 0x584
    int candy_duration;        // 0x588
    int x58c;                  // 0x58c
    int patch_drop_cooldown;   // 0x590, per-spawn cooldown, reset to RiderCommonParam.patch_drop_cooldown_init
                               //        after each spawn and to 0 on a fresh Rider_DropPatches session
    int patch_drop_progress;   // 0x594, drops dispatched this session. Below RiderCommonParam.patch_drop_burst_threshold the
                               //        sub-handler spawns sequentially; at or above it switches to the burst
                               //        path. Reset to 0 on a fresh session.
    int patch_drop_count;      // 0x598, queued patch-item count for the per-frame drop consumer; written by Rider_DropPatches
    int patch_drop_mode;       // 0x59c, drop_mode from the last Rider_DropPatches call. Mode 1 negates the
                               //        velocity vector so drops land behind the rider; 0 and 2 land in front.
    int drop_piece_kind;       // 0x5a0, ItemKind Rider_TickDropAllUp picked for the current throw; written and consumed inside one call
    int allups_dropped;        // 0x5a4, all-ups extracted by Rider_DropPatches, capped at the sum of the
                               //        Hydra and Dragoon collections
    int x5a8;                  // 0x5a8
    int x5ac;                  // 0x5ac
    int x5b0;                  // 0x5b0
    int x5b4;                  // 0x5b4
    int x5b8;                  // 0x5b8
    int x5bc;                  // 0x5bc
    int x5c0;                  // 0x5c0
    int x5c4;                  // 0x5c4
    int x5c8;                  // 0x5c8
    int x5cc;                  // 0x5cc
    int x5d0;                  // 0x5d0
    int x5d4;                  // 0x5d4
    int x5d8;                  // 0x5d8
    int x5dc;                  // 0x5dc
    int x5e0;                  // 0x5e0
    int x5e4;                  // 0x5e4
    int x5e8;                  // 0x5e8
    int x5ec;                  // 0x5ec
    int x5f0;                  // 0x5f0
    int x5f4;                  // 0x5f4
    int x5f8;                  // 0x5f8
    int x5fc;                  // 0x5fc
    int x600;                  // 0x600
    int x604;                  // 0x604
    int x608;                  // 0x608
    int x60c;                  // 0x60c
    int x610;                  // 0x610
    int x614;                  // 0x614
    int x618;                  // 0x618
    int x61c;                  // 0x61c
    int x620;                  // 0x620
    int x624;                  // 0x624
    int x628;                  // 0x628
    int x62c;                  // 0x62c
    int x630;                  // 0x630
    int x634;                  // 0x634
    int x638;                  // 0x638
    int x63c;                  // 0x63c
    int cur_ground_tri;        // 0x640, triangle id under the walking rider; +0x644 holds its ground type (29 = water)
    int x644;                  // 0x644
    int x648;                  // 0x648
    int x64c;                  // 0x64c
    int x650;                  // 0x650
    int x654;                  // 0x654
    int x658;                  // 0x658
    int x65c;                  // 0x65c
    int x660;                  // 0x660
    int x664;                  // 0x664
    int x668;                  // 0x668
    GOBJ *shadow_gobj;         // 0x66c
    CollData *coll_data;       // 0x670
    TriggerData trigger;       // 0x674
    union {                    // 0x74C
        struct {
            float weight;
            float boost;
            float top_speed;
            float turn;
            float charge;
            float glide;
            float offense;
            float defense;
            float hp;
        };
        float values[9];
    } stats;
    int x770;                  // 0x770
    int x774;                  // 0x774
    CpuData *cpu;              // 0x778, CPU rider AI state / virtual pad. NULL for human riders.
    int x77c;                  // 0x77c
    int x780;                  // 0x780
    int x784;                  // 0x784
    int x788;                  // 0x788
    int x78c;                  // 0x78c
    int x790;                  // 0x790
    DmgLog dmg_log;            // 0x794, attack_data from RiderStateDesc.attack_log
    struct                     //
    {                          //
        void (*anim)(GOBJ *);    // 0x7b4, RDPRI_ANIM
        void (*input)(GOBJ *);   // 0x7b8, RDPRI_INPUT, the interrupt checks
        void (*phys)(GOBJ *);    // 0x7bc, RDPRI_PHYS
        void (*envcoll)(GOBJ *); // 0x7c0, RDPRI_ENVCOLL
        void (*x7c4)(GOBJ *);    // 0x7c4, RDPRI_6
        void (*x7c8)(GOBJ *);    // 0x7c8, RDPRI_13
        void (*x7cc)(GOBJ *);    // 0x7cc, RDPRI_0
        void (*x7d0)(GOBJ *);    // 0x7d0, RDPRI_7; the cracker launcher decides when to shoot here
        void (*x7d4)(GOBJ *);  // 0x7d4
        void (*x7d8)(GOBJ *);  // 0x7d8
        void (*x7dc)(GOBJ *);  // 0x7dc
        void (*x7e0)(GOBJ *);  // 0x7e0
        void (*x7e4)(GOBJ *);  // 0x7e4
        void (*x7e8)(GOBJ *);  // 0x7e8
        void (*x7ec)(GOBJ *);  // 0x7ec
        void (*x7f0)(GOBJ *);  // 0x7f0
        void (*x7f4)(GOBJ *);  // 0x7f4
    } cb;
    // Copy-ability teardowns, run by Rider_AbilityRemoveModel. A status installs
    // cb_status_remove (the Fire/Sword/Bomb GET states, the inhale states) and every
    // RiderStateChange clears it; cb_ability_remove persists (Tire, Bird).
    void (*cb_status_remove)(RiderData *);   // 0x7f8
    void (*cb_ability_remove)(RiderData *);  // 0x7fc
    int x800;                           // 0x800
    int x804;                           // 0x804
    // Four motion-script variables, written by the opcode handler at 0x8019b98c.
    // x808 is what RiderState_LegendaryAssemblyAnimThink polls for the machine swap.
    int x808;                           // 0x808
    int x80c;                           // 0x80c
    int x810;                           // 0x810
    int x814;                           // 0x814
    int x818;                           // 0x818
    int x81c;                           // 0x81c
    u8 x820;                            // 0x820, bit 0x04 = attack/charge input active; read by Rider_CanStartInhale
    u8 x821;                            // 0x821
    u8 x822;                            // 0x822
    u8 x823;                            // 0x823
    u8 x824_80 : 1;                     // 0x824, 0x80, set by ice damage
    u8 x824_40 : 1;                     // 0x824, 0x40, set by spin damage
    u8 is_dismounting : 1;              // 0x824, 0x20, set on dismount and KO eject, cleared by every RiderStateChange
    u8 x824_10 : 1;                     // 0x824, 0x10
    u8 x824_08 : 1;                     // 0x824, 0x08
    u8 x824_04 : 1;                     // 0x824, 0x04
    u8 x824_02 : 1;                     // 0x824, 0x02
    u8 x824_01 : 1;                     // 0x824, 0x01
    u8 x825_80 : 1;                     // 0x825, 0x80
    u8 x825_40 : 1;                     // 0x825, 0x40, set by Rider_RoundStart
    u8 x825_20 : 1;                     // 0x825, 0x20
    u8 x825_10 : 1;                     // 0x825, 0x10
    u8 x825_0f : 4;                     // 0x825, 0x0f
    u8 x826_80 : 1;                     // 0x826
    u8 x826_40 : 1;                     // 0x826
    u8 x826_20 : 1;                     // 0x826
    u8 x826_10 : 1;                     // 0x826
    u8 x826_08 : 1;                     // 0x826
    u8 x826_04 : 1;                     // 0x826
    u8 x826_02 : 1;                     // 0x826
    u8 is_walk_after_dead : 1;          // 0x826
    u8 x827;                            // 0x827
    int x828;                           // 0x828
    int x82c;                           // 0x82c
    int x830;                           // 0x830
    int x834;                           // 0x834
    int x838;                           // 0x838
    int x83c;                           // 0x83c
    int x840;                           // 0x840
    int x844;                           // 0x844
    int x848;                           // 0x848
    int x84c;                           // 0x84c
    int x850;                           // 0x850
    int x854;                           // 0x854
    int x858;                           // 0x858
    int x85c;                           // 0x85c
    int x860;                           // 0x860
    int x864;                           // 0x864
    int x868;                           // 0x868
    int x86c;                           // 0x86c
    int x870;                           // 0x870
    int x874;                           // 0x874
    int x878;                           // 0x878
    int x87c;                           // 0x87c
    int x880;                           // 0x880
    int x884;                           // 0x884
    int x888;                           // 0x888
    int x88c;                           // 0x88c
    int x890;                           // 0x890
    int x894;                           // 0x894
    int x898;                           // 0x898
    int x89c;                           // 0x89c
    int x8a0;                           // 0x8a0
    int x8a4;                           // 0x8a4
    int x8a8;                           // 0x8a8
    int x8ac;                           // 0x8ac
    int x8b0;                           // 0x8b0
    int x8b4;                           // 0x8b4
    int x8b8;                           // 0x8b8
    int x8bc;                           // 0x8bc
    int x8c0;                           // 0x8c0
    int x8c4;                           // 0x8c4
    int x8c8;                           // 0x8c8
    int x8cc;                           // 0x8cc
    int x8d0;                           // 0x8d0
    int x8d4;                           // 0x8d4
    int x8d8;                           // 0x8d8
    int x8dc;                           // 0x8dc
    int x8e0;                           // 0x8e0
    int x8e4;                           // 0x8e4
    int x8e8;                           // 0x8e8
    int x8ec;                           // 0x8ec
    int x8f0;                           // 0x8f0
    int x8f4;                           // 0x8f4
    int x8f8;                           // 0x8f8
    void *copy_wheel_jobj;              // 0x8fc, JOBJ for the copy chance wheel 3D model
    int x900;                           // 0x900
    void *copy_wheel_alloc;             // 0x904, heap allocation associated with copy wheel model
    int x908;                           // 0x908
    int x90c;                           // 0x90c
    int x910;                           // 0x910
    int x914;                           // 0x914
    int x918;                           // 0x918
    int copy_timer;                     // 0x91c, ability countdown; expires at 0
    int x920;                           // 0x920, "about to expire" threshold (warning blink fires when copy_timer drops below it)
    int x924;                           // 0x924
    int x928;                           // 0x928
    // Per-frame ability tick (the copy_kind's abilityTimer_* fn), called by
    // abilityTimerBranchToAbilityCountdown (0x801a5f68) in the ability status.
    // Decrements copy_timer and runs the drop at 0. Bomb installs none.
    void (*cb_ability_tick)(RiderData *); // 0x92c
    void (*cb_copy_input)(RiderData *); // 0x930
    int x934;                           // 0x934
    int x938;                           // 0x938
    union                               // 0x93c
    {
        CopyKind copy_wheel_result;     // CopyKind the copy wheel selected
        s32 inhale_timer;               // reused during the inhale statuses; the
                                        // gesture ends when it counts down to 0
        GOBJ *saved_plink_neighbour;    // reused during a legendary assembly
    };
    int x940;                           // 0x940, saved gx_link neighbour GOBJ during a legendary assembly
    int respawn_is_bike;                // 0x944, staged is_bike for Rider_RespawnFullRecreate; status scratch
    int respawn_class_slot;             // 0x948, staged class slot for it; the inhale statuses reuse the pair
    int x94c;                           // 0x94c
    int x950;                           // 0x950
    int x954;                           // 0x954
    int x958;                           // 0x958
    int x95c;                           // 0x95c
    int x960;                           // 0x960
    int x964;                           // 0x964
    int x968;                           // 0x968
    int x96c;                           // 0x96c
    int x970;                           // 0x970
    int x974;                           // 0x974
    int x978;                           // 0x978
    int x97c;                           // 0x97c
    int x980;                           // 0x980
    int x984;                           // 0x984
    int x988;                           // 0x988
    int x98c;                           // 0x98c
    int x990;                           // 0x990
    int x994;                           // 0x994
    int x998;                           // 0x998
    int copy_wheel_index;               // 0x99c, current index into copy_wheel_ability_list (-1 when inactive)
    int x9a0;                           // 0x9a0
    int x9a4;                           // 0x9a4
    int x9a8;                           // 0x9a8
    int x9ac;                           // 0x9ac
    int *copy_wheel_ability_list;       // 0x9b0, pointer to array of CopyKind values the wheel cycles through
    int x9b4;                           // 0x9b4
    int x9b8;                           // 0x9b8
    int x9bc;                           // 0x9bc
    int x9c0;                           // 0x9c0
    int x9c4;                           // 0x9c4
    int jumps_used;                     // 0x9c8
    u8 is_fullhop : 1;                  // 0x9cc 0x80
    u8 x9cd;                            // 0x9cd
    u8 x9ce;                            // 0x9ce
    u8 x9cf;                            // 0x9cf
    int x9d0;                           // 0x9d0
    int x9d4;                           // 0x9d4
    int x9d8;                           // 0x9d8, per-status scratch: Jump effect timer, Dash speed tier
    int x9dc;                           // 0x9dc
    int x9e0;                           // 0x9e0
    int x9e4;                           // 0x9e4
    int x9e8;                           // 0x9e8
    int x9ec;                           // 0x9ec
    struct
    {
        int is_bike;                    // 0x9f0, Sword: auto-attack cooldown, reloaded from jump_param+0x1b8
        MachineKind kind;               // 0x9f4, Sword: bit 0x80 blocks the auto-attack scan during a swing
    } machine_saved;                    // per-ability scratch; Wheel and Wing save the ridden machine here
    int x9f8;                           // 0x9f8
    int x9fc;                           // 0x9fc
    int xa00;                           // 0xa00
    int xa04;                           // 0xa04
    int xa08;                           // 0xa08
    int xa0c;                           // 0xa0c
    int xa10;                           // 0xa10
    int xa14;                           // 0xa14
    int xa18;                           // 0xa18
    int xa1c;                           // 0xa1c
    int xa20;                           // 0xa20
    int xa24;                           // 0xa24
    int xa28;                           // 0xa28
    int xa2c;                           // 0xa2c
    int xa30;                           // 0xa30
    int xa34;                           // 0xa34
    int xa38;                           // 0xa38, quick-spin scratch, cleared on RiderState_QuickSpinEnter
    int xa3c;                           // 0xa3c
    union                               // 0xa40, walk flags off the machine, quick-spin accumulators on it
    {
        struct
        {
            u8 is_jump : 1;             // 0xa40, 0x80
            u8 is_grounded : 1;         // 0xa40, 0x40
            u8 is_fall : 1;             // 0xa40, 0x20, walked off a ledge
            u8 is_fly : 1;              // 0xa40, 0x10
            u8 is_fly2 : 1;             // 0xa40, 0x08, air jump (Jump2 status), checked by CheckIfKirbyJumps
            u8 is_swim : 1;             // 0xa40, 0x04
        };
        struct
        {
            u8 quickspin_cw;            // 0xa40, CW frames (Rider_UpdateQuickSpinTimers)
            u8 quickspin_ccw;           // 0xa41, CCW frames
        };
        int xa40;
    };
    int xa44;                           // 0xa44
    int xa48;                           // 0xa48
    int xa4c;                           // 0xa4c
    int xa50;                           // 0xa50
    int xa54;                           // 0xa54
    int xa58;                           // 0xa58
    int xa5c;                           // 0xa5c
    int xa60;                           // 0xa60
    int xa64;                           // 0xa64
    int xa68;                           // 0xa68
    int xa6c;                           // 0xa6c
    int xa70;                           // 0xa70
    int xa74;                           // 0xa74
    int xa78;                           // 0xa78
    void (*WaterEnter)(RiderData *);    // 0xa7c, walking states: ground type turned to water
    void (*WaterExit)(RiderData *);     // 0xa80, swim states: ground type left water
    int xa84;                           // 0xa84
    int xa88;                           // 0xa88
    int xa8c;                           // 0xa8c
    int xa90;                           // 0xa90
    int xa94;                           // 0xa94
    int xa98;                           // 0xa98
    int xa9c;                           // 0xa9c
} RiderData;


// Per-RiderKind behavior. A kind is its own status table plus these hooks; Dedede's
// and Meta Knight's fill the same shape Kirby's does. Fields past 0x3c are unmapped.
typedef struct RiderKindDesc
{
    RiderStateDesc *state_table;                        // 0x00, -> RiderData.state_table
    void (*on_load)(int a, int b);                      // 0x04, after Rider_LoadFile loads the archive
    void (*on_preload)(int a, int b);                   // 0x08, after Rider_PreloadKindArchive queues it
    void (*on_create)(RiderData *rd);                   // 0x0c, Rider_Create, before the hurt data
    void (*load_model)(RiderData *rd);                  // 0x10, Rider_Create, model part visibility
    void *x14[2];                                       // 0x14
    void (*gx)(RiderData *rd, int pass);                // 0x1c, Rider_GX, after the body draws
    void *x20[2];                                       // 0x20
    void (*on_state_change)(RiderData *rd);             // 0x28, RiderStateChange, before the new status installs
    void *x2c[4];                                       // 0x2c
    int (*resolve_mstatus)(RiderData *rd, int mstatus); // 0x3c, RiderStateChange's mstatus remap
    void *x40[4];                                       // 0x40
    void (*round_start)(RiderData *rd);                 // 0x50, Rider_KindRoundStart, at the round's GO
    void *x54[5];                                       // 0x54
} RiderKindDesc; // 0x68

static RiderKindDesc **stc_rider_kind_desc = (RiderKindDesc **)0x804adbd8; // [RDKIND_NUM]
// {archive filename, rdData public} per RiderKind, back to back with stc_rider_kind_desc.
static char **stc_rider_archive_names = (char **)0x804adbc0; // [RDKIND_NUM * 2]
// Copied into RiderData.rdDataKirby by Rider_InitData, so a rider reads its rdData off
// itself from then on.
static rdDataKirby **stc_rdDataKirby = (rdDataKirby **)0x80559fa8; // [RDKIND_NUM]
static rdDataCommon **stc_rd_common_data = (rdDataCommon **)(0x805dd0e0 + 0x730);
static RiderCommonParam **stc_rider_param = (RiderCommonParam **)(0x805dd0e0 + 0x734);

// Copy ability initialization function table. 11 entries (one per CopyKind),
// each pointing to the ability's init function (e.g., ability_Fire at 0x801af474).
// Indexed by CopyKind. NULL entry means the ability is not implemented.
typedef void (*AbilityInitFunc)(RiderData *);
static AbilityInitFunc *stc_ability_init_table = (AbilityInitFunc *)0x804af4f0;

static RiderStateDesc *stc_rider_common_state_table = (RiderStateDesc *)0x804adc08; // [RDSTATE_COMMON_NUM]
// Kirby's statuses from RDSTATE_COMMON_NUM. RDSTATE_READYPUSHSTART and RDSTATE_PUSHSTART
// play mstatus 100, which arms a 1-frame, 2-damage hitbox against event actors, items,
// weapons and stage objects that credits no player.
static RiderStateDesc *stc_rider_kirby_state_table = (RiderStateDesc *)0x804ae428;

// Copy wheel ability list tables used by Rider_StartCopyWheel.
// Normal mode: 11 entries {0,1,2,...,10} (all CopyKinds).
// Melee mode: 29 entries (abilities + duplicates).
// Each table entry is a struct { int count; int *ability_list; }.
typedef struct CopyWheelTable
{
    int count;          // 0x0, number of entries in ability_list
    int *ability_list;  // 0x4, pointer to array of CopyKind values
} CopyWheelTable;
static CopyWheelTable *stc_copy_wheel_normal = (CopyWheelTable *)0x804af730; // count=11, list at 0x804af690
static CopyWheelTable *stc_copy_wheel_melee = (CopyWheelTable *)0x804af738;  // count=29, list at 0x804af6bc

// The on-foot counterpart of Machine_ActOnHitCollision, including its Rail Fire
// station case (bl Ply_PlayRailFireHitSFX at 0x80196668).
void Rider_ActOnHitCollision(RiderData *rd); // 0x8019655c
void Rider_CheckEventCollision(RiderData *rd); // 0x8019649c, off the machine only: event actors' attacks against the rider
void RiderGObj_CPUThink(GOBJ *gobj);      // 0x8018fc58, rider proc: if CPU, runs the AI update
// Allocates rd->cpu. ai_state 0 picks the profile with Rider_CPUSelectProfile.
void Rider_CPUInit(RiderData *rd, int ai_state, int difficulty); // 0x80262d6c
void Rider_UpdateCPU(RiderData *rd);      // 0x8026beec, orchestrates perceive -> decide -> process -> emit
void Rider_ProcessCPUDistance(RiderData *rd); // 0x8026bbe0, first step of Rider_UpdateCPU; zeroes the per-frame scratch
void Rider_CPUDecideState(RiderData *rd); // 0x802716e8, AI state-machine dispatch (11 states, table 0x804b7a28)
// Per-ai_state handlers: rewrite behavior_flags, pick targets, then tail-call
// Rider_CPUArbitrateManeuver.
void Rider_CPUDecideCruise(RiderData *rd);          // 0x80271790, state 1
void Rider_CPUDecideCruisePlus(RiderData *rd);      // 0x80271b24, states 2, 4 and above 10
void Rider_CPUDecideNavigate(RiderData *rd);        // 0x80271eb4, state 3
void Rider_CPUDecideRouteFollow(RiderData *rd);     // 0x802726fc, state 5
void Rider_CPUDecideRouteFollowCity(RiderData *rd); // 0x80272888, state 6
void Rider_CPUDecideCharge(RiderData *rd);          // 0x80272dd0, state 7
void Rider_CPUDecideAttack(RiderData *rd);          // 0x802735dc, state 8
void Rider_CPUDecideReposition(RiderData *rd);      // 0x80273228, state 9
void Rider_CPUDecidePatrol(RiderData *rd);          // 0x80273b48, state 10
void Rider_CPUProcessCmd(RiderData *rd);  // 0x80275cbc, plays the command stream into the virtual pad (CpuData stick/buttons)
// The maneuver chooser: a priority cascade gated by behavior/desire flags,
// committing CpuData.maneuver.
void Rider_CPUArbitrateManeuver(RiderData *rd); // 0x80274ec0
// Builds the look-ahead waypoint route into scratch 0x8055e964, returning 0 if
// the route is invalid. Does not pick the maneuver.
int  Rider_CPUBuildRoute(RiderData *rd, void *route_scratch, uint *flags_out); // 0x8026a734
void Rider_CPUUpdateNavTarget(RiderData *rd); // 0x8026b6d0, nearest-node spatial query -> assigns CpuData.target_primary (+0x38) + arc (+0x3c)
// ORs inhibitor bits into CpuData.desire_flags from the rider's status.
void Rider_CPUSeedDesire(RiderData *rd);  // 0x802762dc
// Target-selection scans, filling CpuData's target fields each frame.
void  Rider_CPURivalSelect(RiderData *rd);        // 0x80264210, scores 5 slots -> rival_ply (+0x70); shared by states 1/2/4/8/10
// Ranks the top 5 items within radius of center by Rider_CPUGetItemScore plus
// distance bands and returns the path_retry_counter'th; the caller stores it to
// item_target (+0x74) (states 3/8). A non-NULL facing adds 5 to items off its axis.
GOBJ *Rider_CPUScanItems(RiderData *rd, Vec3 *center, Vec3 *facing, float radius); // 0x80263c4c
GOBJ *Rider_CPUScanCityObjects(RiderData *rd, Vec3 *center, float radius); // 0x802638a4, top-5 ranked city-object scan -> city_object (+0x84) (state 3)
void  Rider_CPUScanRouteGoal(RiderData *rd);      // 0x80263fd0, single-best route-goal scan -> route_goal (+0x94) (states 5/6)
void  Rider_CPUSelectChargeAnchor(RiderData *rd); // 0x80263610, resolves charge anchor -> charge_anchor (+0xb0/+0xb4) (state 7)
void  Rider_CPUBlendRoutePoints(RiderData *rd, void *route_scratch); // 0x80267238, blends item/interaction/path-point positions into the route
// Perception (fill the per-frame scratch buffers).
void  Rider_CPUCollectHazards(RiderData *rd);     // 0x80269928, hazard/threat list -> scratch 0x8055e698 (behavior bit 0x02 adds a 5th pass)
void  Rider_CPUCollectRiderHazards(RiderData *rd, CpuHazardList *list, Vec3 *pos, Vec3 *arg3, float f1); // 0x80268234, hazard pass 1: rider bodies + hurt-volumes
int   Rider_CPUForwardLookahead(RiderData *rd, CpuForwardList *list); // 0x80269f10, forward-collision list (behavior bit 0x80 gates it)
void  Rider_CPUForwardLookaheadSetup(RiderData *rd); // 0x8026a498, wrapper: builds the basis then calls Rider_CPUForwardLookahead
typedef int (*CpuRouteStep)(int node);
// Advances along the spline graph from start into out, both {node_id, along, side}.
// step is Spline_GetForward or Spline_GetBackward.
int   Rider_CPUWalkRoute(RiderData *rd, void *start, void *out, CpuRouteStep step, float f1, float f2); // 0x80264924
// Steering / command emission.
void  Rider_CPUEmitSteer(RiderData *rd, Vec3 *desired_dir);      // 0x8026d6a0, projects + avoidance-bends a heading, emits steering opcodes
void  Rider_CPUEmitSteerStick(RiderData *rd, Vec3 *desired_dir); // 0x8026c4ec, yaw error -> opcode 190 nudge / 192 ramp (stick_x), 129 (stick_y)
void  Rider_CPUResolveAvoidVector(RiderData *rd, Vec3 *dir, float f1); // 0x8026d1fc, rotates a heading off the nearest imminent hazard
void  Rider_CPUEmitAbilityAction(RiderData *rd);  // 0x80273d1c, post-maneuver copy-ability press emitter
void  Rider_CPUTerminateCmdStream(RiderData *rd); // 0x80276228, caps the per-maneuver command stream (opcode 0x7f), arms the VM
int   Rider_CPUEmitChargeStutter(RiderData *rd);  // 0x8026da40, velocity-stuck charge-pump (press -> hold-20 -> hold-40); returns 1 if it emitted
void  Rider_CPUTrackStuckProgress(RiderData *rd); // 0x8026ccec, ticks the stuck counter (+0x5c) vs the machine's max-turn tolerance
// Difficulty / envelope getters.
float Rider_CPUDifficultyScale(RiderData *rd);    // 0x80276f00, difficulty_level / 8, clamped to [0, 1]
// ItemKind desirability from the table at 0x804b862c; below 1 means ignore. Food
// is multiplied by 20 or 10 while the machine is low on HP.
int   Rider_CPUGetItemScore(RiderData *rd, int kind); // 0x802768a8
int   Rider_CPUGetAbilityPressHold(CpuData *cpu); // 0x802765d4, per-difficulty ability press-hold frames (table 0x804b7f30)
void  Rider_CPUGetSteerEnvelope(CpuData *cpu, int *step_out, int *cap_out); // 0x80276650, per-difficulty (step,cap) steer envelope (table 0x804b7f54)
float Rider_CPUGetMachineTurnTolerance(RiderData *rd); // 0x802776c4, per-machine max-turn angle (table 0x804b8f30)
float Rider_CPUGetMachineAlignCosNear(RiderData *rd);   // 0x80277538, CpuMachineSteer.align_cos_near
float Rider_CPUGetMachineAlignCosFar(RiderData *rd);    // 0x802775bc, CpuMachineSteer.align_cos_far
float Rider_CPUGetMachineStuckAngle(RiderData *rd);     // 0x80277640, CpuMachineSteer.stuck_angle
// The rider's machine's stadium pair, indexed by MachineGObj_GetAbsoluteKind. 0 with no machine.
int Rider_CPUGetAirGliderMachineParam(RiderData *rd, float *pitch, float *min_len); // 0x80277024
int Rider_CPUGetHighJumpMachineParam(RiderData *rd, float *pitch, float *min_len);  // 0x80277094
// CpuMachineCaps readers. Each splits MachineGObj_GetAbsoluteKind back into a class slot.
int MachineGObj_CPUGetSwapScore(GOBJ *machine_gobj);        // 0x8027699c, -1 with no row
int MachineGObj_CPUCanSwap(GOBJ *machine_gobj);             // 0x80276a24, 0 for a NULL machine
int MachineGObj_CPUCanBrake(GOBJ *machine_gobj);            // 0x80276acc
int MachineGObj_CPUCanChargeHold(GOBJ *machine_gobj);       // 0x80276b64
float MachineGObj_CPUGetChargeRelease(GOBJ *machine_gobj);  // 0x80276bfc
int MachineGObj_CPUCanRamCharge(GOBJ *machine_gobj);        // 0x80276c84
int MachineGObj_CPUSkipsPassageBranch(GOBJ *machine_gobj);  // 0x80276d1c
// Whether the City Trial spawn slot numbered by the machine's absolute kind is filled.
int MachineGObj_CPUIsKindSlotFilled(GOBJ *machine_gobj);    // 0x80276db4
// Bulk, Hydra, Rocket and Formula only: the charge-hold gate. Returns 0 for the rest.
int MachineGObj_CPUGetChargeHoldGate(GOBJ *machine_gobj, float *a, float *b); // 0x80276de4
// Bulk and Hydra only: the charge level a CPU releases at. Returns 0 for the rest.
int MachineGObj_CPUGetChargeReleaseOverride(GOBJ *machine_gobj, float *level); // 0x80276ea0
int  Rider_GetCPUButtons(RiderData *rd);  // 0x80275cb0
int  Rider_GetCPUStickX(RiderData *rd);   // 0x80275c90, stick_x truncated to s8
int  Rider_GetCPUStickY(RiderData *rd);   // 0x80275ca0, stick_y truncated to s8
void RiderGObj_InputThink(GOBJ *gobj);    // 0x8018ee28, rider proc: selects effective input (human / CPU / replay)

void RiderState_RespawnEnter(RiderData *); // 0x801a1d70
// Tears the rider's current machine down and recreates rider plus machine on
// (is_bike, class_index) in place. Called by the legendary assembly and by the copy
// ability's star removal; the trailing arguments are the literals both call sites
// pass. carry_hp_max and carry_xc3b_10 copy those fields off the old machine into the
// spawn desc, set_ply_machine records the new machine on the player, and desc_x80
// lands in MachineSpawnDesc.x80.
void Rider_RespawnFullRecreate(RiderData *rd, int is_bike, u8 class_index, int carry_hp_max, int a5, int set_ply_machine, u8 desc_x80, u32 carry_xc3b_10); // 0x80193900
int Rider_GiveAbility(RiderData *, CopyKind); // 0x801a81a4
int Rider_CheckUnableAbility(RiderData *); // 0x80191798, 1 while grants must be queued (x823 bit 0x01)
// With a copy ability or power-up held, calls cb_status_remove then cb_ability_remove,
// skipping NULL ones. Rider_GiveAbility runs it before every grant. The teardowns end
// in Rider_RevertCopyAbility (0x801a7d70), which removes the hat, clears cb_ability_remove
// and runs Rider_TeardownCopyAbility (copy_kind = -1, poof effect and SFX). It does
// not play the spit-out animation; RiderState_LoseAbilityEnter does.
void Rider_AbilityRemoveModel(RiderData *); // 0x80191554
// Cancels queued copy-ability and power-up grants, freeing their pending objects
// and resetting queued_ability_kind / queued_powerup_kind to -1.
void Rider_AbilityClearQueued(RiderData *); // 0x801915c4
// Grants queued_ability_kind / queued_powerup_kind if one is pending, otherwise runs
// the rider's interrupt fallback chain and settles on RiderState_StarWaitEnter. The engine's generic
// "resolve or return to neutral" step - the tail of Rider_StartCopyWheel and the exit
// of the inhale START state.
void Rider_ResolveQueuedAbility(RiderData *); // 0x801a8454

// Legendary assembly, the rider's half. Kirby only. Enter relinks the rider GOBJ
// from GAMEPLINK_RIDER to p_link 32 and gx_link 6 to 26, saving both neighbours in
// +0x93c / +0x940, and enters RDSTATE_LEGENDARYASSEMBLY with mstatus 0x220 +
// machine_index. p_link 32 is outside PAUSEKIND_EXPLODE's freeze mask, so the rider
// keeps animating while the world is frozen. The machine swap is driven by the
// motion-script variable at +0x808.
void Ply_EnterLegendaryAssembly(int ply, int machine_index);    // 0x8022d6b4, also clears that set's piece bits
void Ply_ExitLegendaryAssembly(int ply);                        // 0x8022d71c
void RiderGObj_EnterLegendaryAssembly(GOBJ *rider_gobj, int machine_index); // 0x8019248c
void RiderGObj_ExitLegendaryAssembly(GOBJ *rider_gobj);         // 0x801924f8
void RiderState_LegendaryAssemblyEnter(RiderData *rd, int machine_index); // 0x801bda34
void RiderState_LegendaryAssemblyAnimThink(RiderData *rd);      // 0x801bdb2c
void RiderState_LegendaryAssemblyExit(RiderData *rd);           // 0x801bdbac
// Motion-script opcode: writes the command word's low 24 bits into RiderData
// +0x808, +0x80c, +0x810 or +0x814, picked by the command byte's low 2 bits.
void RiderScript_SetVariable(RiderData *rd, void *script);      // 0x8019b98c
// Mounts the rider on machine_gobj (RDSTATE_GETONSTAR) and counts a get-on when it
// is not the machine the rider respawned on.
void RiderState_GetOnStarEnter(GOBJ *machine_gobj, RiderData *rd); // 0x801ba054
// Neutral riding status (RDSTATE_RUN). Last resort of the interrupt chain.
void RiderState_StarWaitEnter(RiderData *); // 0x801ab1a0
// The round's GO for one rider, from Ply_RoundStartAll: nothing on foot
// (RDSTATE_FREEMOVE), otherwise Rider_KindRoundStart, then sets x825_40.
void Rider_RoundStart(GOBJ *rider_gobj); // 0x80191c38
void Rider_KindRoundStart(RiderData *rd); // 0x8019f850, RiderKindDesc.round_start if set
// Each kind's RiderKindDesc.round_start: RDSTATE_READY enters RiderState_StarWaitEnter and
// each READYPUSH state its own exit. Dedede's and Meta Knight's first call one more kind
// function.
void Rider_Kirby_RoundStart(RiderData *rd);      // 0x801ab0f0
void Rider_Dedede_RoundStart(RiderData *rd);     // 0x801be86c
void Rider_MetaKnight_RoundStart(RiderData *rd); // 0x801c1f90
// Ends a machine charge and spends it on a boost (RDSTATE_PUSHEND), from the
// A-release interrupt check and the full-charge hold status's think.
// MachineData.charge_value still holds the charge on entry. Kirby only.
void RiderState_StarChargeReleaseEnter(RiderData *); // 0x801abc64
// Its two callers. The interrupt check enters it and returns 1 unless A is still held;
// the full-charge hold status's think enters it once the full-charge window ends.
int Rider_IASACheck_ChargeRelease(RiderData *rd); // 0x801abc2c
void AS_StarChargeFullThink(RiderData *rd);       // 0x801abea0
void RiderState_LoseAbilityEnter(RiderData *); // 0x801b0adc
void Rider_GiveIntangibility(RiderData *, int time); // 0x80195f68
void Rider_GiveInvincibility(RiderData *, int time); // 0x80195f28
int RiderGObj_GetPly(GOBJ *gobj); // 0x8019203c
HurtData *RiderGObj_GetHurtData(GOBJ *rider_gobj); // 0x80192788
u8 RiderGObj_GetTeam(GOBJ *gobj); // 0x80191f68, RiderData.team
void RiderGObj_AddHomingTracker(GOBJ *rider_gobj, void *tracker, void *cb); // 0x801922b0, onto homing_trackers
void RiderGObj_RemoveHomingTracker(GOBJ *rider_gobj, void *tracker);        // 0x801922e4
// The rider counterpart of Weapon_AssignStateFlags, on the attack block at rd+0x794.
void Rider_AssignAttackLog(RiderData *rd, int attack_log); // 0x801a2048
int Rider_IsOnMachine(RiderData *); // 0x80191680
int Rider_IsMachineAirborne(RiderData *);   // 0x80194120, MachineGObj_IsAirborne of the ridden machine; no NULL check
int Rider_IsMachineDead(RiderData *);       // 0x801943e4, can only be called between the RDPRI_HITCOLL and RDPRI_DMGAPPLY priority.
// Enqueues a stat-patch drop event; Rider_TickDropPatches drains it per frame.
// drop_mode 0 = forward, small fixed count, probabilistic all-up; 1 = behind,
// count scaled by stats, no all-ups; 2 = forward, count scaled by stats, all
// remaining all-ups.
void Rider_DropPatches(RiderData *, float stat_array[9], int drop_mode); // 0x8019d330
// Rider_TickDropPatches' all-up branch: throws one legendary piece per cooldown while
// allups_dropped is positive and returns the new patch_drop_count.
int Rider_TickDropAllUp(RiderData *rd); // 0x8019d55c
// A mode-0 Rider_DropPatches when a single hit's damage exceeds
// RiderCommonParam.patch_drop_damage_min (8.0). pos and dir are ignored.
// Run for a hit on foot and, through RiderGObj_DropPatchesOnDamage, for one on a
// ridden machine in the city.
void Rider_DropPatchesOnDamage(RiderData *rd, Vec3 *pos, Vec3 *dir, float stat_array[9], int damage); // 0x8019cdfc
void RiderGObj_DropPatchesOnDamage(GOBJ *rider_gobj, Vec3 *pos, Vec3 *dir, float stat_array[9], int damage); // 0x80192980
int RiderGObj_CheckCanReceiveAbility(GOBJ *gobj); // 0x8019262c, returns 1 if rider can receive a copy ability
int RiderGObj_CheckAndGiveAbility(GOBJ *gobj, int kind); // 0x80192650, checks rider is Kirby, then gives copy ability, returns 1 on success
// Appends to PlayerStats.copy_history, checks the three ability sequences and
// bumps copy_obtain_count. Runs for every grant, whatever the source.
void Ply_RecordCopyAbility(int ply, int copy_kind); // 0x8022ee00
// Sets the PlayerStats.copy_chance_mask bit (15-copy_kind). Only the copy-wheel
// paths call it, so the bit means "the wheel gave it".
void Ply_MarkCopyAbilityObtained(int ply, int copy_kind); // 0x8022f150

int randomAbility_giveAbility(RiderData *, int kind); // 0x801a61d4, gives copy ability from copy chance wheel (no unable/queue check)
// Initializes the copy wheel at a starting ability, setting copy_wheel_ability_list and index.
void Rider_StartCopyWheel(RiderData *rd, int copy_kind); // 0x801ae550
// Picks a random starting ability and starts the wheel; returns 1 if it started.
int Rider_StartRandomCopyWheel(RiderData *rd); // 0x801ae4ec
int RiderGObj_GiveRandomAbility(GOBJ *gobj); // 0x80191fb8, GOBJ wrapper for Rider_StartRandomCopyWheel
int RiderGObj_CheckCanReceivePowerUp(GOBJ *gobj); // 0x80192688, returns 1 if rider can receive a power-up (checks rd->x825_0f is clear)
int RiderGObj_GivePowerUp(GOBJ *gobj, PowerUpKind kind); // 0x801926ac, gives rider a power-up if rider is Kirby, returns 1 on success
int Rider_TryGivePowerUp(RiderData *rd, PowerUpKind kind); // 0x801a828c, checks unable, queues into x460 or calls Rider_GivePowerUpByKind
int Rider_GivePowerUpByKind(RiderData *rd, PowerUpKind kind); // 0x801a8304, removes current ability and initializes power-up kind (0-3), returns 1 on success

// Inhale, the no-copy-ability attack. The native path enters it only with an
// inhalable EventActor already in range. RDSTATE_DRAWSTART (mstatus 0x76) is a
// one-shot gulp that returns to neutral. RDSTATE_DRAW (0x77) is entered only through
// Rider_StartInhaleLoop; its proc re-enters it each time the body motion ends, until
// the countdown at +0x93c (aliasing copy_wheel_result) runs out while the mouth is
// empty. RDSTATE_DRAWEND (0x78) closes the mouth.
// RDSTATE_DRAWSTART with the suction effect, SFX and capture callbacks, and no gate
// or target check.
void Rider_StartInhale(RiderData *rd);        // 0x801ad2c4
// Enters or re-enters RDSTATE_DRAW, reinstalling the scan callbacks without
// respawning the effect or resetting captures.
void Rider_StartInhaleLoop(RiderData *rd);    // 0x801ad4cc
// RDSTATE_DRAWEND: close motion, close puff, back to neutral.
void Rider_EndInhale(RiderData *rd);          // 0x801adf98
// 1 once the rider's body motion has played to its end.
int  Rider_IsBodyAnimDone(RiderData *rd);     // 0x80198b00
// Gate: attack bit set, no copy ability, and fewer than 3 captures held.
int  Rider_CanStartInhale(RiderData *rd);     // 0x801a617c
// Per-frame entry probe: starts an inhale if the gate passes and an inhalable
// EventActor overlaps the mouth volume.
void Rider_TryStartInhale(RiderData *rd);     // 0x8019c5ac
// Per-frame scan of the EventActor bucket, capturing up to 3 a frame (list cap 10).
void Rider_InhaleCaptureScan(RiderData *rd);  // 0x8019c63c
// Candidate predicate: EventActor enemies only. Items and yakumono never pass.
int  EventActorGObj_IsInhalable(GOBJ *cand);  // 0x802041c8
// RDSTATE_SPIT after inhaling an enemy with no ability. Installs the anim callback,
// which fires Rider_SpawnStarBullet(rd, flag) when the motion script raises +0x818 bit 7.
void RiderState_InhaleStarSpitEnter(RiderData *rd, int flag); // 0x801aea5c
void RiderState_InhaleStarSpitAnimThink(RiderData *rd);    // 0x801aeb6c

// Quick spin, the stick-rotation spin attack. Rider_UpdateQuickSpinTimers ticks the
// CW/CCW accumulators at +0xa40 / +0xa41 and Rider_CheckQuickSpinInput (0x80191980)
// compares them with RiderCommonParam.quickspin_flick_window; a status that skips the
// tick freezes them. Tornado's own spin shares the detector but enters elsewhere.
// Per-frame tick: 0 while held in that direction, saturating at 0xfe.
void Rider_UpdateQuickSpinTimers(RiderData *rd); // 0x80191a58
// Interrupt check: excludes copy_kind PLASMA and enters the spin on a flick, returning
// 1 if it did. Installed in the grounded status but not the airborne one.
int  RiderState_QuickSpinInterrupt(RiderData *rd); // 0x801b7e80
// RDSTATE_QUICKSPINTURN (mstatus 0x6a CCW / 0x6b CW) with invincibility. f goes to
// RiderStateChange, dir is +1 CW / -1 CCW, and flag also runs Rider_MachineEnterQuickSpin.
void RiderState_QuickSpinEnter(float f, RiderData *rd, int dir, int flag); // 0x801b7ee4

// Dedede's and Meta Knight's quick-spin enters, each with a single caller. Dedede
// enters status 0x2c, Meta Knight 0x2d.
void RiderState_DededeQuickSpinEnter(RiderData *rd, int dir);     // 0x801c05f8
void RiderState_MetaKnightQuickSpinEnter(RiderData *rd, int dir); // 0x801c3f90
// Per-character quick-spin interrupt checks, installed in their grounded statuses
// but not their air control.
int  RiderState_DededeQuickSpinInterrupt(RiderData *rd);     // 0x801c05a8
int  RiderState_MetaKnightQuickSpinInterrupt(RiderData *rd); // 0x801c3f40
// Dedede's auto-attack. InitAutoAttack installs AutoAttackThink as the per-frame check
// (RiderData+0x914), which enters the hammer swing (status 52) once
// Rider_FindAutoAttackTarget sees a target; status 54 installs AutoAttackChainThink,
// whose hit enters the second swing (status 55).
void Rider_Dedede_InitAutoAttack(RiderData *rd, int keep_cooldown); // 0x801c0d28
void Rider_Dedede_AutoAttackThink(RiderData *rd);                   // 0x801c0b50
void Rider_Dedede_AutoAttackChainThink(RiderData *rd);              // 0x801c0bbc
void RiderState_DededeAttackEnter(RiderData *rd);                   // 0x801c0dd8
void RiderState_DededeAttack2Enter(RiderData *rd);                  // 0x801c123c
// Calls on_found(rd) for the first rider, machine, enemy or object overlapping detect.
// Shared by Dedede, Meta Knight and the Sword ability's auto-swing.
void Rider_FindAutoAttackTarget(RiderData *rd, void *detect, void *on_found); // 0x8019f288

// Rider_LoadFile's archive step for one kind: loads stc_rider_archive_names[kind] into
// stc_rdDataKirby[kind] if empty, then runs the kind's on_load.
void Rider_LoadKindArchive(RiderKind kind, int a, int b);    // 0x80190468
void Rider_PreloadKindArchive(RiderKind kind, int a, int b); // 0x80192bd8
// The motion entry {FigaTree, script, flags} for mstatus, out of the {count, entries}
// runs at rdData+0x0c.
void *Rider_GetMotionDesc(RiderData *rd, int mstatus); // 0x80199734
// Binds RiderData+0x34's FigaTree over model_parts and requests frame at rate.
void Rider_ApplyMotionAnim(RiderData *rd, float frame, float rate); // 0x80198934

// Rider-side entries into the machine spins, through the ridden machine at +0x3f4.
// The three quick-spin enters call the first; two rider statuses call the second.
void Rider_MachineEnterQuickSpin(RiderData *rd, int dir);              // 0x80193c20
void Rider_MachineEnterForcedSpin(RiderData *rd, int frames, int dir); // 0x80193bfc

// The airborne riding status's callback, one per rider kind (Kirby's is airControl,
// 0x801ac128). Dedede's runs the charge check then Rider_UpdateQuickSpinTimers; Meta
// Knight's runs only the charge check, so his spin accumulators freeze while airborne.
void Rider_Dedede_AirControl(RiderData *rd);     // 0x801bf534
void Rider_MetaKnight_AirControl(RiderData *rd); // 0x801c2b08

// Kirby recolor. SetMaterialColor stages a baked palette index in
// model_part[part].cur_mat_index with a dirty bit; SetMaterialColorAndUpdate also
// seeks the part's TObjs in tobj_lookup_arr to it, swapping in the baked texture.
// Rider_ApplyColAnim plays a timed overlay into col_anim.slot[0].
void RiderKirby_SetMaterialColor(RiderData *rd, int part_idx, u8 mat_index);          // 0x80198d1c
void RiderKirby_SetMaterialColorAndUpdate(RiderData *rd, int part_idx, u8 mat_index); // 0x80198d3c
u8   RiderGObj_GetColor(GOBJ *gobj);                                                  // 0x80192758, RiderData.color_idx
// anim_index selects from the global table. Priority-gated, so it returns 0 when a
// higher-priority anim holds the slot.
int  Rider_ApplyColAnim(RiderData *rd, int anim_index, int param); // 0x8019bfb4

// MachineGObj_GetWeaponBaseVelocity of the ridden machine, into *out.
void Rider_GetWeaponBaseVelocity(RiderData *rd, Vec3 *out); // 0x8019407c

void RiderGObj_GetHandBonePos(GOBJ *gobj, Vec3 *out); // 0x80191ffc, hand_bone_pos
void RiderGObj_GetForward(GOBJ *gobj, Vec3 *out);     // 0x80191ef8, forward
void RiderGObj_GetUp(GOBJ *gobj, Vec3 *out);          // 0x80191f18, up

void RiderGObj_SetCandyTimer(GOBJ *gobj, int duration); // 0x801929a4, stores candy_duration and plays SFX 47

AudioEmitter Rider_AllocAudioEmitter(void); // 0x8005dbc8

#endif