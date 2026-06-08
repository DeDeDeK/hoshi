#ifndef KAR_H_ENEMY
#define KAR_H_ENEMY

#include "datatypes.h"
#include "trigger.h"

// Actor type IDs for EventActor_Create. Encompasses all enemies and event actors.
// Data table at 0x804b22b4 maps each ID to {data_index, flags}.
typedef enum ActorID
{
    // Tier 0 — Base enemies (flags=0)
    ACTORID_BROOM_HATTER       = 0x00,
    ACTORID_BROOM_HATTER_B     = 0x01,
    ACTORID_BRONTO_BURT        = 0x02,
    ACTORID_BRONTO_BURT_B      = 0x03,
    ACTORID_SCARFY             = 0x04,
    ACTORID_SWORD_KNIGHT       = 0x05,
    ACTORID_CAPPY              = 0x06,
    ACTORID_CAPPY_B            = 0x07,
    ACTORID_WHEELIE            = 0x08,
    ACTORID_PHAN_PHAN          = 0x09,
    ACTORID_NODDY              = 0x0A,
    ACTORID_CHILLY             = 0x0B,
    ACTORID_FLAPPY             = 0x0C,
    ACTORID_PLASMA_WISP        = 0x0D,
    ACTORID_GORDO              = 0x0E,
    ACTORID_BOMBER             = 0x0F,
    ACTORID_PICHIKURI          = 0x10,
    ACTORID_PICHIKURI_B        = 0x11,
    ACTORID_DAYL               = 0x12,
    ACTORID_DAYL_B             = 0x13,
    ACTORID_CALLER             = 0x14, // Shaturn / Tornado caller
    ACTORID_WALKY              = 0x15,
    ACTORID_WADDLE_DEE_TRUCK   = 0x16,
    ACTORID_WADDLE_DEE         = 0x17,

    // Tier 1 — Enhanced variants (flags=1), same data_index sequence
    ACTORID_T1_BROOM_HATTER       = 0x18,
    ACTORID_T1_BROOM_HATTER_B     = 0x19,
    ACTORID_T1_BRONTO_BURT        = 0x1A,
    ACTORID_T1_BRONTO_BURT_B      = 0x1B,
    ACTORID_T1_SCARFY             = 0x1C,
    ACTORID_T1_SWORD_KNIGHT       = 0x1D,
    ACTORID_T1_CAPPY              = 0x1E,
    ACTORID_T1_CAPPY_B            = 0x1F,
    ACTORID_T1_WHEELIE            = 0x20,
    ACTORID_T1_HEAT_PHAN_PHAN     = 0x21, // Fire-themed variant
    ACTORID_T1_NODDY              = 0x22,
    ACTORID_T1_CHILLY             = 0x23,
    ACTORID_T1_FLAPPY             = 0x24,
    ACTORID_T1_PLASMA_WISP        = 0x25,
    ACTORID_T1_GORDO              = 0x26,
    ACTORID_T1_BOMBER             = 0x27,
    ACTORID_T1_PICHIKURI          = 0x28,
    ACTORID_T1_PICHIKURI_B        = 0x29,
    ACTORID_T1_DAYL               = 0x2A,
    ACTORID_T1_DAYL_B             = 0x2B,
    ACTORID_T1_CALLER             = 0x2C,
    ACTORID_T1_WALKY              = 0x2D,
    ACTORID_T1_WADDLE_DEE_TRUCK   = 0x2E,
    ACTORID_T1_WADDLE_DEE         = 0x2F,

    // Tier 2 — Further enhanced (flags=1), same data_index sequence
    ACTORID_T2_BROOM_HATTER       = 0x30,
    ACTORID_T2_BROOM_HATTER_B     = 0x31,
    ACTORID_T2_BRONTO_BURT        = 0x32,
    ACTORID_T2_BRONTO_BURT_B      = 0x33,
    ACTORID_T2_SCARFY             = 0x34,
    ACTORID_T2_SWORD_KNIGHT       = 0x35,
    ACTORID_T2_CAPPY              = 0x36,
    ACTORID_T2_CAPPY_B            = 0x37,
    ACTORID_T2_WHEELIE            = 0x38,
    ACTORID_T2_HEAT_PHAN_PHAN     = 0x39,
    ACTORID_T2_NODDY              = 0x3A,
    ACTORID_T2_CHILLY             = 0x3B,
    ACTORID_T2_FLAPPY             = 0x3C,
    ACTORID_T2_PLASMA_WISP        = 0x3D,
    ACTORID_T2_GORDO              = 0x3E,
    ACTORID_T2_BOMBER             = 0x3F,
    ACTORID_T2_PICHIKURI          = 0x40,
    ACTORID_T2_PICHIKURI_B        = 0x41,
    ACTORID_T2_DAYL               = 0x42,
    ACTORID_T2_DAYL_B             = 0x43,
    ACTORID_T2_CALLER             = 0x44,
    ACTORID_T2_WALKY              = 0x45,
    ACTORID_T2_WADDLE_DEE_TRUCK   = 0x46,
    ACTORID_T2_WADDLE_DEE         = 0x47,

    // Special / Event actors
    ACTORID_SP_BROOM_HATTER       = 0x48, // flags=2
    ACTORID_SP_SWORD_KNIGHT       = 0x49, // flags=2
    ACTORID_SP_WADDLE_DEE_TRUCK   = 0x4A, // flags=2
    ACTORID_SP_GORDO              = 0x4B, // flags=3, event gordo
    ACTORID_TAC                   = 0x4C,
    ACTORID_DYNA_BLADE            = 0x4D,
    ACTORID_METEOR                = 0x4E,

    ACTORID_NUM                   = 0x4F,

    // Tier boundaries
    ACTORID_TIER0_START           = 0x00,
    ACTORID_TIER0_END             = 0x18,
    ACTORID_TIER1_START           = 0x18,
    ACTORID_TIER1_END             = 0x30,
    ACTORID_TIER2_START           = 0x30,
    ACTORID_TIER2_END             = 0x48,
    ACTORID_SPECIAL_START         = 0x48,
    ACTORID_ENEMIES_PER_TIER      = 0x18, // 24 enemies per tier
} ActorID;

// Descriptor struct passed to EventActor_Create. 0x60 bytes (24 fields).
// Built on the stack by Enemy_SpawnActor and event start functions.
// Fields reconstructed from zz_801fb53c_ (descriptor → EnemyData copy),
// Enemy_SpawnActor, and event_dynablade_start/event_meteor_start.
typedef struct EventActorDesc
{
    int actor_id;           // 0x00: ActorID (0x00–0x4E)
    Vec3 position;          // 0x04: spawn world position
    Vec3 forward;           // 0x10: forward direction (unit vector)
    Vec3 up;                // 0x1C: up direction (unit vector)
    float scale;            // 0x28: model scale (typically abs of a spawn-data value, or 1.0)
    int spawn_index;        // 0x2C: spawn tracking counter. -1 for standalone (skips lifetime field)
    int spawn_slot;         // 0x30: spawn slot index. -1 for standalone
    int x34;                // 0x34: (0 in Enemy_SpawnActor, 0 in event callers)
    int lifetime;           // 0x38: lifetime in frames. Only written to EnemyData if spawn_index != -1
    int x3C;                // 0x3C: parent GOBJ (child spawner sets child_ed[2] = parent_gobj). Also variant flag for special actors 0x48-0x4A.
    int x40;                // 0x40: 1 from Enemy_SpawnActor, varies in event callers
    Vec3 custom_bounds;     // 0x44: custom collision bounds (only used if bounds_flag != -1.0)
    float bounds_flag;      // 0x50: -1.0 = use default bounds. Any other value = use custom_bounds
    Vec3 ground_normal;     // 0x54: ground/surface normal at spawn point
} EventActorDesc;

