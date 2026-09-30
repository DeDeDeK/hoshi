#ifndef KAR_H_ENEMY
#define KAR_H_ENEMY

#include "datatypes.h"
#include "trigger.h"
#include "hurt.h"

// Kinds for EventActor_Create: every enemy and event actor. The table at
// 0x804b22b4 maps each kind to {data_index, flags}.
typedef enum EnemyKind
{
    // Tier 0 - Base enemies (flags=0)
    ENEMYKIND_BROOM_HATTER       = 0x00,
    ENEMYKIND_BROOM_HATTER_B     = 0x01,
    ENEMYKIND_BRONTO_BURT        = 0x02,
    ENEMYKIND_BRONTO_BURT_B      = 0x03,
    ENEMYKIND_SCARFY             = 0x04,
    ENEMYKIND_SWORD_KNIGHT       = 0x05,
    ENEMYKIND_CAPPY              = 0x06,
    ENEMYKIND_CAPPY_B            = 0x07,
    ENEMYKIND_WHEELIE            = 0x08,
    ENEMYKIND_PHAN_PHAN          = 0x09,
    ENEMYKIND_NODDY              = 0x0A,
    ENEMYKIND_CHILLY             = 0x0B,
    ENEMYKIND_FLAPPY             = 0x0C,
    ENEMYKIND_PLASMA_WISP        = 0x0D,
    ENEMYKIND_GORDO              = 0x0E,
    ENEMYKIND_BOMBER             = 0x0F,
    ENEMYKIND_PICHIKURI          = 0x10,
    ENEMYKIND_PICHIKURI_B        = 0x11,
    ENEMYKIND_DAYL               = 0x12,
    ENEMYKIND_DAYL_B             = 0x13,
    ENEMYKIND_CALLER             = 0x14, // Shaturn / Tornado caller
    ENEMYKIND_WALKY              = 0x15,
    ENEMYKIND_WADDLE_DEE_TRUCK   = 0x16,
    ENEMYKIND_WADDLE_DEE         = 0x17,

    // Tier 1 - Enhanced variants (flags=1), same data_index sequence
    ENEMYKIND_T1_BROOM_HATTER       = 0x18,
    ENEMYKIND_T1_BROOM_HATTER_B     = 0x19,
    ENEMYKIND_T1_BRONTO_BURT        = 0x1A,
    ENEMYKIND_T1_BRONTO_BURT_B      = 0x1B,
    ENEMYKIND_T1_SCARFY             = 0x1C,
    ENEMYKIND_T1_SWORD_KNIGHT       = 0x1D,
    ENEMYKIND_T1_CAPPY              = 0x1E,
    ENEMYKIND_T1_CAPPY_B            = 0x1F,
    ENEMYKIND_T1_WHEELIE            = 0x20,
    ENEMYKIND_T1_HEAT_PHAN_PHAN     = 0x21, // Fire-themed variant
    ENEMYKIND_T1_NODDY              = 0x22,
    ENEMYKIND_T1_CHILLY             = 0x23,
    ENEMYKIND_T1_FLAPPY             = 0x24,
    ENEMYKIND_T1_PLASMA_WISP        = 0x25,
    ENEMYKIND_T1_GORDO              = 0x26,
    ENEMYKIND_T1_BOMBER             = 0x27,
    ENEMYKIND_T1_PICHIKURI          = 0x28,
    ENEMYKIND_T1_PICHIKURI_B        = 0x29,
    ENEMYKIND_T1_DAYL               = 0x2A,
    ENEMYKIND_T1_DAYL_B             = 0x2B,
    ENEMYKIND_T1_CALLER             = 0x2C,
    ENEMYKIND_T1_WALKY              = 0x2D,
    ENEMYKIND_T1_WADDLE_DEE_TRUCK   = 0x2E,
    ENEMYKIND_T1_WADDLE_DEE         = 0x2F,

    // Tier 2 - Further enhanced (flags=1), same data_index sequence
    ENEMYKIND_T2_BROOM_HATTER       = 0x30,
    ENEMYKIND_T2_BROOM_HATTER_B     = 0x31,
    ENEMYKIND_T2_BRONTO_BURT        = 0x32,
    ENEMYKIND_T2_BRONTO_BURT_B      = 0x33,
    ENEMYKIND_T2_SCARFY             = 0x34,
    ENEMYKIND_T2_SWORD_KNIGHT       = 0x35,
    ENEMYKIND_T2_CAPPY              = 0x36,
    ENEMYKIND_T2_CAPPY_B            = 0x37,
    ENEMYKIND_T2_WHEELIE            = 0x38,
    ENEMYKIND_T2_HEAT_PHAN_PHAN     = 0x39,
    ENEMYKIND_T2_NODDY              = 0x3A,
    ENEMYKIND_T2_CHILLY             = 0x3B,
    ENEMYKIND_T2_FLAPPY             = 0x3C,
    ENEMYKIND_T2_PLASMA_WISP        = 0x3D,
    ENEMYKIND_T2_GORDO              = 0x3E,
    ENEMYKIND_T2_BOMBER             = 0x3F,
    ENEMYKIND_T2_PICHIKURI          = 0x40,
    ENEMYKIND_T2_PICHIKURI_B        = 0x41,
    ENEMYKIND_T2_DAYL               = 0x42,
    ENEMYKIND_T2_DAYL_B             = 0x43,
    ENEMYKIND_T2_CALLER             = 0x44,
    ENEMYKIND_T2_WALKY              = 0x45,
    ENEMYKIND_T2_WADDLE_DEE_TRUCK   = 0x46,
    ENEMYKIND_T2_WADDLE_DEE         = 0x47,

    // Special / Event actors
    ENEMYKIND_SP_BROOM_HATTER       = 0x48, // flags=2
    ENEMYKIND_SP_SWORD_KNIGHT       = 0x49, // flags=2
    ENEMYKIND_SP_WADDLE_DEE_TRUCK   = 0x4A, // flags=2
    ENEMYKIND_SP_GORDO              = 0x4B, // flags=3, event gordo
    ENEMYKIND_TAC                   = 0x4C,
    ENEMYKIND_DYNA_BLADE            = 0x4D,
    ENEMYKIND_METEOR                = 0x4E,

    ENEMYKIND_NUM                   = 0x4F,

    // Tier boundaries
    ENEMYKIND_TIER0_START           = 0x00,
    ENEMYKIND_TIER0_END             = 0x18,
    ENEMYKIND_TIER1_START           = 0x18,
    ENEMYKIND_TIER1_END             = 0x30,
    ENEMYKIND_TIER2_START           = 0x30,
    ENEMYKIND_TIER2_END             = 0x48,
    ENEMYKIND_SPECIAL_START         = 0x48,
    ENEMYKIND_ENEMIES_PER_TIER      = 0x18, // 24 enemies per tier
} EnemyKind;

