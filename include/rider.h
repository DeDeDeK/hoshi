#ifndef KAR_H_RIDER
#define KAR_H_RIDER

#include "datatypes.h"
#include "obj.h"
#include "hurt.h"
#include "collision.h"
#include "machine.h"
#include "camera.h"
#include "trigger.h"

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

typedef enum CopyKind
{
    COPYKIND_NONE = -1,
    COPYKIND_FIRE = 0,
    COPYKIND_WHEEL,
    COPYKIND_SLEEP,
    COPYKIND_SWORD,
    COPYKIND_BOMB,
    COPYKIND_PLASMA,
    COPYKIND_NEEDLE,
    COPYKIND_MIC,
    COPYKIND_FREEZE,
    COPYKIND_TORNADO,
    COPYKIND_BIRD,
    COPYKIND_NUM,
} CopyKind;

static const char *const CopyKind_Names[COPYKIND_NUM] = {
    [COPYKIND_FIRE]    = "Fire",
    [COPYKIND_WHEEL]   = "Wheel",
    [COPYKIND_SLEEP]   = "Sleep",
    [COPYKIND_SWORD]   = "Sword",
    [COPYKIND_BOMB]    = "Bomb",
    [COPYKIND_PLASMA]  = "Plasma",
    [COPYKIND_NEEDLE]  = "Needle",
    [COPYKIND_MIC]     = "Mic",
    [COPYKIND_FREEZE]  = "Freeze",
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

// CPU rider AI state (the "virtual pad"), pointed to by RiderData.cpu (+0x778).
// Allocated only for CPU riders (NULL for humans). Its leading fields are the
// synthesized controller output that Rider_InputThink reads back via
// Rider_GetCPUStickX/Y/Buttons into the rider's effective input fields (held/
// stickX/stickY). Rider_UpdateCPU fills it each frame: perceive -> decide ->
// process -> emit (command stream). Partial map.
// Two layers drive a CPU rider:
//   - ai_state (0x08)  : the AI PROFILE - chosen once at init by Rider_CPUSelectProfile
//                        from stage/city/ply (1..10; state 0 asserts) and FIXED for the
//                        match. Dispatched by Rider_CPUDecideState; its handler picks the
//                        tactical maneuver each frame. NOT a per-frame transitioning FSM.
//   - maneuver (0x10)  : TACTICAL maneuver, dispatched by Rider_ProcessCPUManeuver
//                        (0..0x15). Emits a fresh command stream into cmd_buffer,
//                        but ONLY when the command VM is idle (cmd_read_ptr==0 &&
//                        cmd_timer==0) - so one maneuver plays to completion before
//                        the next is chosen.
// The command VM (Rider_CPUProcessCmd) then plays cmd_buffer back into the pad
// fields (buttons/stick_x/stick_y). Gaps are padding.
typedef struct CpuData
{
    int buttons;           // 0x00, synthesized button mask -> RiderData.held (0x3d8)
    s8  stick_x;           // 0x04, synthesized stick X -> RiderData.stickX (0x3ec)
    u8  x05;               // 0x05
    s8  stick_y;           // 0x06, synthesized stick Y -> RiderData.stickY (0x3ed)
    u8  x07;               // 0x07
    int ai_state;          // 0x08, AI profile (1..10; 0 asserts) set once at init, dispatched by Rider_CPUDecideState
    u8  machine_kind_a;    // 0x0c, cached machine id from RiderData.machine_gobj (refreshed each perceive)
    u8  machine_kind_b;    // 0x0d, second cached machine id
    u8  city_kind;         // 0x0e, Gm_GetCityKind() captured at init (selects the AI profile)
    u8  stage_kind;        // 0x0f, stGetCurrentStageKind() captured at init (selects the AI profile)
    int maneuver;          // 0x10, TACTICAL maneuver (0..0x15), dispatched by Rider_ProcessCPUManeuver
    int base_maneuver;     // 0x14, fallback maneuver a strategic state parks on (set to 1 or 2); handlers return here via `maneuver = base_maneuver`
    int scratch_18;        // 0x18, cleared at the top of each decide pass but never read (vestigial)
    uint desire_flags;     // 0x1c, INHIBITOR bits (set = suppress a reaction), seeded from the action state
                           //       and cleared each decide pass. 0x100 no-ram-press, 0x200 no-avoidance,
                           //       0x400 no-dodge/attack-scan, 0x1000000 no-charge/intercept
    u8  suppress_timer;    // 0x20, countdown; while > 0 the perceive stage forces target_secondary = -1
    u8  x21;               // 0x21
    u8  difficulty_level;  // 0x22, AI skill level 0..8; Rider_CPUDifficultyScale scales every personality roll by it
    u8  x23;               // 0x23
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
    s16 rival_player_idx;  // 0x70, target rival's player index (5 = none); written by the rival selector, read by attack/patrol via Ply_GetPosition
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
    Vec3 ramcharge_target_pos; // 0xd0, chosen rival's position copied here by the arbiter for RamCharge/PursueLOS (maneuvers 3/4)
    u8  xdc[4];            // 0xdc
    void *xe0;             // 0xe0, current path/spline object pointer
    u8  route_header;      // 0xe4, packed route cache header (bit7 = valid, bits 2..5 = entry count)
    u8  xe5[3];            // 0xe5
    struct { int id; int flag; } route_entries[5]; // 0xe8, upcoming nav-node route cache
    int cmd_timer;         // 0x110, command countdown; reads next opcode when it hits 0
    u8 *cmd_read_ptr;      // 0x114, command VM playback position within cmd_buffer (0 = idle)
    u8 *cmd_write_ptr;     // 0x118, where maneuver handlers append opcodes (reset to cmd_buffer each maneuver)
    u8  cmd_buffer[0x80];  // 0x11c, command opcode stream (ends at 0x19c)
} CpuData;

typedef struct RiderData
{
    int x0;                               // 0x0
    RiderKind kind;                       // 0x4
    u8 ply;                               // 0x8
    u8 x9;                                // 0x9
    u8 color_idx;                         // 0xa
    u8 xb;                                // 0xb
    MachineKind starting_machine_idx : 8; // 0xc
    int x10;                              // 0x10
    int x14;                              // 0x14
    rdDataKirby *rdDataKirby;             // 0x18
    int state_idx;                        // 0x1c
    int x20;                              // 0x20
    int state_frame;                      // 0x24
    int x28;                              // 0x28
    int x2c;                              // 0x2c
    int x30;                              // 0x30
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
    int x5c;                              // 0x5c, body ColAnim overlay state (~0xac bytes, to 0x108) - the
                                          //       animated color overlay that recolors the whole body.
                                          //       index 2 = hurt flash, index 3 = invincibility flash.
                                          //       A rider has three such states (0x5c, 0x108, 0x1b4), each with
                                          //       a priority byte at state+0xa9; ColAnim_GetActiveSlot renders
                                          //       the highest, and ColAnim_Apply rejects a lower-priority anim.
    int x60;                              // 0x60
    int x64;                              // 0x64
    int x68;                              // 0x68
    int x6c;                              // 0x6c
    int x70;                              // 0x70
    int x74;                              // 0x74
    int x78;                              // 0x78
    int x7c;                              // 0x7c
    int x80;                              // 0x80
    int x84;                              // 0x84
    int x88;                              // 0x88
    int x8c;                              // 0x8c
    int x90;                              // 0x90
    int x94;                              // 0x94
    int x98;                              // 0x98
    int x9c;                              // 0x9c
    int xa0;                              // 0xa0
    int xa4;                              // 0xa4
    int xa8;                              // 0xa8
    int xac;                              // 0xac
    int xb0;                              // 0xb0
    int xb4;                              // 0xb4
    int xb8;                              // 0xb8
    int xbc;                              // 0xbc
    int xc0;                              // 0xc0
    int xc4;                              // 0xc4
    int xc8;                              // 0xc8
    int xcc;                              // 0xcc
    int xd0;                              // 0xd0
    int xd4;                              // 0xd4
    int xd8;                              // 0xd8
    int xdc;                              // 0xdc
    int xe0;                              // 0xe0
    int xe4;                              // 0xe4
    int xe8;                              // 0xe8
    int xec;                              // 0xec
    int xf0;                              // 0xf0
    int xf4;                              // 0xf4
    int xf8;                              // 0xf8
    int xfc;                              // 0xfc
    int x100;                             // 0x100
    int x104;                             // 0x104
    int x108;                             // 0x108, second ColAnim overlay state - the additive glow aura driven
                                          //        from copy-ability state code, independent of the body overlay
    int x10c;                             // 0x10c
    int x110;                             // 0x110
    int x114;                             // 0x114
    int x118;                             // 0x118
    int x11c;                             // 0x11c
    int x120;                             // 0x120
    int x124;                             // 0x124
    int x128;                             // 0x128
    int x12c;                             // 0x12c
    int x130;                             // 0x130
    int x134;                             // 0x134
    int x138;                             // 0x138
    int x13c;                             // 0x13c
    int x140;                             // 0x140
    int x144;                             // 0x144
    int x148;                             // 0x148
    int x14c;                             // 0x14c
    int x150;                             // 0x150
    int x154;                             // 0x154
    int x158;                             // 0x158
    int x15c;                             // 0x15c
    int x160;                             // 0x160
    int x164;                             // 0x164
    int x168;                             // 0x168
    int x16c;                             // 0x16c
    int x170;                             // 0x170
    int x174;                             // 0x174
    int x178;                             // 0x178
    int x17c;                             // 0x17c
    int x180;                             // 0x180
    int x184;                             // 0x184
    int x188;                             // 0x188
    int x18c;                             // 0x18c
    int x190;                             // 0x190
    int x194;                             // 0x194
    int x198;                             // 0x198
    int x19c;                             // 0x19c
    int x1a0;                             // 0x1a0
    int x1a4;                             // 0x1a4
    int x1a8;                             // 0x1a8
    int x1ac;                             // 0x1ac
    int x1b0;                             // 0x1b0
    int x1b4;                             // 0x1b4, third ColAnim overlay state, same layout as 0x5c
    int x1b8;                             // 0x1b8
    int x1bc;                             // 0x1bc
    int x1c0;                             // 0x1c0
    int x1c4;                             // 0x1c4
    int x1c8;                             // 0x1c8
    int x1cc;                             // 0x1cc
    int x1d0;                             // 0x1d0
    int x1d4;                             // 0x1d4
    int x1d8;                             // 0x1d8
    int x1dc;                             // 0x1dc
    int x1e0;                             // 0x1e0
    int x1e4;                             // 0x1e4
    int x1e8;                             // 0x1e8
    int x1ec;                             // 0x1ec
    int x1f0;                             // 0x1f0
    int x1f4;                             // 0x1f4
    int x1f8;                             // 0x1f8
    int x1fc;                             // 0x1fc
    int x200;                             // 0x200
    int x204;                             // 0x204
    int x208;                             // 0x208
    int x20c;                             // 0x20c
    int x210;                             // 0x210
    int x214;                             // 0x214
    int x218;                             // 0x218
    int x21c;                             // 0x21c
    int x220;                             // 0x220
    int x224;                             // 0x224
    int x228;                             // 0x228
    int x22c;                             // 0x22c
    int x230;                             // 0x230
    int x234;                             // 0x234
    int x238;                             // 0x238
    int x23c;                             // 0x23c
    int x240;                             // 0x240
    int x244;                             // 0x244
    int x248;                             // 0x248
    int x24c;                             // 0x24c
    int x250;                             // 0x250
    int x254;                             // 0x254
    int x258;                             // 0x258
    int x25c;                             // 0x25c
    int x260;                             // 0x260
    int x264;                             // 0x264
    int x268;                             // 0x268
    int x26c;                             // 0x26c
    int x270;                             // 0x270
    int x274;                             // 0x274
    int x278;                             // 0x278
    int x27c;                             // 0x27c
    int x280;                             // 0x280
    int x284;                             // 0x284
    int x288;                             // 0x288
    int x28c;                             // 0x28c
    int x290;                             // 0x290
    int x294;                             // 0x294
    int x298;                             // 0x298
    int x29c;                             // 0x29c
    int x2a0;                             // 0x2a0
    int x2a4;                             // 0x2a4
    int x2a8;                             // 0x2a8
    int x2ac;                             // 0x2ac
    void *ability_hat_model;              // 0x2b0, rider model container. **+0x0 is the body model JOBJ root,
                                          //        baked every frame by Rider_ApplyModelMatrix; **+0x120 is the
                                          //        copy-ability hat JObj (NULL with no ability), which the
                                          //        projectile spawners use as the throw bone and assert on.
    int x2b4;                             // 0x2b4
    int x2b8;                             // 0x2b8
    int x2bc;                             // 0x2bc
    DOBJ *dobj_lookup_arr;                // 0x2c0, the body's render objects, one per material slot and indexed
                                          //        by material index. The entry point for any per-material color
                                          //        write, and what the recolor path drives MatAnim AObjs through.
    int x2c4;                             // 0x2c4
    int x2c8;                             // 0x2c8
    int x2cc;                             // 0x2cc
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
    Vec3 hand_bone_pos;                   // 0x318, world-space anchor read by Rider_GetHandBonePos.
                                          // Used by Bomb_State0_SnapToHand for the bomb HELD-state position
                                          // and by spawnFireAura/spawnSpikeAura/spawnIceAura as the aura
                                          // spawn position - presumably the rider's hand bone.
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
        int x3f0;           // 0x3f0
    } input;
    GOBJ *machine_gobj;        // 0x3f4
    GOBJ *x3f8;                // 0x3f8
    int x3fc;                  // 0x3fc
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
    int x43c;                  // 0x43c
    int efgroup;               // 0x440, EfGroup bucket this rider's effects spawn into
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
    int x4c4;                  // 0x4c4
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
    int x4f0;                  // 0x4f0
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
    int patch_drop_cooldown;   // 0x590, per-spawn cooldown, reset to game_singleton[0x21c] after each spawn
                               //        and to 0 on a fresh Rider_DropPatches session
    int patch_drop_progress;   // 0x594, drops dispatched this session. Below game_singleton[0x220] the
                               //        sub-handler spawns sequentially; at or above it switches to the burst
                               //        path. Reset to 0 on a fresh session.
    int patch_drop_count;      // 0x598, queued patch-item count for the per-frame drop consumer; written by Rider_DropPatches
    int patch_drop_mode;       // 0x59c, drop_mode from the last Rider_DropPatches call. Mode 1 negates the
                               //        velocity vector so drops land behind the rider; 0 and 2 land in front.
    int x5a0;                  // 0x5a0
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
    int x640;                  // 0x640
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
    int x6d4;                  // 0x6d4
    int x6d8;                  // 0x6d8
    int x6dc;                  // 0x6dc
    int x6e0;                  // 0x6e0
    int x6e4;                  // 0x6e4
    int x6e8;                  // 0x6e8
    int x6ec;                  // 0x6ec
    int x6f0;                  // 0x6f0
    int x6f4;                  // 0x6f4
    int x6f8;                  // 0x6f8
    int x6fc;                  // 0x6fc
    int x700;                  // 0x700
    int x704;                  // 0x704
    int x708;                  // 0x708
    int x70c;                  // 0x70c
    int x710;                  // 0x710
    int x714;                  // 0x714
    int x718;                  // 0x718
    int x71c;                  // 0x71c
    int x720;                  // 0x720
    int x724;                  // 0x724
    int x728;                  // 0x728
    int x72c;                  // 0x72c
    int x730;                  // 0x730
    int x734;                  // 0x734
    int x738;                  // 0x738
    int x73c;                  // 0x73c
    int x740;                  // 0x740
    float x744;                // 0x744
    float x748;                // 0x748
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
    int x768;                  // 0x768
    int x76c;                  // 0x76c
    int x770;                  // 0x770
    int x774;                  // 0x774
    CpuData *cpu;              // 0x778, CPU rider AI state / virtual pad. NULL for human riders.
    int x77c;                  // 0x77c
    int x780;                  // 0x780
    int x784;                  // 0x784
    int x788;                  // 0x788
    int x78c;                  // 0x78c
    int x790;                  // 0x790
    int x794;                  // 0x794
    int x798;                  // 0x798
    int x79c;                  // 0x79c
    int x7a0;                  // 0x7a0
    int x7a4;                  // 0x7a4
    int x7a8;                  // 0x7a8
    int x7ac;                  // 0x7ac
    int x7b0;                  // 0x7b0
    struct                     //
    {                          //
        void (*anim)(GOBJ *);  // 0x7b4
        void (*iasa)(GOBJ *);  // 0x7b8
        void (*phys)(GOBJ *);  // 0x7bc
        void (*coll)(GOBJ *);  // 0x7c0
        void (*x7c4)(GOBJ *);  // 0x7c4, runs per frame from gobj proc 8018f7b0
        void (*x7c8)(GOBJ *);  // 0x7c8, runs per frame from gobj proc 8018fc10
        void (*x7cc)(GOBJ *);  // 0x7cc, runs per frame from gobj proc 8018e9f0
        void (*x7d0)(GOBJ *);  // 0x7d0
        void (*x7d4)(GOBJ *);  // 0x7d4
        void (*x7d8)(GOBJ *);  // 0x7d8
    } cb;
    int x7dc;                           // 0x7dc
    int x7e0;                           // 0x7e0
    int x7e4;                           // 0x7e4
    int x7e8;                           // 0x7e8
    int x7ec;                           // 0x7ec
    int x7f0;                           // 0x7f0
    int x7f4;                           // 0x7f4
    // 0x7f8 / 0x7fc: per-ability teardown callbacks installed at grant, each called
    // with the rider in r3. An ability installs into one slot of the pair: +0x7f8
    // for Fire (0x801af618), Sword (0x801aff1c), Bomb (0x801b13ac); +0x7fc for
    // Wheel (via 0x801af638) and Bird. Both route through
    // Rider_TeardownCopyAbility (0x801a810c), which resets copy_kind to -1, spawns
    // the "ability lost" poof VFX/SFX, and removes the ability model/hat; clearing
    // the callback field itself is left to the per-ability wrapper. Don't call
    // these directly - use Rider_AbilityRemoveModel (0x80191554), which calls
    // +0x7f8 and then +0x7fc, skipping either when NULL (that's how the engine
    // strips the old ability on a new inhale).
    void (*cb_ability_remove2)(RiderData *); // 0x7f8
    void (*cb_ability_remove)(RiderData *);  // 0x7fc
    int x800;                           // 0x800
    int x804;                           // 0x804
    int x808;                           // 0x808
    int x80c;                           // 0x80c
    int x810;                           // 0x810
    int x814;                           // 0x814
    int x818;                           // 0x818, bit 2 (0x04) = attack/charge input active; read by Rider_CanStartInhale
    int x81c;                           // 0x81c
    u8 x820;                            // 0x820
    u8 x821;                            // 0x821
    u8 x822;                            // 0x822
    u8 x823;                            // 0x823
    u8 x824;                            // 0x824
    u8 x825;                            // 0x825
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
    // 0x92c: per-frame ability tick (the copy_kind's abilityTimer_* fn), called by
    // abilityTimerBranchToAbilityCountdown (0x801a5f68) while in the ability action-
    // state. Decrements copy_timer and runs the drop at 0. Not installed by every
    // kind (Bomb has none).
    void (*cb_ability_tick)(RiderData *); // 0x92c
    void (*cb_copy_input)(RiderData *); // 0x930
    int x934;                           // 0x934
    int x938;                           // 0x938
    union                               // 0x93c
    {
        CopyKind copy_wheel_result;     // CopyKind the copy wheel selected
        s32 inhale_timer;               // reused during the inhale action-state; the
                                        // gesture ends when it counts down to 0
    };
    int x940;                           // 0x940
    int x944;                           // 0x944
    int x948;                           // 0x948
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
    int x9d8;                           // 0x9d8
    int x9dc;                           // 0x9dc
    int x9e0;                           // 0x9e0
    int x9e4;                           // 0x9e4
    int x9e8;                           // 0x9e8
    int x9ec;                           // 0x9ec
    struct
    { 
        int is_bike;                    // 0x9f0
        MachineKind kind;               // 0x9f4
    } machine_saved;                    // used to remember your actual machine when an ability changes your machine
    int x9f8;                           // 0x9f8
    int x9fc;                           // 0x9fc
    int xa00;                           // 0xa00
    int xa04;                           // 0xa04
    int xa08;                           // 0xa08
    int xa0c;                           // 0xa0c
    int xa10;                           // 0xa10
    int xa14;                           // 0xa14
    int x;                              // 0xa18
    int xa1c;                           // 0xa1c
    int xa20;                           // 0xa20
    int xa24;                           // 0xa24
    int xa28;                           // 0xa28
    int xa2c;                           // 0xa2c
    int xa30;                           // 0xa30
    int xa34;                           // 0xa34
    int xa38;                           // 0xa38, quick-spin scratch, cleared on Rider_QuickSpin_Enter
    int xa3c;                           // 0xa3c
    int xa40;                           // 0xa40, quick-spin accumulators: byte +0xa40 = CW frames, +0xa41 = CCW
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
    int xa7c;                           // 0xa7c
    int xa80;                           // 0xa80
    int xa84;                           // 0xa84
    int xa88;                           // 0xa88
    int xa8c;                           // 0xa8c
    int xa90;                           // 0xa90
    int xa94;                           // 0xa94
    int xa98;                           // 0xa98
    int xa9c;                           // 0xa9c
    int xaa0;                           // 0xaa0
    int xaa4;                           // 0xaa4
    int xaa8;                           // 0xaa8
    int xaac;                           // 0xaac
    int xab0;                           // 0xab0
    int xab4;                           // 0xab4
    int xab8;                           // 0xab8
    int xabc;                           // 0xabc
    int xac0;                           // 0xac0
} RiderData;