typedef struct EnemyData
{
    GOBJ *gobj;             // 0x0, this actor's own GOBJ
    GOBJ *child_gobj;       // 0x4, child/rider actor GOBJ (e.g., knight on mount). 0 if none.
    GOBJ *parent_gobj;      // 0x8, parent/target GOBJ. Used by state functions to follow/track another entity. Null crashes some states.
    ActorID kind;           // 0xc
    int x10;                // 0x10
    void *actor_data;       // 0x14, tier-specific data from Enemy_GetActorData. Selects sub-entry based on tier flags (0=T0, 1=T1/T2, 2/3/4=special variants).
    int x18;                // 0x18
    int x1c;                // 0x1c
    int spawn_slot;         // 0x20, from desc. -1 for standalone actors.
    int spawn_index;        // 0x24, from desc. -1 for standalone. Checked against -1 for lifetime logic.
    int lifetime_base;      // 0x28, from desc (only if spawn_index != -1)
    int lifetime_counter;   // 0x2c, decremented each frame in proc 21 for OOB enemies
    int tier_flags;         // 0x30, variant selector (0=T0, 1=T1/T2, 2/3/4=special). From desc.x3C for actors < 0x48.
    int state;              // 0x34, current state ID. Written by EnemyStateChange.
    int per_type_threshold; // 0x38, constant 0x0E set in InitFromDesc; state-ID cutoff between common and per-type states (EnemyStateChange uses a hardcoded 14, so this is informational)
    int anim_idx;           // 0x3c, animation index from state table entry. -1 = no animation. Written by EnemyStateChange (stw r0,60(r28)).
    void *common_state_table; // 0x40, pointer to common states 0x00-0x0D (0x804b2950). EnemyStateChange reads this for state_id < 0x0E (lwz r3,64(r28)).
    void *per_type_state_table; // 0x44, per-type state table for states >= 0x0E; initialized to PTR_PTR_804b1d98[kind][0x00]. EnemyStateChange reads this for state_id >= 0x0E (lwz r3,68(r28)).
    void *anim_data;        // 0x48, current animation data pointer (actor_data+0x0C + anim_idx*0x10)
    float anim_timer;       // 0x4c, animation keyframe timer (decremented per frame by StateMachine)
    float anim_frame;       // 0x50, current animation frame accumulator (= x2a8 + x2ac each frame, written by EventActor_StateMachine)
    void *anim_command_ptr; // 0x54, animation script bytecode pointer
    int anim_loop_depth;    // 0x58, animation script loop nesting depth
    int x5c;                // 0x5c
    int x60;                // 0x60
    int x64;                // 0x64
    int x68;                // 0x68
    int x6c;                // 0x6c
    int x70;                // 0x70
    int x74;                // 0x74
    int x78;                // 0x78
    int x7c;                // 0x7c
    int x80;                // 0x80
    int x84;                // 0x84
    int x88;                // 0x88
    int x8c;                // 0x8c
    int x90;                // 0x90
    int x94;                // 0x94
    int x98;                // 0x98
    int x9c;                // 0x9c
    int xa0;                // 0xa0
    int xa4;                // 0xa4
    int xa8;                // 0xa8
    float scale;            // 0xac
    int xb0;                // 0xb0
    int xb4;                // 0xb4
    int xb8;                // 0xb8
    int xbc;                // 0xbc
    int xc0;                // 0xc0
    int xc4;                // 0xc4
    int xc8;                // 0xc8
    int xcc;                // 0xcc
    int xd0;                // 0xd0
    int xd4;                // 0xd4
    int xd8;                // 0xd8
    int xdc;                // 0xdc
    int xe0;                // 0xe0
    int xe4;                // 0xe4
    int xe8;                // 0xe8
    int xec;                // 0xec, unused for enemy actors (always 0). For GrYaku stage hazards, holds HurtData pointer (see GrYaku_GetHurtData at 0x800f8248)
    int xf0;                // 0xf0
    int xf4;                // 0xf4
    int xf8;                // 0xf8
    int xfc;                // 0xfc
    int x100;               // 0x100
    int x104;               // 0x104
    int x108;               // 0x108
    int x10c;               // 0x10c
    int x110;               // 0x110
    int x114;               // 0x114
    int x118;               // 0x118
    int x11c;               // 0x11c
    int x120;               // 0x120
    int x124;               // 0x124
    int x128;               // 0x128
    int x12c;               // 0x12c
    int x130;               // 0x130
    int x134;               // 0x134
    int x138;               // 0x138
    int x13c;               // 0x13c
    int x140;               // 0x140
    int x144;               // 0x144
    int x148;               // 0x148
    int x14c;               // 0x14c
    int x150;               // 0x150
    int x154;               // 0x154
    int x158;               // 0x158
    int x15c;               // 0x15c
    int x160;               // 0x160
    int x164;               // 0x164
    int x168;               // 0x168
    int x16c;               // 0x16c
    int x170;               // 0x170
    int x174;               // 0x174
    int x178;               // 0x178
    int x17c;               // 0x17c
    int x180;               // 0x180
    int x184;               // 0x184
    int x188;               // 0x188
    int x18c;               // 0x18c
    int x190;               // 0x190
    int x194;               // 0x194
    int x198;               // 0x198
    int x19c;               // 0x19c
    int x1a0;               // 0x1a0
    int x1a4;               // 0x1a4
    int x1a8;               // 0x1a8
    int x1ac;               // 0x1ac
    int x1b0;               // 0x1b0
    int x1b4;               // 0x1b4
    int x1b8;               // 0x1b8
    int x1bc;               // 0x1bc
    int x1c0;               // 0x1c0
    int x1c4;               // 0x1c4
    int x1c8;               // 0x1c8
    int x1cc;               // 0x1cc
    int x1d0;               // 0x1d0
    int x1d4;               // 0x1d4
    int x1d8;               // 0x1d8
    int x1dc;               // 0x1dc
    int x1e0;               // 0x1e0
    int x1e4;               // 0x1e4
    int x1e8;               // 0x1e8
    int x1ec;               // 0x1ec
    int x1f0;               // 0x1f0
    int x1f4;               // 0x1f4
    int x1f8;               // 0x1f8
    int x1fc;               // 0x1fc
    int x200;               // 0x200
    int x204;               // 0x204
    int x208;               // 0x208
    int x20c;               // 0x20c
    int x210;               // 0x210
    int x214;               // 0x214
    int x218;               // 0x218
    int x21c;               // 0x21c
    int x220;               // 0x220
    int x224;               // 0x224
    int x228;               // 0x228
    int x22c;               // 0x22c
    int x230;               // 0x230
    int x234;               // 0x234
    int x238;               // 0x238
    int x23c;               // 0x23c
    int x240;               // 0x240
    int x244;               // 0x244
    int x248;               // 0x248
    int x24c;               // 0x24c
    int x250;               // 0x250
    int x254;               // 0x254
    int x258;               // 0x258
    int x25c;               // 0x25c
    int x260;               // 0x260
    int x264;               // 0x264
    int x268;               // 0x268
    int x26c;               // 0x26c
    int x270;               // 0x270
    int x274;               // 0x274
    int x278;               // 0x278
    int x27c;               // 0x27c
    int x280;               // 0x280
    int x284;               // 0x284
    int x288;               // 0x288
    int x28c;               // 0x28c
    int x290;               // 0x290
    int x294;               // 0x294
    int x298;               // 0x298
    int x29c;               // 0x29c
    int x2a0;               // 0x2a0
    int x2a4;               // 0x2a4
    float anim_time_accum;  // 0x2a8
    float anim_frame_accum; // 0x2ac, animation frame accumulator
    float anim_rate;        // 0x2b0
    void *jobj_tree;        // 0x2b4, allocated JObj tree for model hierarchy
    int x2b8;               // 0x2b8
    void *alloc_2bc;        // 0x2bc, second HSD_ObjAlloc structure
    int x2c0;               // 0x2c0
    void *alloc_2c4;        // 0x2c4, third HSD_ObjAlloc structure
    float mode_scale;        // 0x2c8, scale from game mode (1.0 AR, 1.1 TR, 1.2 CT)
    float spawn_scale;       // 0x2cc, scale from descriptor (typically 1.0)
    float tier_base_scale;   // 0x2d0, base scale from actor_data (varies per tier)
    float global_enemy_scale;// 0x2d4, global enemy scale multiplier
    float final_scale;       // 0x2d8, computed: mode * spawn * tier_base * global
    float collision_scale_mult; // 0x2dc, used in collision sizing
    Vec3 accel;             // 0x2e0, acceleration. Physics proc: vel += accel each frame.
    Vec3 vel;               // 0x2ec, velocity. Physics proc: pos += vel each frame.
    Vec3 pos;               // 0x2f8
    Vec3 pos_prev;          // 0x304
    Vec3 pos_initial;       // 0x310
    Vec3 pos_attached;      // 0x31c, for child/attached actors
    int x328;               // 0x328
    int x32c;               // 0x32c
    int x330;               // 0x330
    Vec3 forward;           // 0x334, forward direction unit vector. Model facing.
    Vec3 up;                // 0x340, up direction unit vector. Surface normal for grounded.
    Vec3 right;             // 0x34c, right direction (cross of up × forward)
    int x358;               // 0x358
    int x35c;               // 0x35c
    int x360;               // 0x360
    float param_pre_header; // 0x364, bulk-copied from *actor_data-0x04. Pre-header value.
    float param_base_scale; // 0x368, from *actor_data+0x00. Base scale (also → tier_base_scale at 0x2D0).
    float param_scale_2;    // 0x36c, from *actor_data+0x04. Scale param 2.
    int param_sentinel;     // 0x370, from *actor_data+0x08. Sentinel/flag (-1).
    float param_374;        // 0x374, from *actor_data+0x0C.
    float param_detect_range; // 0x378, from *actor_data+0x10. Sight/detection range.
    float param_chase_range;  // 0x37c, from *actor_data+0x14. Chase/follow range.
    float param_move_param; // 0x380, from *actor_data+0x18. Movement parameter.
    int x384;               // 0x384
    float param_path_speed; // 0x388, from *actor_data+0x20. Path speed.
    float param_spline_walk_speed_base; // 0x38c, from *actor_data+0x24. Spline walk speed base.
    float param_speed;      // 0x390, from *actor_data+0x28. Speed parameter (scales knockback velocity).
    float param_spline_walk_speed_2; // 0x394, from *actor_data+0x2C. Spline walk speed secondary.
    float param_random_timing; // 0x398, from *actor_data+0x30. Random timing variation (× HSD_Randf()).
    float param_speed_2;    // 0x39c, from *actor_data+0x34. Speed param.
    float param_3a0;        // 0x3a0, from *actor_data+0x38.
    float param_gravity;    // 0x3a4, from *actor_data+0x3C. Gravity/fall acceleration.
    float param_3a8;        // 0x3a8, from *actor_data+0x40.
    int param_frame_count;  // 0x3ac, from *actor_data+0x44. Frame count/duration.
    float param_hp_threshold; // 0x3b0, from *actor_data+0x48. Compared against damage_accum_1 in priority 9 proc.
    int param_3b4;          // 0x3b4, from *actor_data+0x4C.
    int param_3b8;          // 0x3b8, from *actor_data+0x50. Ground check mode.
    int param_3bc;          // 0x3bc, from *actor_data+0x54.
    float param_move_speed; // 0x3c0, from *actor_data+0x58. Movement speed (passed to GroundSnap).
    Vec3 param_turn_rate;   // 0x3c4, turn rate parameters for banking/turning.
    int x3d0;               // 0x3d0
    int x3d4;               // 0x3d4
    int x3d8;               // 0x3d8
    int x3dc;               // 0x3dc
    int x3e0;               // 0x3e0
    int x3e4;               // 0x3e4
    int x3e8;               // 0x3e8
    int x3ec;               // 0x3ec
    int x3f0;               // 0x3f0
    int x3f4;               // 0x3f4
    int x3f8;               // 0x3f8
    int x3fc;               // 0x3fc
    int x400;               // 0x400
    int x404;               // 0x404
    int x408;               // 0x408
    void *per_type_params;  // 0x40c, allocated buffer, receives 16 bytes from *(actor_data+4)
    void *hurtdata;         // 0x410, HurtData pointer (hitbox/hurtbox collision)
    int x414;               // 0x414
    int x418;               // 0x418
    int x41c;               // 0x41c
    int x420;               // 0x420
    int x424;               // 0x424
    int x428;               // 0x428
    int x42c;               // 0x42c
    int x430;               // 0x430
    int x434;               // 0x434
    int x438;               // 0x438
    int x43c;               // 0x43c
    int x440;               // 0x440
    int x444;               // 0x444
    int x448;               // 0x448
    int x44c;               // 0x44c
    int x450;               // 0x450
    int x454;               // 0x454
    int x458;               // 0x458
    TriggerData trigger;    // 0x45c
    int x4bc;               // 0x4bc
    int x4c0;               // 0x4c0
    int x4c4;               // 0x4c4
    int x4c8;               // 0x4c8
    int x4cc;               // 0x4cc
    int x4d0;               // 0x4d0
    int x4d4;               // 0x4d4
    int x4d8;               // 0x4d8
    int x4dc;               // 0x4dc
    int x4e0;               // 0x4e0
    int x4e4;               // 0x4e4
    int x4e8;               // 0x4e8
    int x4ec;               // 0x4ec
    int x4f0;               // 0x4f0
    int x4f4;               // 0x4f4
    int x4f8;               // 0x4f8
    int x4fc;               // 0x4fc
    int x500;               // 0x500
    int x504;               // 0x504
    int x508;               // 0x508
    int x50c;               // 0x50c
    int x510;               // 0x510
    int x514;               // 0x514
    int x518;               // 0x518
    int x51c;               // 0x51c
    int x520;               // 0x520
    int x524;               // 0x524
    int x528;               // 0x528
    int x52c;               // 0x52c
    int x530;               // 0x530
    int x534;               // 0x534
    Vec3 anim_pos;          // 0x538, current animation-derived position (scaled by final_scale)
    Vec3 anim_pos_prev;     // 0x544, previous frame's anim_pos
    Vec3 anim_delta;        // 0x550, per-frame animation position delta (anim_pos - anim_pos_prev)
    int x55c;               // 0x55c
    int x560;               // 0x560
    int x564;               // 0x564
    int x568;               // 0x568
    int x56c;               // 0x56c
    int x570;               // 0x570
    int x574;               // 0x574
    int x578;               // 0x578
    int x57c;               // 0x57c
    int x580;               // 0x580
    int x584;               // 0x584
    int x588;               // 0x588
    int x58c;               // 0x58c
    int x590;               // 0x590
    void *map_collision;    // 0x594, map collision object for ground detection (mpColl)
    int x598;               // 0x598
    int x59c;               // 0x59c
    int x5a0;               // 0x5a0
    int x5a4;               // 0x5a4
    int x5a8;               // 0x5a8
    int x5ac;               // 0x5ac
    int x5b0;               // 0x5b0
    int x5b4;               // 0x5b4
    int x5b8;               // 0x5b8
    int x5bc;               // 0x5bc
    int x5c0;               // 0x5c0
    float ground_height;    // 0x5c4, height from ground
    Vec3 ground_normal;     // 0x5c8, ground surface normal
    void *spline_primary;   // 0x5d4, primary spline curve pointer (forward or backward depending on direction)
    void *spline_secondary; // 0x5d8, secondary spline curve pointer
    int spline_segment;     // 0x5dc, index into stage spline array
    int x5e0;               // 0x5e0
    int x5e4;               // 0x5e4
    int x5e8;               // 0x5e8
    int x5ec;               // 0x5ec
    int x5f0;               // 0x5f0
    int x5f4;               // 0x5f4
    int spline_direction;   // 0x5f8, 1=forward, else backward. Determines primary/secondary spline assignment.
    float spline_arc_param; // 0x5fc, current position along spline (arc-length parameter)
    int x600;               // 0x600
    int x604;               // 0x604
    int x608;               // 0x608
    int x60c;               // 0x60c
    int x610;               // 0x610
    int x614;               // 0x614
    int x618;               // 0x618
    int x61c;               // 0x61c
    int x620;               // 0x620
    int x624;               // 0x624
    int x628;               // 0x628
    int x62c;               // 0x62c
    int x630;               // 0x630
    int x634;               // 0x634
    int x638;               // 0x638
    int x63c;               // 0x63c
    int x640;               // 0x640
    int x644;               // 0x644
    int x648;               // 0x648
    int x64c;               // 0x64c
    int x650;               // 0x650
    int spline_path_ready;  // 0x654, set to 1 before EnemyPath_Init; copied to spline_direction
    Vec3 saved_up_normal;   // 0x658, fallback up-normal for path following (used when move_direction is zero)
    Vec3 move_direction;    // 0x664, computed movement direction from path following
    int x670;               // 0x670
    int x674;               // 0x674
    int x678;               // 0x678
    int x67c;               // 0x67c
    int x680;               // 0x680
    int x684;               // 0x684
    int x688;               // 0x688
    int x68c;               // 0x68c
    int x690;               // 0x690
    int x694;               // 0x694
    int x698;               // 0x698
    int x69c;               // 0x69c
    int x6a0;               // 0x6a0
    int x6a4;               // 0x6a4
    int x6a8;               // 0x6a8
    int x6ac;               // 0x6ac
    int x6b0;               // 0x6b0
    int x6b4;               // 0x6b4
    int x6b8;               // 0x6b8
    int x6bc;               // 0x6bc
    int x6c0;               // 0x6c0
    int x6c4;               // 0x6c4
    int x6c8;               // 0x6c8
    int x6cc;               // 0x6cc
    int x6d0;               // 0x6d0
    int x6d4;               // 0x6d4
    int x6d8;               // 0x6d8
    int x6dc;               // 0x6dc
    int x6e0;               // 0x6e0
    int x6e4;               // 0x6e4
    int x6e8;               // 0x6e8
    int x6ec;               // 0x6ec
    int x6f0;               // 0x6f0
    int x6f4;               // 0x6f4
    int x6f8;               // 0x6f8
    int x6fc;               // 0x6fc
    int x700;               // 0x700
    int x704;               // 0x704
    int x708;               // 0x708
    int x70c;               // 0x70c
    int x710;               // 0x710
    int x714;               // 0x714
    int x718;               // 0x718
    int x71c;               // 0x71c
    int x720;               // 0x720
    int x724;               // 0x724
    int x728;               // 0x728
    int x72c;               // 0x72c
    int x730;               // 0x730
    int x734;               // 0x734
    int x738;               // 0x738
    int x73c;               // 0x73c
    int x740;               // 0x740
    int x744;               // 0x744
    int x748;               // 0x748
    int x74c;               // 0x74c
    int x750;               // 0x750
    int x754;               // 0x754
    int x758;               // 0x758
    int x75c;               // 0x75c
    int x760;               // 0x760
    int x764;               // 0x764
    int x768;               // 0x768
    int x76c;               // 0x76c
    int x770;               // 0x770
    int x774;               // 0x774
    int x778;               // 0x778
    int x77c;               // 0x77c
    int x780;               // 0x780
    int x784;               // 0x784
    int x788;               // 0x788
    int x78c;               // 0x78c
    int x790;               // 0x790
    int x794;               // 0x794
    int x798;               // 0x798
    int x79c;               // 0x79c
    int x7a0;               // 0x7a0
    int x7a4;               // 0x7a4
    int x7a8;               // 0x7a8
    int x7ac;               // 0x7ac
    int x7b0;               // 0x7b0
    int x7b4;               // 0x7b4
    int x7b8;               // 0x7b8
    int x7bc;               // 0x7bc
    int x7c0;               // 0x7c0
    int x7c4;               // 0x7c4
    int x7c8;               // 0x7c8
    int x7cc;               // 0x7cc
    int x7d0;               // 0x7d0
    int x7d4;               // 0x7d4
    int x7d8;               // 0x7d8
    int x7dc;               // 0x7dc
    int x7e0;               // 0x7e0
    int x7e4;               // 0x7e4
    int x7e8;               // 0x7e8
    int x7ec;               // 0x7ec
    int x7f0;               // 0x7f0
    int x7f4;               // 0x7f4
    int x7f8;               // 0x7f8
    int x7fc;               // 0x7fc
    int x800;               // 0x800
    int x804;               // 0x804
    int x808;               // 0x808
    int x80c;               // 0x80c
    int x810;               // 0x810
    int x814;               // 0x814
    int x818;               // 0x818
    int x81c;               // 0x81c
    int x820;               // 0x820
    int x824;               // 0x824
    int x828;               // 0x828
    int x82c;               // 0x82c
    int x830;               // 0x830
    int x834;               // 0x834
    int x838;               // 0x838
    int x83c;               // 0x83c
    int x840;               // 0x840
    int x844;               // 0x844
    int x848;               // 0x848
    int x84c;               // 0x84c
    int x850;               // 0x850
    int x854;               // 0x854
    int x858;               // 0x858
    int x85c;               // 0x85c
    int x860;               // 0x860
    float height_interp_target; // 0x864, target height for smooth terrain following (lerp 0.2/frame)
    float height_interp_current; // 0x868, current interpolated height
    int x86c;               // 0x86c
    int x870;               // 0x870
    int x874;               // 0x874
    float kb_speed_mult;    // 0x878, knockback speed multiplier (scales knockback velocity)
    int kb_active;           // 0x87c, set to 1 when entering knockback state (Enemy_ApplyKnockback)
    int ground_warmup;      // 0x880, 2-frame warmup counter for ground physics (skips first 2 frames after spawn)
    int x884;               // 0x884
    int recovery_timer;     // 0x888, recovery countdown during knockback state 0x0B
    int launch_spline_id;   // 0x88c, spline ID for launched trajectory (state 0x0C)
    int x890;               // 0x890
    int x894;               // 0x894
    int x898;               // 0x898
    int x89c;               // 0x89c
    int x8a0;               // 0x8a0
    int x8a4;               // 0x8a4
    float launch_time_accum; // 0x8a8, accumulated time along launch trajectory
    float launch_time_step; // 0x8ac, time step per frame for launch trajectory
    int x8b0;               // 0x8b0
    int x8b4;               // 0x8b4
    int x8b8;               // 0x8b8
    int x8bc;               // 0x8bc
    int x8c0;               // 0x8c0
    int x8c4;               // 0x8c4
    int x8c8;               // 0x8c8
    int x8cc;               // 0x8cc
    int x8d0;               // 0x8d0
    int x8d4;               // 0x8d4
    int x8d8;               // 0x8d8
    int x8dc;               // 0x8dc
    int x8e0;               // 0x8e0
    int x8e4;               // 0x8e4
    int grounded_timer;     // 0x8e8, grounded state entry timer (set to 10 on landing)
    int slide_timer;        // 0x8ec, sliding state friction timer
    Vec3 bounce_vel;        // 0x8f0, bounce velocity during launched/grounded states (decays over time)
    int x8fc;               // 0x8fc
    int x900;               // 0x900
    int x904;               // 0x904
    int grounded_active;    // 0x908, 1 = grounded/active state (affects combat AI movement mode)
    int x90c;               // 0x90c
    int x910;               // 0x910
    int x914;               // 0x914
    int x918;               // 0x918
    int x91c;               // 0x91c
    int x920;               // 0x920
    int x924;               // 0x924
    int x928;               // 0x928
    int x92c;               // 0x92c
    int x930;               // 0x930
    int x934;               // 0x934
    int x938;               // 0x938
    int x93c;               // 0x93c
    int x940;               // 0x940
    int x944;               // 0x944
    int x948;               // 0x948
    int x94c;               // 0x94c
    GOBJ *hit_source_gobj;  // 0x950, GObj of the entity that hit this enemy (for knockback direction)
    void *shadow;           // 0x954, shadow object pointer
    void *shadow_2;         // 0x958, secondary shadow pointer
    float slope_factor;     // 0x95c, accumulated slope factor for ground physics projection
    int x960;               // 0x960
    float movement_speed;   // 0x964, if 0.0, AI physics tick returns immediately (enemy stationary)
    int x968;               // 0x968
    int x96c;               // 0x96c
    int x970;               // 0x970
    float idle_wander_speed; // 0x974, used for airborne/idle computation in AI physics
    int x978;               // 0x978
    int x97c;               // 0x97c
    int x980;               // 0x980
    int x984;               // 0x984
    int x988;               // 0x988
    int x98c;               // 0x98c
    int x990;               // 0x990
    int damage_accum_1;     // 0x994, damage accumulator (capped at 9999). Not used for death — cosmetic only.
    int damage_accum_2;     // 0x998, secondary damage accumulator (capped at 9999)
    int kb_source_kind;      // 0x99c, damage source type for knockback (0=normal, 3=enemy-on-enemy, 5=special)
    int x9a0;               // 0x9a0
    int x9a4;               // 0x9a4
    int x9a8;               // 0x9a8
    int x9ac;               // 0x9ac
    int x9b0;               // 0x9b0
    int x9b4;               // 0x9b4
    int x9b8;               // 0x9b8
    int x9bc;               // 0x9bc
    int death_sfx_id;       // 0x9c0, SFX to play on death (-1 = none)
    int death_vfx_id;       // 0x9c4, VFX to spawn on death (-1 = none)
    int death_frame_counter; // 0x9c8, death state frame counter. Set to 600 on death entry, incremented each frame; destroyed when > 120. Also used as stun spark frame counter.
    Vec3 kb_dir;            // 0x9cc, normalized knockback direction
    float kb_launch_speed;  // 0x9d8, launch speed from enemy param table per tier (+0x50)
    Vec3 kb_start_pos;      // 0x9dc, enemy position saved at knockback start
    int x9e8;               // 0x9e8
    int x9ec;               // 0x9ec
    int x9f0;               // 0x9f0
    int x9f4;               // 0x9f4
    int x9f8;               // 0x9f8
    int x9fc;               // 0x9fc
    Vec3 kb_velocity;       // 0xa00, randomized knockback velocity (sign bits from HSD_Randi(8))
    GOBJ *kb_attacker_gobj; // 0xa0c, attacker's rider GObj (for direction computation)
    int attraction_mode;    // 0xa10, inhale/attraction mode (0=normal/attracted, 1=captured, 5=skip)
    int attraction_target;  // 0xa14, target player index for inhale attraction
    int stun_frames;        // 0xa18, frames remaining in stun/knockback. Decrements each frame; death state at 0.
    int knockback_tier;     // 0xa1c, response tier (0-3) based on per-hit damage thresholds (<10, <21, <32, >=32)
    int xa20;               // 0xa20
    int kb_attacker_entity;  // 0xa24, attacker entity pointer (for enemy-on-enemy knockback direction)
    int death_timer;        // 0xa28, counts up during death processing. Used by func2 to track death frame count.
    float anim_speed_scale; // 0xa2c, animation speed scale factor (decays each frame toward minimum)
    int xa30;               // 0xa30
    int xa34;               // 0xa34
    int xa38;               // 0xa38
    int vfx_handle_1;       // 0xa3c, effect handle. Cleaned by EventActor_CleanupVfxA3C (calls sound stop at 0x80236358). Despite the name, these are SFX handles.
    int vfx_handle_2;       // 0xa40, second effect handle. Cleaned by EventActor_CleanupVfxA40.
    int xa44;               // 0xa44
    int xa48;               // 0xa48
    int xa4c;               // 0xa4c
    int xa50;               // 0xa50
    int xa54;               // 0xa54
    int sfx_handle_1;       // 0xa58, sound effect handle
    int sfx_handle_2;       // 0xa5c, second SFX handle
    int sfx_state_1;        // 0xa60, SFX state (initialized to -1)
    int sfx_state_2;        // 0xa64, SFX state (initialized to -1)
    int sfx_handle_3;       // 0xa68, third SFX handle
    int xa6c;               // 0xa6c
    int hit_vfx_1;          // 0xa70, impact VFX handle (stored by Meteor_HitTransition)
    int hit_vfx_2;          // 0xa74
    int damage_frame_counter; // 0xa78, damage state tracking counter (proc 21)
    int xa7c;               // 0xa7c
    int xa80;               // 0xa80
    int xa84;               // 0xa84
    int xa88;               // 0xa88
    float path_active_flag; // 0xa8c, -1.0 = path-following enabled; checked by movement update
    int xa90;               // 0xa90
    int xa94;               // 0xa94
    int xa98;               // 0xa98
    int xa9c;               // 0xa9c
    int xaa0;               // 0xaa0
    int xaa4;               // 0xaa4
    int xaa8;               // 0xaa8
    int xaac;               // 0xaac
    int xab0;               // 0xab0
    int xab4;               // 0xab4
    void *state_func1;      // 0xab8, per-state callback from priority 1 (ProcUpdate)
    void *state_func2;      // 0xabc, per-state callback from priority 4 (pre-physics)
    void *state_func3;      // 0xac0, per-state callback from priority 5 (state active)
    void *state_func4;      // 0xac4, per-state callback from priority 6 (shared + model)
    void *per_type_cb;      // 0xac8, per-type callback. Reset to 0 on state change. Called from priority 7.
    void *hit_reaction_cb1; // 0xacc, hit reaction callback. Set by init callback. Called from damage proc.
    void *hit_reaction_cb2; // 0xad0, hit reaction callback. Called from priority 10 when damage > threshold. If null, default knockback handler runs.
    int xad4;               // 0xad4, cleared on state change (unless flag 0x10)
    int xad8;               // 0xad8
    int xadc;               // 0xadc
    void *grounded_callback; // 0xae0, called when landing from knockback (states 0x0C/0x0D). If null, actor is destroyed instead.
    int xae4;               // 0xae4
    int xae8;               // 0xae8
    void *custom_death_callback; // 0xaec, if set, replaces default death behavior in func2
    int xaf0;               // 0xaf0
    int xaf4;               // 0xaf4
    int xaf8;               // 0xaf8
    int xafc;               // 0xafc
    int xb00;               // 0xb00
    int xb04;               // 0xb04
    int render_flags;        // 0xb08, byte-accessed multi-purpose flags:
                             // Byte 0 (+0xB08): bits 0-1=ground_state (0=air, 1=transitioning, 2=grounded), bit 4=rendering disabled, bit 7=invisible.
                             // Byte 1 (+0xB09): bit 0=height_interp_enabled, bit 2=ground_contact for turning, bits 5-6=prev frame ground state.
                             // Byte 2 (+0xB0A): bit 2=no-spline/force-kill, bits 5-6=knockback sub-state mode.
                             // Byte 3 (+0xB0B): bit 3=grounded bounce flag, bit 4=shadow visibility, bit 5=shadow active.
    int xb0c;               // 0xb0c
    float inhale_distance;  // 0xb10, distance to inhaling rider (state 0x0A)
    float shadow_base_scale; // 0xb14, shadow size computation base. Also used as inhale scale during state 0x0A.
    int xb18;               // 0xb18
    int suction_active;     // 0xb1c, 1 = being sucked in by rider (state 0x0A)
    int inhale_timer;       // 0xb20, frames since inhale started. Actor destroyed when > 120.
    short target_player_idx; // 0xb24, targeted player index (-1 = none). Set by EnemyActor_FindNearestPlayer.
    short retarget_cooldown; // 0xb26, frames until re-evaluation
    float chase_flag;       // 0xb28, 0.0 = chase active
    int xb2c;               // 0xb2c
    int xb30;               // 0xb30
    int xb34;               // 0xb34
    Vec3 chase_direction;   // 0xb38, normalized direction toward target player
    int xb44;               // 0xb44
    short xb48;              // 0xb48
    short frame_counter;     // 0xb4a, state frame counter (s16). Incremented by state func3. Used for timeouts.
    short in_bounds_flag;    // 0xb4c, set to 1 when meteor enters map bounds (state 14)
    short camera_flag;       // 0xb4e, set to 1 when camera effect triggered
    Vec3 initial_pos;       // 0xb50, saved by post-init callback (pos at creation time, before state transitions or spline snaps)
    float zone_offset;      // 0xb5c, height offset from zone table (meteor)
    int landing_vfx_1;      // 0xb60, landing VFX handle (stored by Meteor_Landing, monitored by state 16 func3)
    int landing_vfx_2;      // 0xb64
    Vec3 collision_radii;   // 0xb68, base collision sphere radii (from actor_data)
    void *collision_sphere;  // 0xb74, xB74 collision sphere handle. Created by HitTransition/Meteor_Landing.
    void *collision_sphere_2;// 0xb78
    int xb7c;               // 0xb7c
    int xb80;               // 0xb80
    int xb84;               // 0xb84
    int xb88;               // 0xb88
    int xb8c;               // 0xb8c
    int xb90;               // 0xb90
    int xb94;               // 0xb94
    int xb98;               // 0xb98
    int xb9c;               // 0xb9c
    int xba0;               // 0xba0
    int xba4;               // 0xba4
    int xba8;               // 0xba8
    int xbac;               // 0xbac
    int xbb0;               // 0xbb0
    int xbb4;               // 0xbb4
    int xbb8;               // 0xbb8
    int xbbc;               // 0xbbc
} EnemyData;