// Descriptor passed to EventActor_Create, built on the stack by Enemy_SpawnActor
// and the event start functions.
typedef struct EventActorDesc
{
    EnemyKind kind;         // 0x00
    Vec3 position;          // 0x04: spawn world position
    Vec3 forward;           // 0x10: forward direction (unit vector)
    Vec3 up;                // 0x1C: up direction (unit vector)
    float scale;            // 0x28: model scale (typically abs of a spawn-data value, or 1.0)
    int spawn_index;        // 0x2C: spawn tracking counter. -1 for standalone (skips lifetime field)
    int spawn_slot;         // 0x30: spawn slot index. -1 for standalone
    int x34;                // 0x34: 0 from every vanilla caller; copied to EnemyData.x24
    int lifetime;           // 0x38: lifetime in frames. Only written to EnemyData if spawn_index != -1
    int variant;            // 0x3C: tier/variant flags -> EnemyData.tier_flags; for kinds 0x48-0x4A the parent GOBJ instead
    int is_airborne;        // 0x40: initial EnemyData.is_airborne; 1 from Enemy_SpawnActor
    Vec3 leash_center;      // 0x44: center of the horizontal leash circle
    float leash_radius;     // 0x50: -1.0 = no leash; otherwise position is clamped to this radius of leash_center
    Vec3 ground_normal;     // 0x54: ground/surface normal at spawn point
} EventActorDesc;

// One entry of the per-actor animseq table. The table base is *(actor_data+0x0C);
// EventActor_ChangeState resolves ed->anim_data = &table[anim_index] (anim_index
// is the state-table entry's word0).
// EventActor_AnimDataInit (0x80200c04) feeds anim_joint/mat_anim_joint to
// JObj_AddAnimAll to bind the joint and material animation to the model tree.
typedef struct EnemyAnimSeqEntry
{
    void *anim_joint;       // 0x00, HSD AnimJoint (joint/skeletal animation)
    void *mat_anim_joint;   // 0x04, HSD MatAnimJoint (material animation)
    void *script;           // 0x08, anim-script bytecode, loaded into EnemyData.anim_command_ptr
    unsigned char flags;    // 0x0C, bit 0x40 loops the animation; bit 0x80 gates per-frame animation work
    unsigned char padd;     // 0x0D
    unsigned char pade;     // 0x0E
    unsigned char padf;     // 0x0F
} EnemyAnimSeqEntry; // 0x10 bytes

// GObj proc priorities EventActor_Create registers.
typedef enum EnemyPri
{
    ENEMYPRI_0,             // per-frame damage reset
    ENEMYPRI_ANIM,          // anim advance, state machine, anim_cb
    ENEMYPRI_PHYS = 4,      // phys_cb, vel += accel, pos += vel
    ENEMYPRI_ENVCOLL,       // envcoll_cb
    ENEMYPRI_6,             // shadow, pri6_cb, model matrix
    ENEMYPRI_TRIGGER,       // trigger_cb, HurtData update
    ENEMYPRI_8,             // no-op
    ENEMYPRI_HITCOLL,
    ENEMYPRI_DMGAPPLY,
    ENEMYPRI_ENDOFFRAME = 21,
} EnemyPri;

typedef struct EnemyData EnemyData;

// One state-table entry: the 14 common states at 0x804b2950 and each kind's own
// table use this layout. Each callback is copied into EnemyData on a state change.
typedef struct EnemyStateDesc
{
    int anim_index;                    // 0x00, animseq index, -1 = none
    void (*anim_cb)(EnemyData *ed);    // 0x04
    void (*phys_cb)(EnemyData *ed);    // 0x08
    void (*envcoll_cb)(EnemyData *ed); // 0x0C
    void (*pri6_cb)(EnemyData *ed);    // 0x10
} EnemyStateDesc;