static rdDataKirby **stc_rdDataKirby = (rdDataKirby **)0x80559fa8;

// Copy ability initialization function table. 11 entries (one per CopyKind),
// each pointing to the ability's init function (e.g., ability_Fire at 0x801af474).
// Indexed by CopyKind. NULL entry means the ability is not implemented.
typedef void (*AbilityInitFunc)(RiderData *);
static AbilityInitFunc *stc_ability_init_table = (AbilityInitFunc *)0x804af4f0;

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

// CPU rider AI ("virtual pad") pipeline.
void Rider_CPUThink(GOBJ *gobj);          // 0x8018fc58, rider proc: if CPU, runs the AI update
void Rider_UpdateCPU(RiderData *rd);      // 0x8026beec, orchestrates perceive -> decide -> process -> emit
void Rider_CPUDecideState(RiderData *rd); // 0x802716e8, AI state-machine dispatch (11 states, table 0x804b7a28)
void Rider_CPUProcessCmd(RiderData *rd);  // 0x80275cbc, plays the command stream into the virtual pad (CpuData stick/buttons)
// The maneuver chooser: a priority cascade gated by behavior/desire flags,
// committing CpuData.maneuver.
void Rider_CPUArbitrateManeuver(RiderData *rd); // 0x80274ec0
// Builds the look-ahead waypoint route into scratch 0x8055e964, returning 0 if
// the route is invalid. Does not pick the maneuver.
int  Rider_CPUBuildRoute(CpuData *cpu, void *route_scratch, uint *flags_out); // 0x8026a734
void Rider_CPUUpdateNavTarget(RiderData *rd); // 0x8026b6d0, nearest-node spatial query -> assigns CpuData.target_primary (+0x38) + arc (+0x3c)
// ORs inhibitor bits into CpuData.desire_flags from the rider's action state.
void Rider_CPUSeedDesire(RiderData *rd);  // 0x802762dc
// Target-selection scans (populate CpuData target fields each frame; signatures approximate).
void  Rider_CPURivalSelect(RiderData *rd);        // 0x80264210, scores 5 slots -> rival_player_idx (+0x70); shared by states 1/2/4/8/10
void  Rider_CPUScanItems(RiderData *rd);          // 0x80263c4c, top-5 ranked item scan -> item_target (+0x74) (states 3/8)
void  Rider_CPUScanCityObjects(RiderData *rd);    // 0x802638a4, top-5 ranked city-object scan -> city_object (+0x84) (state 3)
void  Rider_CPUScanRouteGoal(RiderData *rd);      // 0x80263fd0, single-best route-goal scan -> route_goal (+0x94) (states 5/6)
void  Rider_CPUSelectChargeAnchor(RiderData *rd); // 0x80263610, resolves charge anchor -> charge_anchor (+0xb0/+0xb4) (state 7)
void  Rider_CPUBlendRoutePoints(RiderData *rd);   // 0x80267238, blends item/interaction/path-point positions into the steering route
// Perception (fill the per-frame scratch buffers).
void  Rider_CPUCollectHazards(RiderData *rd);     // 0x80269928, hazard/threat list -> scratch 0x8055e698 (behavior bit 0x02 adds a 5th pass)
void  Rider_CPUCollectRiderHazards(RiderData *rd); // 0x80268234, hazard pass 1: rider bodies + hurt-volumes
void  Rider_CPUForwardLookahead(RiderData *rd);   // 0x80269f10, forward-collision list -> scratch 0x8055e8b4 (behavior bit 0x80 gates it)
void  Rider_CPUForwardLookaheadSetup(RiderData *rd); // 0x8026a498, wrapper: builds the basis then calls Rider_CPUForwardLookahead
// Advances along the spline graph by arc-length. out = {node_id, along, side},
// where side is a link-direction tag of -1/0/+1.
int   Rider_CPUWalkRoute(RiderData *rd, float arc_len, void *out, void *spline_lookup); // 0x80264924
// Steering / command emission.
void  Rider_CPUEmitSteer(RiderData *rd, Vec3 *desired_dir);      // 0x8026d6a0, projects + avoidance-bends a heading, emits steering opcodes
void  Rider_CPUEmitSteerStick(RiderData *rd, Vec3 *desired_dir); // 0x8026c4ec, yaw error -> opcode 190 nudge / 192 ramp (stick_x), 129 (stick_y)
void  Rider_CPUResolveAvoidVector(RiderData *rd, Vec3 *dir);     // 0x8026d1fc, rotates a heading off the nearest imminent hazard (reads 0x8055e698)
void  Rider_CPUEmitAbilityAction(RiderData *rd);  // 0x80273d1c, post-maneuver copy-ability press emitter
void  Rider_CPUTerminateCmdStream(RiderData *rd); // 0x80276228, caps the per-maneuver command stream (opcode 0x7f), arms the VM
int   Rider_CPUEmitChargeStutter(RiderData *rd);  // 0x8026da40, velocity-stuck charge-pump (press -> hold-20 -> hold-40); returns 1 if it emitted
void  Rider_CPUTrackStuckProgress(RiderData *rd); // 0x8026ccec, ticks the stuck counter (+0x5c) vs the machine's max-turn tolerance
// Difficulty / envelope getters.
int   Rider_CPUGetAbilityPressHold(CpuData *cpu); // 0x802765d4, per-difficulty ability press-hold frames (table 0x804b7f30)
void  Rider_CPUGetSteerEnvelope(CpuData *cpu, int *step_out, int *cap_out); // 0x80276650, per-difficulty (step,cap) steer envelope (table 0x804b7f54)
float Rider_CPUGetMachineTurnTolerance(RiderData *rd); // 0x802776c4, per-machine max-turn angle (table 0x804b8f30)
int  Rider_GetCPUButtons(RiderData *rd);  // 0x80275cb0, returns CpuData.buttons (+0x00)
int  Rider_GetCPUStickX(RiderData *rd);   // 0x80275c90, returns CpuData.stick_x (+0x04)
int  Rider_GetCPUStickY(RiderData *rd);   // 0x80275ca0, returns CpuData.stick_y (+0x06)
void Rider_InputThink(GOBJ *gobj);        // 0x8018ee28, rider proc: selects effective input (human / CPU / replay)