// Enemy data table at 0x804b22b4 (stride 8). Maps enemy ID → {data_index (int), flags (int)}.
// Data index used by Enemy_LoadFile to load archive files.

// Enemy manager functions
void Enemy_LoadStageEnemies(void); // 0x800f25b4, iterates stage enemy list, calls Enemy_CheckAndLoad per ID. Skips only in City Trial Free Run (Major==MJRKIND_CITY && Gm_GetCityMode()==CITYMODE_FREERUN); runs normally in timed City Trial.
short *Enemy_GetStagesEnemies(int stage_kind); // 0x80262808, returns short* array of enemy IDs for stage (terminated by -1)
void Enemy_InitPositionData(void); // 0x800f2634, allocates enemy position slots, loads positions from stage data
void Enemy_InitSpawner(void); // 0x800f2ee4, creates enemy manager GObj with Enemy_Think proc
void Enemy_Think(void); // 0x800f3904, GObj proc callback for enemy manager (Air Ride)
void Enemy_CityTrialThink(void); // 0x800f33c0, GObj proc callback for enemy manager (City Trial)

// Archive loading — idempotent, safe to call multiple times
void Enemy_CheckAndLoad(int actor_id); // 0x801fd060, validates actor ID, calls Enemy_LoadFile
void Enemy_LoadFile(int actor_id); // 0x801fd348, loads enemy archive data from disc. No-ops if already loaded.