typedef struct EnemyData
{
    GOBJ *gobj;             // 0x0, this actor's own GOBJ
    GOBJ *child_gobj;       // 0x4, child/rider actor GOBJ (e.g., knight on mount). 0 if none.
    GOBJ *parent_gobj;      // 0x8, parent/target GOBJ. Used by state functions to follow/track another entity. Null crashes some states.
    EnemyKind kind;         // 0xc
    void *model_desc;       // 0x10, JOBJDesc of the model, **(actor_data+0x08)
    void *actor_data;       // 0x14, tier-specific data from Enemy_GetActorData. Selects sub-entry based on tier flags (0=T0, 1=T1/T2, 2/3/4=special variants).
    int x18;                // 0x18
    int x1c;                // 0x1c
    int spawn_slot;         // 0x20, from desc. -1 for standalone actors.
    int x24;                // 0x24, desc.x34
    int spawn_index;        // 0x28, from desc. -1 for standalone, which leaves lifetime at -1.
    int lifetime;           // 0x2c, desc.lifetime; decremented each frame at ENEMYPRI_ENDOFFRAME for OOB enemies
    int tier_flags;         // 0x30, variant selector (0=T0, 1=T1/T2, 2/3/4=special). desc.variant, 0 for kinds 0x48-0x4A.
    int state;              // 0x34, current state ID. Written by EventActor_ChangeState.
    int common_state_num;   // 0x38, 14; EventActor_ChangeState hardcodes the same cutoff
    int anim_index;         // 0x3c, animation index from state table entry. -1 = no animation.
    EnemyStateDesc *common_state_table; // 0x40, 0x804b2950, states below common_state_num
    EnemyStateDesc *state_table;        // 0x44, the kind's own states, from the kind descriptor at 0x804b1d98
    EnemyAnimSeqEntry *anim_data; // 0x48, current animation entry, resolved as
                                  //       *(actor_data+0x0C) + ed->anim_index*0x10
    float anim_timer;       // 0x4c, animation keyframe timer (decremented per frame by StateMachine)
    float anim_frame;       // 0x50, current animation frame accumulator (= x2a8 + x2ac each frame, written by EventActor_StateMachine)
    void *anim_command_ptr; // 0x54, animation script bytecode pointer
    int anim_loop_depth;    // 0x58, animation script loop nesting depth
    int x5c;                // 0x5c
    int x60;                // 0x60
    int x64;                // 0x64
    int x68;                // 0x68
    int x6c;                // 0x6c
    void *col_anim;         // 0x70, ColAnim component (anim-script cmd 24/25 -> ColAnim_Apply/Reset)
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
    int xec;                // 0xec
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
    Vec3 spawn_forward;     // 0x34c, desc.forward as given
    Vec3 spawn_up;          // 0x358, desc.up as given
    float alpha;            // 0x364, model alpha; the fade-in proc steps it 0.1 -> 1.0, and hit collision waits for 1.0
    float param_base_scale; // 0x368, from *actor_data+0x00. Base scale; also -> tier_base_scale at 0x2D0.
    float param_scale_2;    // 0x36c, from *actor_data+0x04. Per-type secondary params.
    int param_sentinel;     // 0x370, from *actor_data+0x08
    float param_lod_radius; // 0x374, from *actor_data+0x0C. Sphere radius EventActorGObj_GetLOD projects.
    float param_lod0_size;  // 0x378, from *actor_data+0x10. Screen size above which LOD 0 is used.
    float param_lod1_size;  // 0x37c, from *actor_data+0x14. Screen size above which LOD 1 is used.
    float param_move_param; // 0x380, from *actor_data+0x18. Movement parameter.
    int x384;               // 0x384
    float param_path_speed; // 0x388, from *actor_data+0x20. Path speed.
    float param_spline_walk_speed_base; // 0x38c, from *actor_data+0x24. Spline walk speed base.
    float param_speed;      // 0x390, from *actor_data+0x28. Speed parameter (scales knockback velocity).
    float param_spline_walk_speed_2; // 0x394, from *actor_data+0x2C. Spline walk speed secondary.
    float param_random_timing; // 0x398, from *actor_data+0x30. Random timing variation (x HSD_Randf()).
    float param_speed_2;    // 0x39c, from *actor_data+0x34. Speed param.
    float param_3a0;        // 0x3a0, from *actor_data+0x38.
    float param_gravity;    // 0x3a4, from *actor_data+0x3C. Gravity/fall acceleration.
    float param_3a8;        // 0x3a8, from *actor_data+0x40.
    int param_frame_count;  // 0x3ac, from *actor_data+0x44. Frame count/duration.
    int param_hp;           // 0x3b0, from *actor_data+0x48. Hit collision stops once damage_accum_1 reaches it.
    int param_3b4;          // 0x3b4, from *actor_data+0x4C.
    int param_3b8;          // 0x3b8, from *actor_data+0x50. Ground check mode.
    int param_3bc;          // 0x3bc, from *actor_data+0x54.
    float param_ground_clearance; // 0x3c0, from *actor_data+0x58. Height above the ground EventActor_GroundSnap places the actor.
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
    void *per_type_params;  // 0x40c, copy of the kind's *(actor_data+4) block; sized by the per-kind table at 0x804b2558
    HurtData *hurt_data;    // 0x410
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
    HitRegion hit_region;   // 0x45c, attack hitbox, refreshed by EventActor_RefreshAttackParams (0x80201ba4)
    GOBJ *captor_gobj;      // 0x524, rider GObj that inhaled the actor (EventActor_OnCapture)
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
    int captor_ply;         // 0x58c, captor_gobj's ply
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
    float gravity_strength; // 0x5c4, Gr_GetDownVector return, refreshed by EventActor_UpdateGravity
    Vec3 gravity_dir;       // 0x5c8, unit gravity-down vector from Gr_GetDownVector
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
    int spline_path_ready;  // 0x654, set to 1 before EventActor_PathInit; copied to spline_direction
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
    int kb_active;           // 0x87c, set to 1 when entering knockback state (EventActor_ApplyKnockback)
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
    int is_airborne;        // 0x908, 1 when EventActor_GroundSnap found no ground; selects the combat AI movement mode
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
    int shadow_visible;     // 0x958, EventActor_UpdateShadow sets the shadow-visibility render bit from it
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
    int damage_accum_1;     // 0x994, damage accumulator (capped at 9999); gates hit collision against param_hp
    int damage_accum_2;     // 0x998, secondary damage accumulator (capped at 9999)
    HurtKind attacker_kind; // 0x99c, HurtKind of the last hit's source, from the hit log
    int x9a0;               // 0x9a0
    int x9a4;               // 0x9a4
    int x9a8;               // 0x9a8
    int x9ac;               // 0x9ac
    int x9b0;               // 0x9b0
    int x9b4;               // 0x9b4
    int x9b8;               // 0x9b8
    int x9bc;               // 0x9bc
    int death_effect_id;    // 0x9c0, effect spawned on death (-1 = none)
    int death_sfx_id;       // 0x9c4, SFX played on death (-1 = none)
    int death_frame_counter; // 0x9c8, incremented each frame of the death state; the actor is destroyed
                             //        above 120. Doubles as the stun spark frame counter.
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
    GOBJ *kb_attacker_gobj; // 0xa0c, attacker GObj, resolved to the rider for machines and rider-owned weapons
    HurtKind kb_attacker_kind; // 0xa10, HurtKind of kb_attacker_gobj
    int kb_attacker_ply;    // 0xa14, attacker ply, 5 = none
    int stun_frames;        // 0xa18, hitstop frames left, counted down by CommonEnvColl; CommonPhys launches the actor at 0
    int knockback_tier;     // 0xa1c, response tier (0-3) based on per-hit damage thresholds (<10, <21, <32, >=32)
    int special_hit_kind;   // 0xa20, EventActor_ResolveHit: 1 = Bomber or bomb hit with attacker_flags 1, 2 = rider hit with attacker_flags 7
    GOBJ *special_hit_gobj; // 0xa24, attacker GObj of that hit; ApplyKnockback aims away from it for an HURTKIND_EVENTACTOR attacker
    int death_timer;        // 0xa28, counts up during death processing. Used by phys_cb to track death frame count.
    float anim_speed_scale; // 0xa2c, animation speed scale factor (decays each frame toward minimum)
    int xa30;               // 0xa30
    int xa34;               // 0xa34
    int xa38;               // 0xa38
    int efgroup;            // 0xa3c, effect group killed by EventActor_KillEfGroup (-1 = none)
    int efgroup2;           // 0xa40, effect group killed by EventActor_KillEfGroup2 (-1 = none)
    int xa44;               // 0xa44
    int xa48;               // 0xa48
    int xa4c;               // 0xa4c
    int xa50;               // 0xa50
    int xa54;               // 0xa54
    int sfx_handle_1;       // 0xa58, sound effect handle (audio emitter; anim-script cmd 20/21)
    int sfx_handle_2;       // 0xa5c, second SFX handle (audio emitter; anim-script cmd 20/21)
    int sfx_state_1;        // 0xa60, SFX state/sentinel (initialized to -1; anim-script cmd 20/21)
    int sfx_state_2;        // 0xa64, SFX state (initialized to -1)
    int sfx_handle_3;       // 0xa68, third SFX handle (audio emitter; anim-script cmd 20/21)
    int xa6c;               // 0xa6c
    int hit_vfx_1;          // 0xa70, impact VFX handle (stored by Meteor_HitTransition; also a particle effect handle for anim-script cmd 18)
    int hit_vfx_2;          // 0xa74, particle effect handle (anim-script cmd 18)
    int damage_frame_counter; // 0xa78, damage state tracking counter (ENEMYPRI_ENDOFFRAME)
    int xa7c;               // 0xa7c
    Vec3 leash_center;      // 0xa80, desc.leash_center
    float leash_radius;     // 0xa8c, desc.leash_radius; -1.0 = no leash, which also enables path following
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
    void (*anim_cb)(EnemyData *ed);    // 0xab8, ENEMYPRI_ANIM, from the state entry
    void (*phys_cb)(EnemyData *ed);    // 0xabc, ENEMYPRI_PHYS, from the state entry
    void (*envcoll_cb)(EnemyData *ed); // 0xac0, ENEMYPRI_ENVCOLL, from the state entry
    void (*pri6_cb)(EnemyData *ed);    // 0xac4, ENEMYPRI_6, from the state entry
    void *trigger_cb;       // 0xac8, ENEMYPRI_TRIGGER. No vanilla enemy installs it and
                            //        EventActor_ChangeState only zeroes it, which makes it the cleanest
                            //        custom-AI injection point - re-assert it after each state change.
    void *hit_reaction_cb1; // 0xacc, hit reaction callback. Set by init callback. Called from damage proc.
    void *hit_reaction_cb2; // 0xad0, hit reaction callback. Called at ENEMYPRI_DMGAPPLY when damage > threshold. If null, default knockback handler runs.
    int xad4;               // 0xad4, cleared on state change (unless flag 0x10)
    int xad8;               // 0xad8
    int xadc;               // 0xadc
    void *grounded_callback; // 0xae0, called when landing from knockback (states 0x0C/0x0D). If null, actor is destroyed instead.
    int xae4;               // 0xae4
    int xae8;               // 0xae8
    void *custom_death_callback; // 0xaec, if set, replaces default death behavior in phys_cb
    int script_const_0;     // 0xaf0, anim-script constant slot (cmd 22)
    int script_const_1;     // 0xaf4, anim-script constant slot (cmd 22)
    int script_const_2;     // 0xaf8, anim-script constant slot (cmd 22)
    int script_const_3;     // 0xafc, anim-script constant slot (cmd 22)
    int attribute_flags;    // 0xb00, attribute flag byte (anim-script cmd 11)
    float xb04;             // 0xb04, float written by anim-script cmd 11
    int render_flags;        // 0xb08, byte-accessed multi-purpose flags:
                             // Byte 0 (+0xB08): bits 0-1=ground_state (0=air, 1=transitioning, 2=grounded), bit 4=rendering disabled, bit 7=invisible.
                             // Byte 1 (+0xB09): bit 0=height_interp_enabled, bit 2=ground_contact for turning, bits 5-6=prev frame ground state.
                             // Byte 2 (+0xB0A): bit 1=event/state flag (anim-script cmd 23), bit 2=no-spline/force-kill, bits 5-6=knockback sub-state mode.
                             // Byte 3 (+0xB0B): bit 3=grounded bounce flag, bit 4=shadow visibility, bit 5=shadow active.
    int xb0c;               // 0xb0c
    float inhale_distance;  // 0xb10, distance to inhaling rider (state 0x0A)
    float shadow_base_scale; // 0xb14, shadow size computation base. Also used as inhale scale during state 0x0A.
    int xb18;               // 0xb18
    int suction_active;     // 0xb1c, 1 = being sucked in by rider (state 0x0A)
    int inhale_timer;       // 0xb20, frames since inhale started. Actor destroyed when > 120.
    short target_ply;       // 0xb24, targeted ply (-1 = none). Set by EventActor_FindNearestPlayer.
    short retarget_cooldown; // 0xb26, frames until re-evaluation
    float turn_timer;       // 0xb28, turn blend toward turn_to; counts down, 0 = no turn in progress
    Vec3 turn_from;         // 0xb2c, forward when the turn started
    Vec3 turn_to;           // 0xb38, turn target; EventActor_FindNearestPlayer sets it to pos - target pos
    int xb44;               // 0xb44
    short xb48;              // 0xb48
    short frame_counter;     // 0xb4a, state frame counter (s16). Incremented by envcoll_cb. Used for timeouts.
    short in_bounds_flag;    // 0xb4c, set to 1 when meteor enters map bounds (state 15)
    short camera_flag;       // 0xb4e, set to 1 when camera effect triggered
    Vec3 initial_pos;       // 0xb50, saved by post-init callback (pos at creation time, before state transitions or spline snaps)
    float zone_offset;      // 0xb5c, height offset from zone table (meteor)
    int landing_vfx_1;      // 0xb60, landing VFX handle (stored by Meteor_Landing, monitored by state 17 envcoll_cb)
    int landing_vfx_2;      // 0xb64
    Vec3 collision_radii;   // 0xb68, base collision sphere radii (from actor_data)
    void *collision_sphere;  // 0xb74, collision sphere handle. Created by Meteor_HitTransition/Meteor_Landing.
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

// Per-kind descriptor, one pointer per EnemyKind in stc_enemy_kind_desc_table.
typedef struct EnemyKindDesc
{
    EnemyStateDesc *state_table;        // 0x00, -> EnemyData.state_table
    void *x4;                           // 0x04
    void (*init)(EnemyData *ed);        // 0x08, run by EventActor_Create
    void (*copy_params)(EnemyData *ed); // 0x0C, run by EventActor_CopyParamBlock
    void *x10;                          // 0x10
    void (*on_destroy)(EnemyData *ed);  // 0x14, run by EventActor_Destructor
    void *x18;                          // 0x18
    void (*on_capture)(EnemyData *ed);  // 0x1C, run by EventActor_OnCapture
} EnemyKindDesc;

static EnemyStateDesc *stc_enemy_common_state_table = (EnemyStateDesc *)0x804b2950; // [14]
static EnemyKindDesc **stc_enemy_kind_desc_table = (EnemyKindDesc **)0x804b1d98;   // [ENEMYKIND_NUM]
static int *stc_enemy_kind_archive = (int *)0x804b22b4;       // [ENEMYKIND_NUM][2]: data_index, flags
static char *stc_enemy_archive_loaded = (char *)0x8055a210;   // [22], by data_index, 1 = loaded
static void **stc_enemy_archive_root = (void **)0x8055a228;   // [22], by data_index

// Iterates the stage enemy list, calling Enemy_CheckAndLoad per kind. Skipped only
// in City Trial Free Run; runs normally in timed City Trial.
void Enemy_LoadStageEnemies(void); // 0x800f25b4
short *Enemy_GetStagesEnemies(int stage_kind); // 0x80262808, EnemyKind array for the stage, -1 terminated
void Enemy_InitPositionData(void); // 0x800f2634, allocates enemy position slots, loads positions from stage data
void Enemy_InitSpawner(void); // 0x800f2ee4, creates enemy manager GObj with Enemy_Think proc
void Enemy_Think(void); // 0x800f3904, GObj proc callback for enemy manager (Air Ride)
void Enemy_CityTrialThink(void); // 0x800f33c0, GObj proc callback for enemy manager (City Trial)

// Idempotent. The event actors 0x4C-0x4E load only while Gm_IsEventsEnabled holds;
// every other kind always loads.
void Enemy_CheckAndLoad(EnemyKind kind); // 0x801fd060
void Enemy_LoadFile(EnemyKind kind); // 0x801fd348, loads enemy archive data from disc. No-ops if already loaded.

// Spawn-slot wrapper for modes 1 and 3: builds a descriptor and calls
// EventActor_Create. enemy_id_packed = (variant << 8) | enemy_id; -1 skips creation.
void Enemy_SpawnActor(int spawn_slot, int enemy_id_packed, int position_index); // 0x800f13a8
// Mode 2 (STKIND_MELEE1) spawn helper, building its descriptor from
// spawn_entries[position_index].
void Enemy_SpawnActorMode2(int spawn_slot, int position_index); // 0x800f16c0
// Mode 2 picker: weighted-picks a meta-enemy category biased by
// EnemyMgr.time_progress, then a concrete enemy from that category's weight
// column. Writes the spawn-entry index and returns the enemy_id, -1 if none.
int Enemy_SpawnerDecideMode2(int *out_entry_index); // 0x800f0efc
// Any EnemyKind; NULL on failure.
GOBJ *EventActor_Create(EventActorDesc *desc); // 0x801fbb50
// Proper actor destruction - recursively destroys children, clears inter-actor
// references and runs cleanup before GObj_Destroy. Use instead of raw GObj_Destroy.
void EventActorGObj_Destroy(GOBJ *gobj); // 0x801fbf2c
// Tail of EventActorGObj_ProcHitColl: resolves the frame's hit log into the attacking
// machine / rider / weapon, credits it through Ply_RecordEnemyDefeat, and leaves
// the attacker in kb_attacker_gobj / kb_attacker_kind / kb_attacker_ply.
void EventActor_ResolveHit(EnemyData *ed); // 0x802021fc
// Per-hit stadium KO credit from EventActor_ResolveHit: Ply_AddStadiumEnemyKO for an
// actor passing Enemy_KindCountsAsKO. ply 5 = no player.
void EventActor_CreditStadiumKO(EnemyData *ed, int ply); // 0x802025dc
// 0 for the Cappy B and Dayl B kinds of every tier, the special Broom Hatter / Sword
// Knight / Waddle Dee Truck, and anything out of range; 1 for every other kind.
int Enemy_KindCountsAsKO(EnemyKind kind); // 0x802049fc
// The same test on an actor GObj's EnemyData.kind; gates Ply_RecordEnemyDefeat and
// Ply_RecordEnemySwallow.
int EventActorGObj_IsChecklistEnemy(GOBJ *gobj); // 0x80204a80
EnemyKind EventActorGObj_GetKind(GOBJ *gobj); // 0x8020409c, the actor's EnemyData.kind
void EventActor_CleanupCollisionSphere(EnemyData *ed); // 0x8021f1bc, destroys collision_sphere if non-null and nulls it.
void EventActor_KillEfGroup(EnemyData *ed);  // 0x8020c6e0, kills efgroup if != -1
void EventActor_KillEfGroup2(EnemyData *ed); // 0x8020c70c, kills efgroup2 if != -1
void EventActor_Hide(EnemyData *ed); // 0x801fed40, sets bit 7 (invisible) of render_flags (+0xB08), then calls EventActorGObj_DisableRendering. Full hide.
// Clears the invisible bit of render_flags, then enables rendering for kinds
// below 0x4C and disables it at or above - so it leaves a meteor (0x4E) hidden.
void EventActor_SetVisibility(EnemyData *ed); // 0x801fed74
void EventActorGObj_EnableRendering(GOBJ *gobj); // 0x80204198, clears bit 4 of render_flags (+0xB08).
void EventActorGObj_DisableRendering(GOBJ *gobj); // 0x802041b0, sets bit 4 of render_flags (+0xB08).
// render_flags bits and JOBJ_HIDDEN are independent. For kinds >= 0x4C, clearing
// render_flags alone is insufficient - also JObj_ClearFlagsAll(jobj, JOBJ_HIDDEN).
// Sets JOBJ_HIDDEN on the actor's model and its child's, and clears shadow_visible.
void EventActorGObj_HideModel(GOBJ *gobj); // 0x802042fc
float EventActorGObj_GetAnimRate(GOBJ *gobj); // 0x802049b8, EnemyData.anim_rate. Crashes on NULL.
int Gm_IsEventsEnabled(void); // 0x8000a348, GameData.is_enable_events

// Common states 0x00-0x0D are shared by every kind; 0x0E and up index the kind's own table.
#define ENEMYSTATE_DEATH     0x09 // EventActor_DeathAnim (0x80203e60)
#define ENEMYSTATE_INHALED   0x0A // EventActor_InhaledEnvColl (0x80203b28) destroys after 120 frames
#define ENEMYSTATE_KNOCKBACK 0x0B
#define ENEMYSTATE_LAUNCHED  0x0C
#define ENEMYSTATE_SLIDING   0x0D
#define ENEMYSTATE_PERTYPE   0x0E // first per-type state, entered on spawn

// Transitions to a new behavioral state. flags: 0x01 skip anim setup, 0x02 skip
// anim reset if same, 0x04 skip StateCleanup, 0x08 keep anim_pos, 0x10 keep
// xad4, 0x20 skip HurtData reset, 0x40 skip efgroup cleanup. trigger_cb is
// zeroed regardless.
void EventActor_ChangeState(EnemyData *ed, int state, int flags, float start_frame, float rate); // 0x801fc398

// Disables rendering and sets JOBJ_HIDDEN, then zeros velocity, enters state 15
// and seeds velocity from the zone/speed tables. Requires valid
// stc_meteor_event_data, and the caller must re-enable rendering afterward.
void Meteor_BehaviorInit(EnemyData *ed); // 0x8021e1a0
// State 16 timeout: enters state 17 (the table's last entry), creates the landing
// VFX and damage sphere and rumbles nearby players. State 17 destroys the actor
// once the landing VFX ends.
void Meteor_Landing(EnemyData *ed); // 0x8021ea5c
// State 15 hit handler: normalizes velocity, computes impact speed, enters state
// 16 and creates the impact VFX and audio fade.
void Meteor_HitTransition(EnemyData *ed); // 0x8021e7c4

float Enemy_DistToPlayer(int ply, Vec3 *pos); // 0x801fffa4, distance from pos to ply's rider; FLT_MAX with no rider
// Rumbles ply's controller through Rider_TriggerRumble; returns 1 when ply has no
// rider. Enemy damage flows through the HitColl pipeline instead.
int Enemy_RumblePlayer(int ply, int intensity, int duration); // 0x801ff80c

// Finds the nearest spline to ed->pos and assigns spline_primary/secondary plus
// segment and arc param. Sets bit 2 of +0xB0A if no spline is found.
void EventActor_PathInit(EnemyData *ed); // 0x80206e2c

// actor_data for a loaded archive, indexed by {data_index, flags} from
// stc_enemy_kind_archive. Returns 0 if the archive is not loaded.
void *Enemy_GetActorData(EnemyKind kind); // 0x801fd498

// GObj procs, all registered unconditionally by EventActor_Create at the EnemyPri
// priorities, plus the Enemy_GX render callback on GX link 9 (GAMEGX_ENEMY).
void EventActorGObj_ProcResetDamage(GOBJ *gobj); // 0x801fc670, ENEMYPRI_0: zeros per-frame damage via HurtData_ResetPerFrame
void EventActorGObj_ProcAnim(GOBJ *gobj); // 0x801fc698, ENEMYPRI_ANIM: HSD anim advance + state machine + anim_cb
void EventActorGObj_ProcPhys(GOBJ *gobj); // 0x801fc6fc, ENEMYPRI_PHYS: phys_cb + vel += accel, pos += vel, OOB floor kill (skipped for kinds >= 0x4C)
void EventActorGObj_ProcEnvColl(GOBJ *gobj); // 0x801fc7c4, ENEMYPRI_ENVCOLL: envcoll_cb (main per-state AI logic)
void EventActorGObj_ProcSharedModel(GOBJ *gobj); // 0x801fc7f8, ENEMYPRI_6: shadow update + pri6_cb + EventActor_SharedUpdate
void EventActorGObj_ProcTrigger(GOBJ *gobj); // 0x801fc848, ENEMYPRI_TRIGGER: trigger_cb + HurtData update + position snap
// ENEMYPRI_8: EventActorGObj_ProcHitCollInit (0x801fc8e8) - single blr, no-op stub
// ENEMYPRI_HITCOLL: HitColl processing, skipped while alpha < 1.0 or damage_accum_1 >= param_hp.
void EventActorGObj_ProcHitColl(GOBJ *gobj); // 0x801fc8ec
void EventActorGObj_ProcDmgApply(GOBJ *gobj); // 0x801fc9f0, ENEMYPRI_DMGAPPLY: reads HurtData output, calls EventActor_GiveDamage, dispatches hit_reaction_cb2
void EventActorGObj_ProcEndOfFrame(GOBJ *gobj); // 0x801fcabc, ENEMYPRI_ENDOFFRAME: pos -> pos_prev, ground state flags, lifetime/despawn, OOB destroy

void EventActorGObj_InitFromDesc(GOBJ *gobj, EventActorDesc *desc); // 0x801fb53c, copies desc into the GObj's EnemyData
void EventActor_SpawnChild(EnemyData *parent_ed); // 0x801fcda0, spawns child actor (rider/attached entity) and links parent<->child GOBJs
void EventActor_StateCleanup(EnemyData *ed); // 0x801fe110, cleans up attach slots (ed+0x918), detaches/destroys orphaned child objects
void EventActor_SharedUpdate(EnemyData *ed); // 0x801fd780, rebuilds the model matrix from pos, forward, up and final_scale
// When is_airborne and vel is within 90 degrees of gravity_dir, replaces vel's
// gravity_dir component with param_3a8 if that component is below -param_3a8.
void EventActor_ClampGravityVel(EnemyData *ed); // 0x801fd7bc
void EventActor_UpdateShadow(EnemyData *ed); // 0x80200208, per-frame shadow update at ENEMYPRI_6
// Runs when a rider inhales the actor: stores rider_gobj in captor_gobj, and arg2 at ed+0x52c.
void EventActor_OnCapture(EnemyData *ed, GOBJ *rider_gobj, int arg2); // 0x802038c4
void EventActor_InitHurtData(EnemyData *ed); // 0x80201ee8, creates hurt_data
void EventActor_FollowParent(EnemyData *ed); // 0x80219eec, child actor state: syncs position/scale from parent_gobj
void EventActor_CopyParentState(EnemyData *ed); // 0x80219fd4, copies parent's position/orientation/forward/up to child
void EventActor_Destructor(EnemyData *ed); // 0x801fcca0, GObj userdata destructor
void EventActor_StateMachine(EnemyData *ed); // 0x802017d0, animation script bytecode processor (commands 0-10 builtin, 11+ enemy-specific)
void Enemy_UnregisterFromSpawnSlot(GOBJ *gobj); // 0x800f3b28, removes enemy from spawn slot tracking, decrements active count, sets respawn timer
// Copies 0xA4 bytes from *actor_data into param_base_scale..+0x40B, then runs the
// kind descriptor's copy_params.
void EventActor_CopyParamBlock(EnemyData *ed); // 0x802006b4

void EventActor_GiveDamage(EnemyData *ed, float damage); // 0x8020b680, adds damage to both accumulators (capped at 9999)
int Enemy_ClassifyDamageTier(float damage); // 0x8020b740, classifies damage into tier 0-3 based on global thresholds
float Enemy_ScaleDamage(float damage); // 0x8020b71c, scales damage by global multiplier from enemy param table
// Full knockback transition: sets stun_frames, computes velocity, enters the state.
void EventActor_ApplyKnockback(EnemyData *ed, HurtData *hurt_data, int mode); // 0x8020b784

void EventActor_GroundPhysicsVelocity(EnemyData *ed); // 0x80209104, velocity-based ground projection with raycast
void EventActor_GroundPhysicsSurface(EnemyData *ed); // 0x802096b4, direct surface advancement with wall bounce
void EventActor_GroundAttach(EnemyData *ed); // 0x8020a664, final ground attachment after path following
int EventActor_CheckPathFollow(EnemyData *ed); // 0x8020b01c, checks if enemy should follow path (spline)
// Sets the terrain-locked flag, bit 2 of ed+0xB0B, from the Broom Hatter and
// Wheelie init callbacks. The unlocker at 0x8020ae68 clears the same bit.
void EventActor_SetTerrainLocked(EnemyData *ed); // 0x8020ae54
void EventActor_PathAdvance(EnemyData *ed, Vec3 *input_pos, Vec3 *output_pos, float speed); // 0x8020a040, advances parametric position along spline path

// Callbacks of the common state table.
void EventActor_CommonAnim(EnemyData *ed); // 0x8020bd68, anim_cb of states 0-8: animation rate scaling, stun freeze
void EventActor_CommonPhys(EnemyData *ed); // 0x8020be1c, phys_cb of states 0-8: hitstop hold and shake, launch at stun end, death once intangibility runs out
void EventActor_CommonEnvColl(EnemyData *ed); // 0x8020c558, envcoll_cb of states 0-8: stun countdown, surface bounce once launched, spark every 5th frame
// Refreshes map_collision at pos through mpColl_Update, mpColl_SetDefaultParams2 and
// mpColl_UpdateShapeExtents. CommonEnvColl runs it while stun_frames is positive.
void EventActor_RefreshCollision(EnemyData *ed); // 0x80204d30
// Collides map_collision at pos; on a wall, floor or ceiling contact turns vel off the
// surface normal, plays a random bump sound and rumbles a rider, machine or weapon
// attacker within EnemyParamTable+0x84. CommonEnvColl runs it once the actor is launched.
void EventActor_CollideAndBounce(EnemyData *ed); // 0x80205bb4
// anim_cb of state 0x09: death effect and SFX, model hide, destroy once
// death_frame_counter passes 120. Kinds 0x48-0x4A jump the counter to 600.
void EventActor_DeathAnim(EnemyData *ed); // 0x80203e60
void EventActor_KnockbackAnim(EnemyData *ed); // 0x8020ddb4, anim_cb of state 0x0B: compute knockback velocity, set launch trajectory
void EventActor_EnterLaunched(EnemyData *ed); // 0x8020e0bc, enters state 0x0C (launched/airborne)
void EventActor_EnterSliding(EnemyData *ed); // 0x8020e63c, enters state 0x0D (grounded/sliding)

void EventActor_ZeroVelocity(EnemyData *ed); // 0x801fd6b0, zeros accel (0x2E0) and vel (0x2EC)
// Raycasts from pos + up*5 to pos - up*50. On a hit, places the actor clearance
// above it (0.005 when clearance is 0), takes up from the surface and clears
// is_airborne; on a miss sets is_airborne.
void EventActor_GroundSnap(EnemyData *ed, float clearance); // 0x80204fac
void EventActor_UpdateGravity(EnemyData *ed); // 0x802054e4, refreshes gravity_strength and gravity_dir at pos
int EventActor_AIPhysicsTick(EnemyData *ed); // 0x802081ec, central ground-following movement. Returns 0=moved, 1=stationary.
void EventActor_PathFollowUpdate(EnemyData *ed, int direction_mode); // 0x80209ce4, spline path-following movement update

// Shared AI helpers, used by Sword Knight chase and other combat enemies.
void EventActor_CombatMovement(EnemyData *ed); // 0x8020b490, AIPhysicsTick when is_airborne == 0, accel-based chase when 1
void EventActor_CombatAI(EnemyData *ed); // 0x802069e8, targeting when is_airborne == 0, proximity/range checks when 1
// Ground-following chase physics: orientation, speed, terrain raycast and
// ground-snap. Returns 0 if it moved, 1 if stationary.
int EventActor_GroundFollowMovement(EnemyData *ed); // 0x80208bd4

// Nearest rider within EnemyParamTable.detect_range, on a 20-39 frame retarget
// cooldown; sets target_ply and turn_to.
void EventActor_FindNearestPlayer(EnemyData *ed); // 0x801ffd78
// Nearest rider within range / 0.114286, then aims joint jobj_tree[2] at it. Its
// forward-angle gate only rejects angles above 180 degrees. Vanilla callers pass
// EnemyParamTable.leash_range.
void EventActor_FindNearestPlayerFOV(EnemyData *ed, float range); // 0x801ff8d8
// Writes the ply's signed forward-axis distance; returns 1 if behind, 0 if in
// front, -1 if the ply has no rider.
int EventActor_PlayerAheadDist(EnemyData *ed, int ply, float *out_forward_dist); // 0x801fea60
// Render LOD for Enemy_GX: the screen size of a param_lod_radius sphere at pos
// gives 0 above param_lod0_size, 1 above param_lod1_size, else 2. Also stored in
// ed+0xB09 bits 3-4.
int EventActorGObj_GetLOD(GOBJ *gobj); // 0x80206cc0

// Loads Enemy.dat (public emDataAll) into stc_enemy_param_table and clears the
// archive loaded flags and roots.
void Enemy_LoadCommonParams(void); // 0x801fd580

// One entry of the per-stage spawn table (EnemySpawnData.spawn_entries), 0x38
// bytes. Static data loaded verbatim from the stage .dat file - the game only
// reads it. Each entry describes one spawn position: where to draw the position
// vectors from, what enemy IDs may spawn there with what weights, plus per-spawn
// scale and lifetime.
//
// The id/weight arrays sit at mode-dependent offsets (the union below) because
// the three spawn modes pack them differently. Field readers:
//   - location_index, scale, lifetime, variant: Enemy_SpawnActor (0x800f13a8)
//     and the mode-2 spawn helper (0x800f16c0)
//   - ids/weights: Enemy_SpawnerDecide (0x800f1a14) and the mode-2 picker
//     (0x800f0efc)
// location_index indexes the stage enemy-position table (GrObj+0x138, stride
// 0x24 of three Vec3s) via loadEnemy_spawnXYLocation; the resulting position /
// direction / ground-normal are stored into the runtime per-position extended
// data, NOT into this entry.
typedef struct EnemySpawnEntry
{
    short location_index;       // 0x00, index into the stage enemy-position table
    short pad02;                // 0x02
    short entry_type;           // 0x04, sub-kind selector (modes 2/3; e.g. spline vs point)
    union
    {
        // mode 2 (STKIND_MELEE1): two-stage select. A meta-enemy category is
        // chosen from secondary_table[0], then one enemy is drawn from that
        // category's weight column here. enemy_id is this entry's enemy; the
        // column for a category is indexed by its meta id - 0x50, so the array
        // is sized to the space the entry has (0x08..0x2F) rather than to the
        // stage's category count. Declaring it any shorter lets the compiler
        // fold away writes to columns past the end.
        struct
        {
            short enemy_id;           // 0x06
            short weight_columns[20]; // 0x08, weight per meta-enemy category, indexed by id - 0x50
        } mode2;
        // mode 3 (STKIND_MELEE2): up to 5 {id, weight} pairs, weight -1 terminates.
        struct
        {
            short ids[5];           // 0x06
            short weights[5];       // 0x10
        } mode3;
        // mode 1 (Air Ride courses): up to 4 {id, weight} pairs, weight -1
        // terminates. lifetime jitter shares the 0x1A/0x1C pair (see below).
        struct
        {
            char pad06[0x18];       // 0x06..0x1D
            short ids[4];           // 0x1E
            short weights[4];       // 0x26
        } mode1;
        // Lifetime base/jitter for modes 1 and 3 (read by Enemy_SpawnActor).
        // Mode 2 instead uses lifetime2_base/lifetime2_range below.
        struct
        {
            char pad_lt[0x14];      // 0x06..0x19
            short lifetime_base;    // 0x1A, lifetime in frames (abs); 0/1 = no lifetime
            short lifetime_range;   // 0x1C, +/- random jitter applied when > 1 (abs)
        } life13;
        // Lifetime base/jitter for mode 2 (read by the mode-2 spawn helper).
        struct
        {
            char pad_lt2[0x1e];     // 0x06..0x23
            short lifetime2_base;   // 0x24, lifetime in frames (abs)
            short lifetime2_range;  // 0x26, +/- random jitter (abs)
        } life2;
    };
    float scale;                // 0x30, model scale (negated if negative, 1.0 if 0)
    int variant;               // 0x34, copied into the actor descriptor (parent/variant slot)
} EnemySpawnEntry; // 0x38 bytes

// Per-stage enemy spawn data, pointed to by r13 + 0x630.
// NULL when the per-stage "enemies enabled" flag (GameData+0xAA6 bit 4) is off
// (City Trial city map, Top Ride, and stadiums without enemies like Air Glider /
// Destruction Derby / Single Race). Populated by Enemy_InitPositionData.
//
// secondary_table is a pointer-array indexed by meta-enemy ID (0x50-0x5E offset
// by -0x50). Each entry points to a sub-table of {enemy_id, weight} short pairs,
// -1 terminated. May be NULL.
#define ENEMY_META_ID_BASE 0x50
#define ENEMY_META_NUM     15   // meta-enemy IDs 0x50-0x5E

typedef struct EnemySpawnConfig
{
    char x00[0x28];     // 0x00, layout unknown
    short mode;         // 0x28, 1=Air Ride, 2=STKIND_MELEE1, 3=STKIND_MELEE2
} EnemySpawnConfig;

typedef struct EnemySpawnData
{
    short spawn_count;          // 0x00, number of entries in spawn_entries
    short pad02;                // 0x02
    EnemySpawnEntry *spawn_entries; // 0x04, primary spawn table (stride 0x38)
    int x08;                    // 0x08
    int **secondary_table;      // 0x0C, pointer-array of meta-enemy sub-tables (may be NULL)
    EnemySpawnConfig *config;   // 0x10
} EnemySpawnData;

static EnemySpawnData **stc_enemy_spawn_data = (EnemySpawnData **)(0x805dd0e0 + 0x630);

// Enemy global parameter table: Enemy.dat public emDataAll, shared by every enemy
// type and tier. Tier arrays are indexed by the damage tier Enemy_ClassifyDamageTier
// picks.
typedef struct EnemyParamTable
{
    float global_scale;         // 0x00, 1.0, copied to EnemyData.global_enemy_scale
    float damage_scale;         // 0x04, 0.4, Enemy_ScaleDamage
    float tier_threshold[3];    // 0x08, {10,21,32}, Enemy_ClassifyDamageTier
    int x14[4];                 // 0x14, {10,30,50,70}
    float x24;                  // 0x24, 4.0
    float x28;                  // 0x28, 5.0
    int x2c;                    // 0x2c, 50
    int hit_iframes[4];         // 0x30, per tier {20,30,40,50}; post-hit intangibility (EventActor_ApplyKnockback)
    float hit_iframes_scale[4]; // 0x40, {1,.8,.6,.5}, indexed by GameData.player_num, not by tier
    float kb_launch[4];         // 0x50, per tier {2,3,4,5} -> EnemyData 0x9d8
    float stun_frames[4];       // 0x60, per tier {2,4,6,8} -> EnemyData 0xa18
    float mode_scale[3];        // 0x70, {1,1.1,1.2}, picked by game mode into EnemyData.mode_scale
    float x7c;                  // 0x7c, 15, EventActor_OnCapture
    float detect_range;         // 0x80, 50, EventActor_FindNearestPlayer acquisition radius
    float x84;                  // 0x84, 30
    float x88;                  // 0x88, 30, EventActor_CommonPhys
    float x8c;                  // 0x8c, 300, EventActor_CommonPhys attraction
    float leash_range;          // 0x90, 500, read by most per-type AI states
    int retarget_min;           // 0x94, 20
    int retarget_max;           // 0x98, 40; cooldown = min + HSD_Randi(max - min)
} EnemyParamTable;

// Loaded by Enemy_LoadCommonParams on every 3D scene load; NULL until then.
static EnemyParamTable **stc_enemy_param_table = (EnemyParamTable **)0x805dd878;

// HSD spline evaluation, used by actor path following.
float splArcLengthGetParameter(void *spline, float s); // 0x80415758, spline parameter at arc length s
// Evaluates the spline at param, which runs [0.0, 1.0] from the first to the last
// control point. The spline struct has u8 type at +0x0 and s16 num_points at +0x2.
// Crashes if spline is NULL.
void splGetSplinePoint(Vec3 *output, void *spline, float param); // 0x80414fc0
void splArcLengthPoint(Vec3 *output, void *spline, float s); // 0x80415958, splGetSplinePoint at splArcLengthGetParameter(spline, s). Crashes if spline is NULL.

#endif