void Rider_RespawnEnter(RiderData *);
int Rider_GiveAbility(RiderData *, CopyKind);
int Rider_CheckUnableAbility(RiderData *); // checks if the rider can receive an ability?
// 0x80191554. Universal "remove the currently-held copy ability" teardown: if
// copy_kind or powerup_kind is set, invokes whichever per-ability teardown
// callbacks are installed (cb_ability_remove at +0x7fc, and its sibling at +0x7f8
// used by e.g. Bomb). Those route through Rider_TeardownCopyAbility (0x801a810c),
// which resets copy_kind to -1, spawns the "ability lost" poof VFX/SFX, removes
// the ability model/hat, and clears the ability callback fields. This is what
// Rider_GiveAbility calls to strip the old ability before granting a new one, so
// it works for every copy_kind. Does NOT play the spit-out animation - pair with
// Rider_LoseAbilityState_Enter for that.
void Rider_AbilityRemoveModel(RiderData *);
// Cancels queued copy-ability and power-up grants, freeing their pending objects
// and resetting queued_ability_kind / queued_powerup_kind to -1.
void Rider_AbilityClearQueued(RiderData *); // 0x801915c4
void Rider_LoseAbilityState_Enter(RiderData *);
void Rider_GiveIntangibility(RiderData *, int time);
void Rider_GiveInvincibility(RiderData *, int time);
int RiderGObj_GetPly(GOBJ *gobj); // 0x8019203c, returns player index from a rider GOBJ
int Rider_IsOnMachine(RiderData *);
int Rider_IsMachineDead(RiderData *);       // can only be called between the RDPRI_HITCOLL and RDPRI_DMGAPPLY priority.
// Enqueues a stat-patch drop event; Rider_TickDropPatches drains it per frame.
// drop_mode 0 = forward, small fixed count, probabilistic all-up; 1 = behind,
// count scaled by stats, no all-ups; 2 = forward, count scaled by stats, all
// remaining all-ups.
void Rider_DropPatches(RiderData *, float stat_array[9], int drop_mode); // 0x8019d330
int Rider_CheckCanReceiveAbility(GOBJ *gobj); // returns 1 if rider can receive a copy ability
int Rider_CheckAndGiveAbility(GOBJ *gobj, int kind); // checks rider is Kirby, then gives copy ability, returns 1 on success
// Appends to PlayerStats.copy_history, checks the three ability sequences and
// bumps copy_obtain_count. Runs for every grant, whatever the source.
void Rider_RecordCopyAbility(int ply, int copy_kind); // 0x8022ee00
// Sets the PlayerStats.copy_chance_mask bit (15-copy_kind). Only the copy-wheel
// paths call it, so the bit means "the wheel gave it".
void Rider_MarkCopyAbilityObtained(int ply, int copy_kind); // 0x8022f150