// Spawning
void Enemy_SpawnActor(int spawn_slot, int enemy_id_packed, int position_index); // 0x800f13a8, spawn-slot wrapper: builds descriptor and calls EventActor_Create. enemy_id_packed = (variant << 8) | enemy_id. Pass -1 to skip creation.
GOBJ *EventActor_Create(void *desc); // 0x801fbb50, universal actor factory. Creates GOBJ for any ActorID (0x00–0x4E). desc points to an EventActorDesc struct. Returns GOBJ* (0 on failure).
void EventActor_Destroy(GOBJ *gobj); // 0x801fbf2c, proper actor destruction. Recursively destroys child/attached actors, clears inter-actor references, runs cleanup, then calls GObj_Destroy. Use this instead of raw GObj_Destroy.
void EventActor_CleanupCollisionSphere(EnemyData *ed); // 0x8021f1bc, destroys xB74 collision sphere if non-null and nulls it.
void EventActor_CleanupVfxA3C(EnemyData *ed); // 0x8020c6e0, destroys VFX handle at xa3c if != -1.
void EventActor_CleanupVfxA40(EnemyData *ed); // 0x8020c70c, destroys VFX handle at xa40 if != -1.
void EventActor_Hide(EnemyData *ed); // 0x801fed40, sets bit 7 (invisible) of render_flags (+0xB08), then calls EventActor_DisableRendering. Full hide.
void EventActor_SetVisibility(EnemyData *ed); // 0x801fed74, clears bit 7 (invisible) of render_flags (+0xB08), then calls EnableRendering (actor_id < 0x4C) or DisableRendering (>= 0x4C). For actor_id >= 0x4C (e.g. meteor 0x4E), this leaves rendering DISABLED. Used by per-type idle func1 to make actor visible after init.
void EventActor_EnableRendering(GOBJ *gobj); // 0x80204198, clears bit 4 of render_flags (+0xB08).
void EventActor_DisableRendering(GOBJ *gobj); // 0x802041b0, sets bit 4 of render_flags (+0xB08).
// Note: render_flags bits and JOBJ_HIDDEN are independent. For actors with id >= 0x4C,
// clearing render_flags alone is insufficient — must also JObj_ClearFlagsAll(jobj, JOBJ_HIDDEN).
double EventActor_GetParentScale(GOBJ *parent_gobj); // 0x802049b8, reads parent_gobj->userdata + 0x2B0 (scale). Crashes if parent_gobj is null.
int Gm_CheckEnemyEnabled(void); // 0x8000a348, returns 1 if enemy spawning is enabled

// State machine
void EnemyStateChange(EnemyData *ed, int state_id, int flags, float anim_rate, float anim_end_frame); // 0x801fc398, transitions enemy to new behavioral state. flags bitmask: 0x01=skip anim setup, 0x02=skip anim reset if same, 0x04=skip cleanup, 0x08=save/restore pos, 0x10=keep per-type cb, 0x20=skip HurtData reset, 0x40=skip SFX cleanup. anim_rate/anim_end_frame passed in f1/f2.

// Meteor-specific
void Meteor_BehaviorInit(EnemyData *ed); // 0x8021e1a0, DISABLES RENDERING (calls DisableRendering + sets JOBJ_HIDDEN), then zeros velocity, enters state 15, sets vel from zone/speed tables. Reads stc_meteor_event_data — must point to valid data. Caller must re-enable rendering + clear JOBJ_HIDDEN after.
void Meteor_Landing(EnemyData *ed); // 0x8021ea5c, WARNING: transitions to state 17 which does NOT exist in the meteor state table — calling this causes out-of-bounds read and memory corruption. Dead code in vanilla (only reachable from state 15 timeout, which is never entered via normal creation). Do not call.
void Meteor_HitTransition(EnemyData *ed); // 0x8021e7c4, state 14 hit handler: normalizes velocity, computes impact speed from actor_data, enters state 16, creates impact VFX (0x5A5A2) and audio fade. State 16 waits for effects to finish then calls EventActor_Destroy.