// Action states ability_Mic (0x801b3dac) drives: the held pose, the singing blast
// (spawns Effect 0x5a5a2 / SFX 0x2006b), and the recovery the blast ends in.
#define RIDERSTATE_MIC_HOLD 0x3e
#define RIDERSTATE_MIC_SING 0x61
#define RIDERSTATE_MIC_END  0x62
int randomAbility_giveAbility(RiderData *, int kind); // 0x801a61d4, gives copy ability from copy chance wheel (no unable/queue check)
// Initializes the copy wheel at a starting ability, setting copy_wheel_ability_list and index.
void Rider_StartCopyWheel(RiderData *rd, int copy_kind); // 0x801ae550
// Picks a random starting ability and starts the wheel; returns 1 if it started.
int Rider_StartRandomCopyWheel(RiderData *rd); // 0x801ae4ec
int Rider_GiveRandomAbility(GOBJ *gobj); // 0x80191fb8, GOBJ wrapper for Rider_StartRandomCopyWheel
int Rider_CheckCanReceivePowerUp(GOBJ *gobj); // returns 1 if rider can receive a power-up (checks lower 4 bits of rd->x825 are clear)
int Rider_GivePowerUp(GOBJ *gobj, PowerUpKind kind); // gives rider a power-up if rider is Kirby, returns 1 on success
int Rider_TryGivePowerUp(RiderData *rd, PowerUpKind kind); // checks unable, queues into x460 or calls Rider_GivePowerUpByKind
int Rider_GivePowerUpByKind(RiderData *rd, PowerUpKind kind); // removes current ability and initializes power-up kind (0-3), returns 1 on success