// Enemy-player interaction
float EnemyActor_DistToPlayer(int player_idx, float *pos); // 0x801fffa4, returns 3D distance from pos to player player_idx. Result in f1.
void EnemyActor_RumblePlayer(int player_idx, int intensity, int duration); // 0x801ff80c, triggers controller rumble for a player. Gets rider GObj, then calls rumble with (controller_idx, 2, intensity, duration). NOT a damage function — actual enemy damage flows through the HitColl collision pipeline.

// Path-following initialization
void EnemyPath_Init(EnemyData *ed); // 0x80206e2c, finds nearest spline to ed->pos and assigns spline_primary/secondary. Sets spline_segment and spline_arc_param. If no spline found, sets bit 2 of +0xB0A instead.

// Actor data lookup
void *Enemy_GetActorData(int actor_id); // 0x801fd498, returns actor_data pointer for loaded archive. Indexes by {data_index, flags} from table at 0x804b22b4. Returns 0 if archive not loaded.

// GObj proc functions (registered by EventActor_Create — 10 procs unconditionally, at priorities 0/1/4/5/6/7/8/9/10/21; plus a separate GXLink render callback zz_801fd158_ at priority 9)
void EventActor_ProcResetDamage(GOBJ *gobj); // 0x801fc670, priority 0: zeros per-frame damage via HurtData_ResetPerFrame
void EventActor_ProcUpdate(GOBJ *gobj); // 0x801fc698, priority 1: HSD anim advance + state machine + state_func1 dispatch
void EnemyPhysicsProc(GOBJ *gobj); // 0x801fc6fc, priority 4: state_func2 dispatch + vel += accel, pos += vel, OOB floor kill (skipped for actor_id >= 0x4C)
void EventActor_ProcStateActive(GOBJ *gobj); // 0x801fc7c4, priority 5: state_func3 dispatch (main per-state AI logic)
void EventActor_ProcSharedModel(GOBJ *gobj); // 0x801fc7f8, priority 6: shadow update + state_func4 dispatch + SharedUpdate
void EventActor_ProcPerType(GOBJ *gobj); // 0x801fc848, priority 7: per_type_cb dispatch + HurtData update + position snap
// Priority 8: EventActor_ProcHitCollInit (0x801fc8e8) — single blr, no-op stub
void EventActor_ProcHitColl(GOBJ *gobj); // 0x801fc8ec, priority 9: HitColl processing + collision checks
void EventActor_ProcDamage(GOBJ *gobj); // 0x801fc9f0, priority 10: reads HurtData output, calls giveEnemyDamage, dispatches hit_reaction_cb2
void EventActor_ProcFinal(GOBJ *gobj); // 0x801fcabc, priority 21: pos → pos_prev, ground state flags, lifetime/despawn, OOB destroy

// Internal creation/destruction helpers
void EventActor_InitFromDesc(EnemyData *ed, void *desc); // 0x801fb53c, copies EventActorDesc fields into EnemyData (0x4fc bytes)
void EventActor_SpawnChild(EnemyData *parent_ed); // 0x801fcda0, spawns child actor (rider/attached entity) and links parent↔child GOBJs
void EventActor_StateCleanup(EnemyData *ed); // 0x801fe110, cleans up attach slots (ed+0x918), detaches/destroys orphaned child objects
void EventActor_SharedUpdate(GOBJ *gobj); // 0x801fd780, updates JObj world matrix from position/orientation/scale
void EventActor_UpdateFacing(EnemyData *ed); // 0x801fd7bc, recalculates forward/up/right axes from orientation
void EventActor_ShadowInit(EnemyData *ed); // 0x80200208, shadow and 3D model initialization/update
void EventActor_FinalizeInit(EnemyData *ed); // 0x802042fc, finalize init — sets up animation, collision, model visibility
void EventActor_OnCapture(EnemyData *ed); // 0x802038c4, called when enemy is captured/inhaled by a rider
void EventActor_HurtDataCreate(EnemyData *ed); // 0x80201ee8, creates HurtData for the actor (hitbox/hurtbox collision setup)
void EventActor_FollowParent(EnemyData *ed); // 0x80219eec, child actor state: syncs position/scale from parent_gobj
void EventActor_CopyParentState(EnemyData *ed); // 0x80219fd4, copies parent's position/orientation/forward/up to child
void EventActor_GObjDestroyHandler(GOBJ *gobj); // 0x801fcca0, GObj destructor callback (cleanup on GObj_Destroy)
void EventActor_StateMachine(EnemyData *ed); // 0x802017d0, animation script bytecode processor (commands 0-10 builtin, 11+ enemy-specific)
void Enemy_UnregisterFromSpawnSlot(GOBJ *gobj); // 0x800f3b28, removes enemy from spawn slot tracking, decrements active count, sets respawn timer
void Enemy_CopyParamBlock(EnemyData *ed); // 0x802006b4, bulk-copies 0xA4 bytes from *actor_data-4 into ed+0x364 through ed+0x408