// Inhale (no-copy-ability default attack). The native pipeline only targets
// EventActor enemies and only enters the state when one is already in range, so
// it never fires in City Trial - but Rider_StartInhale can be called directly to
// drive the open-mouth suck animation + native suction VFX/SFX for custom mods.
//
// The inhale is three action-states in RiderData.state_idx (anim id, +0x1c): 0x2f suck
// START, 0x30 suck LOOP, 0x31 suck END. They are NOT chained automatically:
//   * START (0x2f) is a ONE-SHOT gulp. When its body anim finishes the engine resolves to
//     a normal riding state (Rider_InhaleStartProc -> the generic ability-resolve/star-wait
//     path); it does NOT advance to the LOOP. A tap of the native inhale is just this gulp.
//   * LOOP (0x30) is entered ONLY via Rider_StartInhaleLoop. Its process then re-enters 0x30
//     itself each time the body anim finishes (Rider_IsBodyAnimDone), so the suck sustains
//     and animates on its own. It self-terminates when a countdown at RiderData+0x93C
//     reaches 0 (Rider_InhaleLoopTick, only while the mouth is empty) -> Rider_EndInhale.
//   * END (0x31) closes the mouth (+ a close puff) and returns to neutral.
// NOTE +0x93C is per-action-state scratch that ALIASES copy_wheel_result, so it is not a
// dependable timer when driving the inhale from mod code. To drive a held suck: call
// Rider_StartInhale for the gulp, then once START ends call Rider_StartInhaleLoop to enter
// the LOOP, keep +0x93C topped up so the engine doesn't time it out, and call
// Rider_EndInhale yourself on release for a clean vanilla ending.
// Force action-state 0x76: inhale anim, suction effect, SFX, and the per-frame
// capture callbacks, with no gate or target check. A one-shot gulp - it returns
// to neutral when the anim ends and does not enter the LOOP.
void Rider_StartInhale(RiderData *rd);        // 0x801ad2c4
// Enter or re-enter the suck-LOOP substate (action-state 0x77), reinstalling the
// scan/volume callbacks without respawning VFX/SFX or resetting captures. The
// LOOP process calls this itself on body-anim-done to sustain the suck, and it
// is also how the LOOP is first entered - the engine never advances into it.
void Rider_StartInhaleLoop(RiderData *rd);    // 0x801ad4cc
// End the suck: action-state 0x78, close anim, close puff, back to neutral. The
// engine's own ending; call it to stop a driven LOOP cleanly.
void Rider_EndInhale(RiderData *rd);          // 0x801adf98
// 1 once the rider's body motion has played to its end. Gates the LOOP process's
// per-cycle re-entry of the suck-LOOP anim.
int  Rider_IsBodyAnimDone(RiderData *rd);     // 0x80198b00
// Gate: attack bit set, no copy ability, and fewer than 3 captures held.
int  Rider_CanStartInhale(RiderData *rd);     // 0x801a617c
// Per-frame entry probe: starts an inhale if the gate passes and an inhalable
// EventActor overlaps the mouth volume.
void Rider_TryStartInhale(RiderData *rd);     // 0x8019c5ac
// Per-frame scan of the EventActor bucket, capturing up to 3 a frame (list cap 10).
void Rider_InhaleCaptureScan(RiderData *rd);  // 0x8019c63c
// Candidate predicate: EventActor enemies only. Items and yakumono never pass.
int  EventActor_IsInhalable(GOBJ *cand);      // 0x802041c8

// Quick spin (stick-rotation spin attack). Rider_UpdateQuickSpinTimers
// (0x80191a58) ticks the CW/CCW frame accumulators at RiderData+0xa40 / +0xa41 -
// frames since the stick was last held past the threshold in that direction -
// and Rider_CheckQuickSpinInput (0x80191980) reads them back against the config
// struct at *0x805DD814 (+0x1b0) to spot a flick. A state that omits the tick
// leaves the accumulators frozen, so the detector cannot fire there. When it
// fires, Rider_QuickSpin_Enter transitions to action-state 0x2c (anim 0x6a CCW /
// 0x6b CW), grants i-frames, and applies the spin hitbox. Two entry paths funnel
// here: Rider_IASACheck_QuickSpin (0x801b7e80, excludes copy_kind PLASMA) and the
// neutral-state entry Rider_TryQuickSpinNeutral (0x801b7e0c). The Tornado copy
// ability's own spin shares the detector but enters via a DIFFERENT function, so
// it is unaffected.
// Per-frame tick of the +0xa40 / +0xa41 accumulators: 0 while held in that
// direction, saturating at 0xfe.
void Rider_UpdateQuickSpinTimers(RiderData *rd); // 0x80191a58
// Per-frame interrupt check: excludes copy_kind PLASMA, reads the stick, and
// enters the spin on a flick, returning 1 if it did. Called from the grounded
// rider state but not the airborne one.
int  Rider_IASACheck_QuickSpin(RiderData *rd); // 0x801b7e80
// dir is +1 CW / -1 CCW, flag 1 applies the hitbox.
void Rider_QuickSpin_Enter(float f, RiderData *rd, int dir, int flag); // 0x801b7ee4