// Damage system
void giveEnemyDamage(EnemyData *ed, float damage); // 0x8020b680, adds damage to accumulators (capped at 9999). Cosmetic only — does not cause death.
int Enemy_ClassifyDamageTier(float damage); // 0x8020b740, classifies damage into tier 0-3 based on global thresholds
float Enemy_ScaleDamage(float damage); // 0x8020b71c, scales damage by global multiplier from enemy param table
void Enemy_ApplyKnockback(EnemyData *ed, void *hurtdata, int mode); // 0x8020b784, full knockback state transition: sets stun_frames, computes velocity, enters knockback state

// Ground physics
void Enemy_GroundPhysicsVelocity(EnemyData *ed); // 0x80209104, velocity-based ground projection with raycast
void Enemy_GroundPhysicsSurface(EnemyData *ed); // 0x802096b4, direct surface advancement with wall bounce
void Enemy_GroundAttach(EnemyData *ed); // 0x8020a664, final ground attachment after path following
int Enemy_CheckPathFollow(EnemyData *ed); // 0x8020b01c, checks if enemy should follow path (spline)
void Enemy_SetTerrainLocked(EnemyData *ed); // 0x8020ae54, sets terrain-locked flag (used by Wheelie, Gordo)
void EnemyPath_Advance(EnemyData *ed, Vec3 *input_pos, Vec3 *output_pos, float speed); // 0x8020a040, advances parametric position along spline path

// Common state callbacks (shared by states 0x00-0x0D)
void EnemyState_AnimEnter(EnemyData *ed); // 0x8020bd68, func1 for states 0-8: animation rate scaling, stun freeze
void EnemyState_AnimTick(EnemyData *ed); // 0x8020be1c, func2 for states 0-8: death processing, attraction physics, inhale follow
void EnemyState_AnimExit(EnemyData *ed); // 0x8020c558, func3 for states 0-8: stun frame decrement, ground physics, stun spark VFX
void EnemyState_DeathEnter(EnemyData *ed); // 0x80203e60, func1 for state 0x09: death effects, model hide, 600-frame timer → destroy
void EnemyState_KnockbackEnter(EnemyData *ed); // 0x8020ddb4, func1 for state 0x0B: compute knockback velocity, set launch trajectory
void EnemyState_TransitionToLaunched(EnemyData *ed); // 0x8020e0bc, enters state 0x0C (launched/airborne)
void EnemyState_TransitionToSliding(EnemyData *ed); // 0x8020e63c, enters state 0x0D (grounded/sliding)

// Movement and physics
void EventActor_ZeroVelocity(EnemyData *ed); // 0x801fd6b0, zeros accel (0x2E0) and vel (0x2EC)
void EventActor_GroundSnap(EnemyData *ed, float scale); // 0x80204fac, raycasts downward, snaps to ground, updates ground_normal
void EventActor_UpdateOrientation(EnemyData *ed); // 0x802054e4, recalculates facing/orientation vectors
int Enemy_AIPhysicsTick(EnemyData *ed); // 0x802081ec, central ground-following movement. Returns 0=moved, 1=stationary.
void EnemyPath_FollowUpdate(EnemyData *ed, int direction_mode); // 0x80209ce4, spline path-following movement update

// Shared AI helpers (used by Sword Knight chase and other combat enemies)
void EnemyActor_CombatMovement(EnemyData *ed); // 0x8020b490, combat movement: AIPhysicsTick when ed+0x908==0, accel-based chase when ed+0x908==1
void EnemyActor_CombatAI(EnemyData *ed); // 0x802069e8, combat AI: targeting when ed+0x908==0, proximity/range checks when ed+0x908==1
int EnemyActor_GroundFollowMovement(EnemyData *ed); // 0x80208bd4, ground-following chase physics: normalizes orientation, computes movement speed, terrain raycasting, ground-snap. Returns 0=moved, 1=stationary.

// Player targeting
void EnemyActor_FindNearestPlayer(EnemyData *ed); // 0x801ffd78, finds nearest player, sets target_player_idx/chase_direction
void EnemyActor_FindNearestPlayerFOV(EnemyData *ed); // 0x801ff8d8, targeting with forward-cone FOV check

// Common state table data address (14 entries, 0x14 bytes each, states 0x00-0x0D)
#define stc_common_state_table (*(void **)0x804b2950) // 0x804b2950

// Per-type descriptor table (79 pointers indexed by ActorID)
#define stc_per_type_descriptor_table ((void **)0x804b1d98) // 0x804b1d98

// Actor data table: {data_index (int), flags (int)} per ActorID, stride 8
#define stc_actor_data_table ((int *)0x804b22b4) // 0x804b22b4

// Archive loaded flags: byte per data_index (22 entries), 1 = loaded
#define stc_archive_loaded_flags ((char *)0x8055a210) // 0x8055a210

// Archive root pointers: pointer per data_index (22 entries)
#define stc_archive_root_pointers ((void **)0x8055a228) // 0x8055a228

// Per-stage enemy spawn data, pointed to by r13 + 0x630.
// NULL when the per-stage "enemies enabled" flag (GameData+0xAA6 bit 4) is off
// (City Trial city map, Top Ride, and stadiums without enemies like Air Glider /
// Destruction Derby / Single Race). Populated by Enemy_InitPositionData.
//
// secondary_table is a pointer-array indexed by meta-enemy ID (0x50-0x5E offset
// by -0x50). Each entry points to a sub-table of {enemy_id, weight} short pairs,
// -1 terminated. May be NULL.
//
// Per-entry layout differs by config.mode and is parsed as raw bytes off
// spawn_entries (stride 0x38):
//   mode 1 (Air Ride courses):    short ids[4] at +0x1E, short weights[4] at +0x26
//   mode 2 (STKIND_MELEE1):       short enemy_id at +0x06, short weight_columns[N] at +0x08
//                                 (two-stage: meta-enemy category from secondary[0], then
//                                  individual enemy from that column)
//   mode 3 (STKIND_MELEE2):       short ids[5] at +0x06, short weights[5] at +0x10
typedef struct EnemySpawnConfig
{
    char x00[0x28];     // 0x00, layout TBD
    short mode;         // 0x28, 1=Air Ride, 2=STKIND_MELEE1, 3=STKIND_MELEE2
} EnemySpawnConfig;

typedef struct EnemySpawnData
{
    short spawn_count;          // 0x00, number of entries in spawn_entries
    short pad02;                // 0x02
    char *spawn_entries;        // 0x04, primary spawn table (stride 0x38)
    int x08;                    // 0x08
    int **secondary_table;      // 0x0C, pointer-array of meta-enemy sub-tables (may be NULL)
    EnemySpawnConfig *config;   // 0x10
} EnemySpawnData;

static EnemySpawnData **stc_enemy_spawn_data = (EnemySpawnData **)(0x805dd0e0 + 0x630);

// Enemy global parameter table (detection range, retarget cooldown, knockback thresholds)
#define stc_enemy_param_table ((void *)0x805dd878) // 0x805dd878

// HSD spline functions — used by actor movement/path-following systems
float splArcLengthGetParameter(void *spline); // 0x80415758, returns arc-length parameter in f1
void splGetSplinePoint(Vec3 *output, void *spline, float param); // 0x80414fc0, evaluates spline at param. param passed in f1 (first float arg). Crashes if spline is null.
void splArcLengthPoint(Vec3 *output, void *spline); // 0x80415958, wrapper: calls splArcLengthGetParameter then splGetSplinePoint. Crashes if spline is null.

#endif