// Dedede and Meta Knight (alternate rider characters) each have their own
// quick-spin enter, separate from Kirby's Rider_QuickSpin_Enter. Same detector
// (Rider_CheckQuickSpinInput), but the per-character IASA checks funnel through
// these instead: Dedede -> action-state 0x2c, Meta Knight -> 0x2d. Each has a
// single call site.
void Rider_Dedede_QuickSpin_Enter(RiderData *rd, int dir);     // 0x801c05f8, arg regs: r3=rd, r4=dir; sole caller @ 0x801c05d4
void Rider_MetaKnight_QuickSpin_Enter(RiderData *rd, int dir); // 0x801c3f90, arg regs: r3=rd, r4=dir; sole caller @ 0x801c3f6c
// Per-character quick-spin interrupt checks, installed in their grounded states
// but not their air control.
int  Rider_Dedede_IASACheck_QuickSpin(RiderData *rd);     // 0x801c05a8
int  Rider_MetaKnight_IASACheck_QuickSpin(RiderData *rd); // 0x801c3f40

// Airborne machine-riding state logic. Each rider character has its own rider
// state-descriptor table, so this state has one callback per character - the
// three sole callers of the airborne input helper 0x8019fcf0. Kirby's is
// airControl (0x801ac128). Dedede's runs the charge check then
// Rider_UpdateQuickSpinTimers; Meta Knight's runs only the charge check, so his
// spin accumulators stay frozen while airborne.
void Rider_Dedede_AirControl(RiderData *rd);     // 0x801bf534
void Rider_MetaKnight_AirControl(RiderData *rd); // 0x801c2b08

// Kirby recolor. Three native paths:
//   1. Material-index swap (discrete baked palettes - the 8 player colors + wing/fire):
//      RiderKirby_SetMaterialColor stages model_part[part].cur_mat_index + dirty bit;
//      RiderKirby_SetMaterialColorAndUpdate drives the model's MatAnim AObj to that
//      baked color keyframe (walks dobj_lookup_arr at RiderData+0x2c0).
//   2. ColAnim overlay (animated, time-based color flash/glow): Rider_ApplyColAnim
//      selects a baked color-anim from the global table into RiderData+0x5c (body)
//      via the generic ColAnim_Apply. Used by hurt (index 2) / invincibility (index 3).
//   3. Direct material color: walk dobj_lookup_arr[i] -> MObj -> HSD_Material and write
//      ambient/diffuse (GXColor) each frame for an arbitrary smooth hue (no baked limit).
// Stages model_part[part].cur_mat_index and sets the recolor-dirty bit.
void RiderKirby_SetMaterialColor(RiderData *rd, int part_idx, u8 mat_index);          // 0x80198d1c
// Stages it and immediately drives the body MatAnim to the new baked color.
void RiderKirby_SetMaterialColorAndUpdate(RiderData *rd, int part_idx, u8 mat_index); // 0x80198d3c
u8   Rider_GetColor(RiderData *rd);                                                   // 0x80192758, PlayerData.color_idx
// Requests a baked color-overlay anim into the body overlay at rd+0x5c;
// anim_index selects from the global table (3 = invincibility).
int  Rider_ApplyColAnim(RiderData *rd, int anim_index, int param); // 0x8019bfb4
// Generic priority-gated ColAnim applier; colanim_state is rd+0x5c or rd+0x108.
int  ColAnim_Apply(void *colanim_state, void *table, int index, int param);          // 0x8006a3f0
void ColAnim_Reset(void *colanim_state);                                             // 0x8006a250, clears the tint

// Reads the machine's projectile inherit velocity via the rider's
// machine_gobj, into *out. Thin wrapper around
// MachineGObj_GetProjectileBaseVelocity.
void Rider_GetProjectileBaseVelocity(RiderData *rd, Vec3 *out); // 0x8019407c

// 8-instruction Vec3 readers - `gobj` is the rider GObj, `out` is filled with
// the corresponding RiderData field. The "hand bone" naming for rd+0x318
// reflects its use by the bomb HELD-snap and the fire/spike/ice aura spawn
// helpers - the field is not directly named in the struct.
void Rider_GetHandBonePos(GOBJ *gobj, Vec3 *out); // 0x80191ffc, reads rd[0x318..0x320]
void Rider_GetForward(GOBJ *gobj, Vec3 *out);     // 0x80191ef8, reads rd->forward (rd+0x324)
void Rider_GetUp(GOBJ *gobj, Vec3 *out);          // 0x80191f18, reads rd->up      (rd+0x330)

void Rider_SetCandyTimer(GOBJ *gobj, int duration); // stores duration in rd->candy_duration, enters rider state 47 (countdown timer)

AudioEmitter Rider_AllocAudioEmitter(int index);

#endif