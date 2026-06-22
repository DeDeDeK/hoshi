#ifndef KAR_H_MACHINE
#define KAR_H_MACHINE

#include "datatypes.h"
#include "obj.h"
#include "hurt.h"
#include "collision.h"
#include "trigger.h"
#include "item.h"

typedef enum MachineKind
{
    VCKIND_WARP,
    VCKIND_COMPACT,
    VCKIND_WINGED,
    VCKIND_SHADOW,
    VCKIND_HYDRA,
    VCKIND_BULK,
    VCKIND_SLICK,
    VCKIND_FORMULA,
    VCKIND_DRAGOON,
    VCKIND_WAGON,
    VCKIND_ROCKET,
    VCKIND_SWERVE,
    VCKIND_TURBO,
    VCKIND_JET,
    VCKIND_FLIGHT,
    VCKIND_FREE,
    VCKIND_STEER,
    VCKIND_WINGKIRBY,
    VCKIND_WINGMETAKNIGHT,
    VCKIND_WHEELNORMAL,
    VCKIND_WHEELKIRBY,
    VCKIND_WHEELIEBIKE,
    VCKIND_REXWHEELIE,
    VCKIND_WHEELIESCOOTER,
    VCKIND_WHEELDEDEDE,
    VCKIND_WHEELVSDEDEDE,
    VCKIND_NUM,
} MachineKind;

static const char *const MachineKind_Names[VCKIND_NUM] = {
    [VCKIND_WARP]           = "Warp Star",
    [VCKIND_COMPACT]        = "Compact Star",
    [VCKIND_WINGED]         = "Winged Star",
    [VCKIND_SHADOW]         = "Shadow Star",
    [VCKIND_HYDRA]          = "Hydra",
    [VCKIND_BULK]           = "Bulk Star",
    [VCKIND_SLICK]          = "Slick Star",
    [VCKIND_FORMULA]        = "Formula Star",
    [VCKIND_DRAGOON]        = "Dragoon",
    [VCKIND_WAGON]          = "Wagon Star",
    [VCKIND_ROCKET]         = "Rocket Star",
    [VCKIND_SWERVE]         = "Swerve Star",
    [VCKIND_TURBO]          = "Turbo Star",
    [VCKIND_JET]            = "Jet Star",
    [VCKIND_FLIGHT]         = "Flight Warp Star",
    [VCKIND_FREE]           = "Free Star",
    [VCKIND_STEER]          = "Steer Star",
    [VCKIND_WINGKIRBY]      = "Wing Kirby",
    [VCKIND_WINGMETAKNIGHT] = "Wing Meta Knight",
    [VCKIND_WHEELNORMAL]    = "Wheel",
    [VCKIND_WHEELKIRBY]     = "Wheel Kirby",
    [VCKIND_WHEELIEBIKE]    = "Wheelie Bike",
    [VCKIND_REXWHEELIE]     = "Rex Wheelie",
    [VCKIND_WHEELIESCOOTER] = "Wheelie Scooter",
    [VCKIND_WHEELDEDEDE]    = "Dedede Wheelie",
    [VCKIND_WHEELVSDEDEDE]  = "VS Dedede Wheelie",
};

typedef struct vcStarCommonAttr
{
    float x0;        // 0x0
    float x4;        // 0x0
    float x8;        // 0x0
    float xc;        // 0x0
    float x10;       // 0x0
    float x14;       // 0x0
    float x18;       // 0x0
    float max_speed; // 0x1c
    float x20;       // 0x0
    float x24;       // 0x0
    float x28;       // 0x0
} vcStarCommonAttr;

typedef struct vcWheelCommonAttr
{
    float x0;  // 0x0
    float x4;  // 0x0
    float x8;  // 0x0
    float xc;  // 0x0
    float x10; // 0x0
    float x14; // 0x0
    float x18; // 0x18
} vcWheelCommonAttr;

typedef struct vcWheelHandlingAttr
{
    float x0;        // 0x0
    float x4;        // 0x4
    float x8;        // 0x8
    float xc;        // 0xc
    float x10;       // 0x10
    float x14;       // 0x14
    float x18;       // 0x18
    float x1c;       // 0x1c
    float x20;       // 0x20
    float x24;       // 0x24
    float x28;       // 0x28
    float x2c;       // 0x2c
    float x30;       // 0x30
    float x34;       // 0x34
    float x38;       // 0x38
    float x3c;       // 0x3c
    float x40;       // 0x40
    float max_speed; // 0x40
} vcWheelHandlingAttr;

typedef struct vcDataKindStar
{
    struct
    {
        int x0;
    } *x0;
    struct
    {
        int x0;
    } *x4;
    vcStarCommonAttr *attr; // 0x8

} vcDataKindStar;

typedef struct vcDataCommon
{
    void *x0;
    void *x4;
    void *x8;
    void *xc;
    void *x10;
    void *x14;
    void *x18;
    void *x1c;
    struct // 0x20
    {
        void *x0;
        void *x4;
        struct
        {
            float match_progress; // time
            float chance[VCKIND_NUM];
        } *spawn_desc; // 0x8
        void *xc;
        struct
        {
            float weight[11]; // first float is the X/305 chance for 1 additional machine to spawn, second float is the chance for 2 additional, etc....
            int machine_kinds[29]; // not sure exactly what these are but likely the machine kinds that spawn in the city
        } *max_spawn_weights; // 0x10
    } *spawn_data;
} vcDataCommon;

typedef struct vcData
{
    struct // 0x0
    {
        int rider_sit_bone_idx;     // 0x0
        int rider_unk_bone_idx;     // 0x4
        float scale;                // 0x8
        float offense;              // 0xc
        float initial_cam_distance; // 0x10
        // more
    } *attr;
    struct // 0x4
    {
        JOBJ *joint; // 0x0
        // more
    } *model;
    struct // 0x8
    {
        int x0;
    } *x8;
    struct // 0xc
    {
        int x0;
    } *coll_attr;
    struct // 0x10
    {
        int x0;
    } *coll_sphere;
    vcWheelHandlingAttr *handling_attr; // 0x14
} vcData;

typedef struct vcDataLookup
{
    vcData *data[2][19]; // one for star and one for bike
} vcDataLookup;

typedef struct MachineSpawnDesc
{
    int is_bike; // 0x0
    u8 kind;     // 0x4
    Vec3 pos;    // 0x8
    Vec3 x14;    // 0x14 (is 0,0,-1)
    Vec3 x20;    // 0x20 (is 0,1,0)
    Vec3 x2c;    // unk, is 1,0,0
    float x38;   // unk, is 0
    float x3c;   // unk, is 0
    float x40;   // unk, is 0
    float x44;   // unk, is 0
    float x48;   // unk, is 0
    float x4c;   // unk, is 0
    float x50;   // unk, is 0
    float x54;   // unk, is 0
    float x58;   // unk, is 0
    float x5c;   // unk, is 0
    float x60;   // unk, is 0
    float x64;   // unk, is 0
    float x68;   // unk, is 0
    float x6c;   // unk, is 0
    float x70;   // unk, is 0
    float x74;   // unk, is 0
    float x78;   // unk, is 0
    float x7c;   // unk, is 0
    u8 x80;      // 0x81 is 0
    u8 x81;      // 0x81, is 0
} MachineSpawnDesc;

typedef struct MachineSpawnData
{
    GOBJ *g;                                      // 0x0,
    int total_match_frames;                       // 0x4, equal to the amount of frames in the match
    int spawn_timer;                              // 0x8, decreases, spawns when 0
    int vehicle_max;                              // 0xc, will not spawn more vehicles is cur amt is > this
    u16 x10;                                      // 0x10, amount of machines that exist?
    u16 x12;                                      // 0x12, amount that should be created?
    int vehicle_pos_num;                          // 0x14
    int vehicle_area_pos_num;                     // 0x18
    int x1c;                                      // 0x1c, unk counts down with each vehicle spawn
    Vec3 machineformation_pos[4];                 // 0x20
    u8 prev_machine_kind[4];                      // 0x50, circular buffer
    int prev_machine_index;                       // 0x54
    u8 x58[0x54];                                 // 0x58
    int xac;                                      // 0xac
    int xb0;                                      // 0xb0
    int xb4;                                      // 0xb4
    int xb8;                                      // 0xb8
    int xbc;                                      // 0xbc
    int c0;                                       // 0xc0
    u8 c4;                                        // 0xc4
    u8 c5;                                        // 0xc5
    u8 xc6_80 : 1;                                // 0xc6, 0x80, is set to 0 when a certain amount of machines have spawned
    u8 machineformation_is_start : 1;             // 0xc6, 0x40, flag set when machine formation event is queued.
    u8 machineformation_is_spawning_machines : 1; // 0xc6, 0x20, flag set when machine formation event is queued. lowered when all 5 are spawned
} MachineSpawnData;

typedef struct MachineData
{
    GOBJ *gobj;                           // 0x0. points to the machines gobj
    GOBJ *rider_gobj;                     // 0x4. points to the rider GOBJ currently on the machine. is 0 if machine is unoccupied.
    GOBJ *rider_unk1;                     // 0x8
    GOBJ *rider_unk2;                     // 0xc
    int is_bike;                          // 0x10
    int x14;                              // 0x14
    int x18;                              // 0x18
    int x1c;                              // 0x1c
    int x20;                              // 0x20
    MachineKind kind : 8;                 // 0x24
    int x28;                              // 0x28
    vcData *vcData;                       // 0x2c
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
    int x5c;                              // 0x5c
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
    int x108;                             // 0x108
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
    int x1b4;                             // 0x1b4
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
    int x2b0;                             // 0x2b0
    int x2b4;                             // 0x2b4
    int x2b8;                             // 0x2b8
    int x2bc;                             // 0x2bc
    int x2c0;                             // 0x2c0
    int x2c4;                             // 0x2c4
    int x2c8;                             // 0x2c8
    int x2cc;                             // 0x2cc
    int x2d0;                             // 0x2d0
    int x2d4;                             // 0x2d4
    int x2d8;                             // 0x2d8
    int x2dc;                             // 0x2dc
    int x2e0;                             // 0x2e0
    int x2e4;                             // 0x2e4
    int x2e8;                             // 0x2e8
    int x2ec;                             // 0x2ec
    int x2f0;                             // 0x2f0
    int x2f4;                             // 0x2f4
    int x2f8;                             // 0x2f8
    int x2fc;                             // 0x2fc
    int x300;                             // 0x300
    int x304;                             // 0x304
    int x308;                             // 0x308
    int x30c;                             // 0x30c
    float model_scale;                    // 0x310, per-machine model-scale multiplier (default 1.0). The machine model appliers (e.g. 0x801c9074) bake model_scale * model_scale_base into the machine model JObj's user matrix every frame via gmLanMenu_Scale3DObject. Mirrors RiderData.model_scale: writing it rescales the rendered machine until recreated.
    void *model_section;                  // 0x314, pointer to the machine model section; **(model_section) is the model root JObj passed to gmLanMenu_Scale3DObject.
    int x318;                             // 0x318
    int x31c;                             // 0x31c
    int x320;                             // 0x320
    Vec3 velocity;                        // 0x324 velocity vector, used in charge rate angle calculation
    int x330;                             // 0x330
    int x334;                             // 0x334
    int x338;                             // 0x338
    int x33c;                             // 0x33c
    int x340;                             // 0x340
    int x344;                             // 0x344
    int x348;                             // 0x348
    int x34c;                             // 0x34c
    int x350;                             // 0x350
    Vec3 projectile_inherit_velocity;     // 0x354, base velocity inherited by projectiles spawned from this machine's rider (added to rider->self_vel in spawnBomb/spawnGordo/spawnSensorBomb). Read via Machine_GetProjectileBaseVelocity.
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
    int x38c;                             // 0x38c
    int x390;                             // 0x390
    int x394;                             // 0x394
    float top_speed_current;              // 0x398, top speed for the active state - the cruise-speed cap the movement controllers clamp velocity against. Set each Machine_AdjustAttributes from top_speed_ground (0x4f0) when grounded (action_state_class 0x754 == 0) or the airborne value (0x5ac) otherwise, and held between recalcs (only AdjustAttributes writes it). Note machine_accel cancels out of the thrust/drag equilibrium, so this cap - not machine_accel - is what scales actual cruise speed.
    int x39c;                             // 0x39c
    int x3a0;                             // 0x3a0
    int x3a4;                             // 0x3a4
    int x3a8;                             // 0x3a8
    int x3ac;                             // 0x3ac
    int x3b0;                             // 0x3b0
    int x3b4;                             // 0x3b4
    int x3b8;                             // 0x3b8
    int x3bc;                             // 0x3bc
    int x3c0;                             // 0x3c0
    int x3c4;                             // 0x3c4
    int x3c8;                             // 0x3c8
    int x3cc;                             // 0x3cc
    int x3d0;                             // 0x3d0
    int x3d4;                             // 0x3d4
    int x3d8;                             // 0x3d8
    int x3dc;                             // 0x3dc
    int x3e0;                             // 0x3e0
    int x3e4;                             // 0x3e4
    Vec3 pos;                             // 0x3e8
    int x3f4;                             // 0x3f4
    int x3f8;                             // 0x3f8
    int x3fc;                             // 0x3fc
    int x400;                             // 0x400
    int x404;                             // 0x404
    int x408;                             // 0x408
    int x40c;                             // 0x40c
    int x410;                             // 0x410
    int x414;                             // 0x414
    Vec3 forward;                         // 0x418, velocity vector?
    Vec3 up;                              // 0x424, up vector
    int x430;                             // 0x430
    int x434;                             // 0x434
    int x438;                             // 0x438
    int x43c;                             // 0x43c
    int x440;                             // 0x440
    int x444;                             // 0x444
    int x448;                             // 0x448
    int x44c;                             // 0x44c
    int x450;                             // 0x450
    int x454;                             // 0x454
    int x458;                             // 0x458
    int base_attributes;                  // 0x45c, start of the derived-attribute block (124 words) memcpy'd from md->vcData each Machine_AdjustAttributes; the real per-vehicle base stats (separate from the patch/cap stat arrays at 0x94C+)
    int x460;                             // 0x460
    int x464;                             // 0x464
    float model_scale_base;               // 0x468, intrinsic model scale of this vehicle; the machine model appliers bake model_scale * model_scale_base into the model matrix. Per-machine size differences live here, so model_scale rests at 1.0.
    float coll_radius_base;               // 0x46c, mpColl sphere radius (f1 arg to mpColl_Update in Machine_InitialCollisionCheck/SetMpCollPosition -> coll_data->radius)
    int x470;                             // 0x470
    int x474;                             // 0x474
    int x478;                             // 0x478
    int x47c;                             // 0x47c
    int x480;                             // 0x480
    int x484;                             // 0x484
    int x488;                             // 0x488
    int x48c;                             // 0x48c
    int x490;                             // 0x490
    int x494;                             // 0x494
    int x498;                             // 0x498
    int x49c;                             // 0x49c
    int x4a0;                             // 0x4a0
    int x4a4;                             // 0x4a4
    int x4a8;                             // 0x4a8
    int x4ac;                             // 0x4ac
    int charge_full_duration;              // 0x4b0 frame count loaded into charge_full_timer when charge hits 1.0
    int charge_cooldown_duration;          // 0x4b4 frame count loaded into charge_cooldown_timer after auto-discharge
    int x4b8;                             // 0x4b8
    int x4bc;                             // 0x4bc
    int x4c0;                             // 0x4c0
    int x4c4;                             // 0x4c4
    int x4c8;                             // 0x4c8
    float hp_max;                         // 0x4cc
    int x4d0;                             // 0x4d0
    int x4d4;                             // 0x4d4
    int x4d8;                             // 0x4d8
    int x4dc;                             // 0x4dc
    int x4e0;                             // 0x4e0
    int x4e4;                             // 0x4e4
    float projectile_charge_scale;        // 0x4e8, scalar multiplier for projectile velocity, likely tied to machine charge/boost state. Read via Machine_GetProjectileChargeScale.
    int x4ec;                             // 0x4ec
    float top_speed_ground;               // 0x4f0
    int x4f4;                             // 0x4f4
    int x4f8;                             // 0x4f8
    float base_charge_rate;               // 0x4fc scaled by charge patches
    float turning_charge_rate;            // 0x500 maybe?
    int x504;                             // 0x504
    int x508;                             // 0x508
    int x50c;                             // 0x50c
    int x510;                             // 0x510
    int x514;                             // 0x514
    int x518;                             // 0x518
    int x51c;                             // 0x51c
    int x520;                             // 0x520
    int x524;                             // 0x524
    int x528;                             // 0x528
    int x52c;                             // 0x52c
    int x530;                             // 0x530
    int x534;                             // 0x534
    int x538;                             // 0x538
    int x53c;                             // 0x53c
    int x540;                             // 0x540
    int x544;                             // 0x544
    int x548;                             // 0x548
    int x54c;                             // 0x54c
    int x550;                             // 0x550
    int x554;                             // 0x554
    int x558;                             // 0x558
    int x55c;                             // 0x55c
    int x560;                             // 0x560
    int x564;                             // 0x564
    int x568;                             // 0x568
    int x56c;                             // 0x56c
    int x570;                             // 0x570
    int x574;                             // 0x574
    int x578;                             // 0x578
    int x57c;                             // 0x57c
    int x580;                             // 0x580
    int x584;                             // 0x584
    int x588;                             // 0x588
    int x58c;                             // 0x58c
    int x590;                             // 0x590
    int x594;                             // 0x594
    int x598;                             // 0x598
    int x59c;                             // 0x59c
    int x5a0;                             // 0x5a0
    int x5a4;                             // 0x5a4
    int x5a8;                             // 0x5a8
    u8 x5ac;                              // 0x5ac, Machine_AdjustAttributes reads a 4-byte value here as the airborne top speed (copied into top_speed_current 0x398) - likely a float overlapping the next bytes; reconcile with stadium_kind
    u8 stadium_kind;                      // 0x5ad
    u8 x5ae;                              // 0x5ae
    u8 x5af;                              // 0x5af
    int x5b0;                             // 0x5b0
    int x5b4;                             // 0x5b4
    int x5b8;                             // 0x5b8
    int x5bc;                             // 0x5bc
    int x5c0;                             // 0x5c0
    int x5c4;                             // 0x5c4
    int x5c8;                             // 0x5c8
    int x5cc;                             // 0x5cc
    int x5d0;                             // 0x5d0
    int x5d4;                             // 0x5d4
    int x5d8;                             // 0x5d8
    int x5dc;                             // 0x5dc
    int x5e0;                             // 0x5e0
    int x5e4;                             // 0x5e4
    int x5e8;                             // 0x5e8
    int x5ec;                             // 0x5ec
    int x5f0;                             // 0x5f0
    int x5f4;                             // 0x5f4
    int x5f8;                             // 0x5f8
    int x5fc;                             // 0x5fc
    int x600;                             // 0x600
    int x604;                             // 0x604
    int x608;                             // 0x608
    int x60c;                             // 0x60c
    int x610;                             // 0x610
    int x614;                             // 0x614
    int x618;                             // 0x618
    int x61c;                             // 0x61c
    int x620;                             // 0x620
    int x624;                             // 0x624
    int x628;                             // 0x628
    int x62c;                             // 0x62c
    int x630;                             // 0x630
    int x634;                             // 0x634
    int x638;                             // 0x638
    int x63c;                             // 0x63c
    int x640;                             // 0x640
    int x644;                             // 0x644
    int x648;                             // 0x648
    int x64c;                             // 0x64c
    union                                 // 0x650
    {                                     //
        vcStarCommonAttr *star;           //
        struct                            //
        {                                 //
            vcWheelCommonAttr common;     //
            vcWheelHandlingAttr handling; //
        } *wheel;                         //
    } attr;                               //
    int x654;                             // 0x654
    int x658;                             // 0x658
    int x65c;                             // 0x65c
    HurtData *hurt_data;                  // 0x660, passed as first arg to Machine_ApplyHurt. Created by Machine_InitHurtData
    struct                                //
    {                                     //
        Vec2 stick;                       // 0x664
        int buttons;                      // 0x66c
        u8 tilt_timer_x;                  // 0x670
        u8 tilt_timer_y;                  // 0x671
        u8 x672;                          // 0x672
        u8 x673;                          // 0x673
    } input;                              //
    GOBJ *shadow_gobj;                    // 0x674
    int x678;                             // 0x678
    int x67c;                             // 0x67c
    int x680;                             // 0x680
    int x684;                             // 0x684
    int x688;                             // 0x688
    int x68c;                             // 0x68c
    int x690;                             // 0x690
    int x694;                             // 0x694
    int x698;                             // 0x698
    int x69c;                             // 0x69c
    int x6a0;                             // 0x6a0
    int x6a4;                             // 0x6a4
    int x6a8;                             // 0x6a8
    float dmg_accumulator;                 // 0x6ac, cumulative damage taken, incremented by Machine_GiveDamage. Capped at a max value
    int x6b0;                             // 0x6b0
    int x6b4;                             // 0x6b4
    int x6b8;                             // 0x6b8
    int x6bc;                             // 0x6bc
    int x6c0;                             // 0x6c0
    int x6c4;                             // 0x6c4
    int x6c8;                             // 0x6c8
    int x6cc;                             // 0x6cc
    int x6d0;                             // 0x6d0
    int x6d4;                             // 0x6d4
    int x6d8;                             // 0x6d8
    int x6dc;                             // 0x6dc
    int x6e0;                             // 0x6e0
    int x6e4;                             // 0x6e4
    int x6e8;                             // 0x6e8
    int x6ec;                             // 0x6ec
    int x6f0;                             // 0x6f0
    int x6f4;                             // 0x6f4
    CollData *coll_data;                  // 0x6f8, machine's mpColl CollData (created at spawn; mpColl_Update target in Machine_InitialCollisionCheck/SetMpCollPosition; passed to the mpColl query helpers each frame in Machine_ProcessEnvColl). The sphere radius lives at coll_data->radius (+0x344) and coll_data->shape_data->radius/radius2 (+0x30/+0x34).
    int x6fc;                             // 0x6fc
    int x700;                             // 0x700
    int x704;                             // 0x704
    int x708;                             // 0x708
    int x70c;                             // 0x70c
    int x710;                             // 0x710
    int x714;                             // 0x714
    int x718;                             // 0x718
    int x71c;                             // 0x71c
    int x720;                             // 0x720
    int x724;                             // 0x724
    int x728;                             // 0x728
    int x72c;                             // 0x72c
    int ground_type_prev;                 // 0x730
    int ground_type_cur;                  // 0x734
    int x738;                             // 0x738
    int x73c;                             // 0x73c
    int x740;                             // 0x740
    int ground_traction_cur;              // 0x744
    int x748;                             // 0x748
    int x74c;                             // 0x74c
    int x750;                             // 0x750
    int action_state_class;               // 0x754, 0 = grounded states, 1 = launched/airborne; splits PlayerStats drive-time & distance buckets
    int x758;                             // 0x758
    int x75c;                             // 0x75c
    int x760;                             // 0x760
    int x764;                             // 0x764
    int x768;                             // 0x768
    int x76c;                             // 0x76c
    int x770;                             // 0x770
    int x774;                             // 0x774
    int x778;                             // 0x778
    int x77c;                             // 0x77c
    int x780;                             // 0x780
    int x784;                             // 0x784
    int x788;                             // 0x788
    float charge_value;                   // 0x78c current charge level, ranges 0.0-1.0
    int charge_full_timer;                // 0x790 counts down from charge_full_duration when charge hits 1.0; auto-discharges at 0
    int charge_cooldown_timer;            // 0x794 post-discharge cooldown; blocks charging while nonzero
    float charge_display_value;           // 0x798 mirror of charge_value, written each frame for HUD display
    int x79c;                             // 0x79c
    int x7a0;                             // 0x7a0
    int x7a4;                             // 0x7a4
    int x7a8;                             // 0x7a8
    int x7ac;                             // 0x7ac
    int x7b0;                             // 0x7b0
    int x7b4;                             // 0x7b4
    int x7b8;                             // 0x7b8
    int x7bc;                             // 0x7bc
    int x7c0;                             // 0x7c0
    int x7c4;                             // 0x7c4
    int x7c8;                             // 0x7c8
    int x7cc;                             // 0x7cc
    int x7d0;                             // 0x7d0
    int x7d4;                             // 0x7d4
    int x7d8;                             // 0x7d8
    int x7dc;                             // 0x7dc
    int x7e0;                             // 0x7e0
    int x7e4;                             // 0x7e4
    int x7e8;                             // 0x7e8
    int x7ec;                             // 0x7ec
    int x7f0;                             // 0x7f0
    int x7f4;                             // 0x7f4
    int x7f8;                             // 0x7f8
    int x7fc;                             // 0x7fc
    int x800;                             // 0x800
    int x804;                             // 0x804
    int x808;                             // 0x808
    int x80c;                             // 0x80c
    int x810;                             // 0x810
    int x814;                             // 0x814
    int x818;                             // 0x818
    int x81c;                             // 0x81c
    int x820;                             // 0x820
    int x824;                             // 0x824
    int x828;                             // 0x828
    int x82c;                             // 0x82c
    int x830;                             // 0x830
    int x834;                             // 0x834
    int x838;                             // 0x838
    int x83c;                             // 0x83c
    int x840;                             // 0x840
    struct
    {
        AudioEmitter emitter;               // 0x844
        int track;                          // 0x848
        int track2;                         // 0x84c, used when releasing a charge
        int track3;                         // 0x850, used during nice landings
        int track4;                         // 0x854, used when collecting a patch
        int track5;                         // 0x858, read every state change
        float radius;                       // 0x85c
        int x860;                           // 0x860
        int x864;                           // 0x864
        int x868;                           // 0x868
        int x86c;                           // 0x86c
        int x870;                           // 0x870
        int x874;                           // 0x874
        int x878;                           // 0x878
        int x87c_fgm_instance;              // 0x87c
        int x880;                           // 0x880
        int x884;                           // 0x884
        int charge_fgm_instance;            // 0x888
        int charge_fgm_kind;                // 0x88c
        int x890;                           // 0x890
        int x894;                           // 0x894
        int x898;                           // 0x898
        int x89c;                           // 0x89c
        int x8a0;                           // 0x8a0
        FGMInstance skid_fgm_instance;      // 0x8a4
    } audio;
    float respawn_pos[3];                 // 0x8a8, checkpoint as mpColl spline params: [0]=segment index (uint), [1]=progress (float), [2]=Y offset (float). Updated per-frame by Machine_UpdateCheckpoint. Initialized to {0xFFFFFFFF, 0.0, 0.0}.
    float prev_respawn_pos[3];            // 0x8b4, previous frame's respawn_pos, used as intermediate by checkpoint update
    float backup_respawn_pos[3];          // 0x8c0, last-known-good checkpoint, saved when spline lookup fails. Used by Machine_CheckFallDeath OOB-distance path when xc37 bit 6 is set.
    int x8cc;                             // 0x8cc
    int x8d0;                             // 0x8d0
    int x8d4;                             // 0x8d4
    int x8d8;                             // 0x8d8
    int x8dc;                             // 0x8dc
    int x8e0;                             // 0x8e0
    int x8e4;                             // 0x8e4
    int x8e8;                             // 0x8e8
    int x8ec;                             // 0x8ec
    int x8f0;                             // 0x8f0
    int x8f4;                             // 0x8f4
    int x8f8;                             // 0x8f8
    int x8fc;                             // 0x8fc
    int x900;                             // 0x900
    int x904;                             // 0x904
    int x908;                             // 0x908
    int x90c;                             // 0x90c
    int x910;                             // 0x910
    int x914;                             // 0x914
    int x918;                             // 0x918
    int x91c;                             // 0x91c
    int x920;                             // 0x920
    int x924;                             // 0x924
    int x928;                             // 0x928
    int x92c;                             // 0x92c
    int x930;                             // 0x930
    int x934;                             // 0x934
    int x938;                             // 0x938
    int x93c;                             // 0x93c
    int x940;                             // 0x940
    int x944;                             // 0x944
    int x948;                             // 0x948
    union {                               // 0x94C, live patch stats - source 0 of the 5 stat arrays summed by Machine_GetStatRatio (see note below). Machine_GivePatch/GiveAllUp/ApplyStatClamped write here; seeded from PlayerData.stats via the spawn descriptor; read back into PlayerData by cityTrial_getMasterStats on writeback
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
    // --- Per-stat contribution arrays (0x970-0xA08) ------------------------
    // Four more per-stat arrays parallel to `stats` (0x94C) above. All five are
    // summed element-wise by Machine_GetStatRatio: ratio = (stats + stat_aux +
    // statcap_a + statcap_b + statcap_c) / Patch_GetMaxValue(), clamped to [0,1];
    // Machine_GetStatRatio2 lerps that ratio across the vcDataCommon attr pair.
    // statcap_* are zero-inited at spawn (Machine_InitRuntimeState), cleared
    // per-group by Machine_SetStatCap (item pickup) or all-groups by 0x801caf90;
    // special items write per-stat cap points into them via the item attr list.
    int statcap_a[9];                     // 0x970, stat-cap source group 0 (integer points)
    int statcap_b[9];                     // 0x994, stat-cap source group 1
    int statcap_c[9];                     // 0x9b8, stat-cap source group 2 (clearing this group also resets charge timer 0x790)
    int statcap_scalar[3];                // 0x9dc, one scalar per statcap group; read+cleared with its group (old value passed to 0x801d5e34)
    float stat_aux[9];                    // 0x9e8, secondary per-stat floats, seeded from PlayerData.stat_aux at spawn (0 in practice), reset to 0 by 0x801cae1c
    int xa0c;                             // 0xa0c
    int xa10;                             // 0xa10
    int xa14;                             // 0xa14
    float hp;                             // 0xa18, current HP. Seeded from spawn desc[0xc]; if sentinel 0.0, defaults to hp_max_override
    float hp_max_override;                // 0xa1c, spawn-desc HP-max override (desc[0xd]): if != 0.0, copied into hp_max (0x4cc) by Machine_AdjustAttributes
    TriggerData trigger;                  // 0xa20
    int xa80;                             // 0xa80
    int xa84;                             // 0xa84
    int xa88;                             // 0xa88
    int xa8c;                             // 0xa8c
    int xa90;                             // 0xa90
    int xa94;                             // 0xa94
    int xa98;                             // 0xa98
    int xa9c;                             // 0xa9c
    int xaa0;                             // 0xaa0
    int xaa4;                             // 0xaa4
    int xaa8;                             // 0xaa8
    int xaac;                             // 0xaac
    int xab0;                             // 0xab0
    int xab4;                             // 0xab4
    int xab8;                             // 0xab8
    int xabc;                             // 0xabc
    int xac0;                             // 0xac0
    int xac4;                             // 0xac4
    int xac8;                             // 0xac8
    int xacc;                             // 0xacc
    int xad0;                             // 0xad0
    int xad4;                             // 0xad4
    int xad8;                             // 0xad8
    int xadc;                             // 0xadc
    int xae0;                             // 0xae0
    int xae4;                             // 0xae4
    int xae8;                             // 0xae8
    int xaec;                             // 0xaec
    int xaf0;                             // 0xaf0
    int xaf4;                             // 0xaf4
    int xaf8;                             // 0xaf8
    int xafc;                             // 0xafc
    int xb00;                             // 0xb00
    int xb04;                             // 0xb04
    int xb08;                             // 0xb08
    int xb0c;                             // 0xb0c
    int xb10;                             // 0xb10
    int xb14;                             // 0xb14
    int xb18;                             // 0xb18
    int xb1c;                             // 0xb1c
    int xb20;                             // 0xb20
    int xb24;                             // 0xb24
    int xb28;                             // 0xb28
    int xb2c;                             // 0xb2c
    int xb30;                             // 0xb30
    int xb34;                             // 0xb34
    int xb38;                             // 0xb38
    int xb3c;                             // 0xb3c
    int wall_stuck_timer;                 // 0xb40, counts DOWN from 120 when becoming airborne. wall stuck only gets checked if this is 0
    int xb44;                             // 0xb44
    int xb48;                             // 0xb48
    int xb4c;                             // 0xb4c
    int xb50;                             // 0xb50
    int xb54;                             // 0xb54
    int xb58;                             // 0xb58
    int xb5c;                             // 0xb5c
    int xb60;                             // 0xb60
    int xb64;                             // 0xb64
    int xb68;                             // 0xb68
    int xb6c;                             // 0xb6c
    int xb70;                             // 0xb70
    int xb74;                             // 0xb74
    int xb78;                             // 0xb78
    int xb7c;                             // 0xb7c
    int xb80;                             // 0xb80
    int xb84;                             // 0xb84
    int xb88;                             // 0xb88
    int xb8c;                             // 0xb8c
    int xb90;                             // 0xb90
    int xb94;                             // 0xb94
    int xb98;                             // 0xb98
    int xb9c;                             // 0xb9c
    int xba0;                             // 0xba0
    int xba4;                             // 0xba4
    int xba8;                             // 0xba8
    DmgLog dmg_log;                       // 0xbac
    int xbcc;                             // 0xbcc
    int xbd0;                             // 0xbd0
    int xbd4;                             // 0xbd4
    void (*xbd8)(GOBJ *m);                // 0xbd8
    void (*xbdc)(GOBJ *m);                // 0xbdc
    void (*xbe0)(GOBJ *m);                // 0xbe0
    void (*xbe4)(GOBJ *m);                // 0xbe4
    void (*xbe8)(GOBJ *m);                // 0xbe8
    void (*xbec)(GOBJ *m);                // 0xbec
    void (*xbf0)(GOBJ *m);                // 0xbf0
    void (*xbf4)(GOBJ *m);                // 0xbf4
    void (*xbf8)(GOBJ *m);                // 0xbf8
    int xbfc;                             // 0xbfc
    int xc00;                             // 0xc00
    int xc04;                             // 0xc04
    int xc08;                             // 0xc08
    int xc0c;                             // 0xc0c
    int xc10;                             // 0xc10
    int xc14;                             // 0xc14
    int xc18;                             // 0xc18
    int xc1c;                             // 0xc1c
    int xc20;                             // 0xc20
    int xc24;                             // 0xc24
    int xc28;                             // 0xc28
    int xc2c;                             // 0xc2c
    u8 charge_is_playing_skid_sfx : 1;    // 0xc30, 0x80
    u8 charge_is_grounded : 1;            // 0xc30, 0x40. flag that dictates whether kirby should gain charge when holding A. it actually is raised when the machine touches the ground? bikes always seem to have this raised
    u8 xc30_20 : 1;                       // 0xc30, 0x20
    u8 xc30_10 : 1;                       // 0xc30, 0x10
    u8 xc30_08 : 1;                       // 0xc30, 0x08
    u8 xc30_04 : 1;                       // 0xc30, 0x04
    u8 xc30_02 : 1;                       // 0xc30, 0x02
    u8 xc30_01 : 1;                       // 0xc30, 0x01
    u8 pad_c31[3];                        // 0xc31, padding to 0xc34
    u8 xc34;                              // 0xc34
    u8 is_fall_dead : 1;                   // 0xc35, 0x80, set by Machine_SetFallDead
    u8 xc35_40 : 1;                       // 0xc35, 0x40
    u8 is_dead : 1;                       // 0xc35, 0x20
    u8 xc36;                              // 0xc36
    u8 xc37_80 : 1;                       // 0xc37, 0x80. set by Machine_SetFallDead
    u8 use_backup_checkpoint : 1;         // 0xc37, 0x40. set when spline lookup fails, cleared on success; selects backup_respawn_pos over respawn_pos in Machine_CheckFallDeath
    u8 xc37_20 : 1;                       // 0xc37, 0x20
    u8 xc37_10 : 1;                       // 0xc37, 0x10. cleared by Machine_RespawnStateEntry
    u8 xc37_08 : 1;                       // 0xc37, 0x08
    u8 xc37_04 : 1;                       // 0xc37, 0x04
    u8 xc37_02 : 1;                       // 0xc37, 0x02
    u8 xc37_01 : 1;                       // 0xc37, 0x01. cleared by Machine_InitRuntimeState
    u8 xc38;                              // 0xc38, flag byte; low 6 bits cleared by Machine_InitRuntimeState
    u8 xc39;                              // 0xc39, flag byte (low nibble updated by Machine_SetFallDead)
    u8 xc3a;                              // 0xc3a, flag byte
    // 0xc3b: per-vehicle model/variant flag byte, populated by the vehicle's
    // model-setup callback (vcDataCommon+0x18) at spawn. The sign bit
    // (suppress_attr_recalc) is set only for the special transformation star
    // variants (Wing Kirby kind 0x11 and Compact Star kind 0x1) whose derived
    // attributes are fixed rather than patch-driven; Machine_GivePatch /
    // Machine_GiveAllUp skip Machine_AdjustAttributes when it is set.
    // Bits declared MSB-first (0x80 -> 0x01): on big-endian PPC, GCC assigns the
    // first-declared bitfield to bit 0x80. Order must match the xc30/xc35/xc37
    // groups so each named bit lands on its commented mask.
    u8 suppress_attr_recalc : 1;          // 0xc3b, 0x80. set for the transformation star variants (Wing Kirby 0x11, Compact 0x1); gates the Machine_AdjustAttributes call in Machine_GivePatch/GiveAllUp
    u8 xc3b_40 : 1;                       // 0xc3b, 0x40. set with suppress_attr_recalc by the wing/compact model-setup callback
    u8 xc3b_20 : 1;                       // 0xc3b, 0x20
    u8 xc3b_10 : 1;                       // 0xc3b, 0x10. from spawn desc byte 0x81 bit 4 (Machine_StoreVcDataPtr)
    u8 xc3b_08 : 1;                       // 0xc3b, 0x08. from spawn desc byte 0x81 bit 1 (Machine_StoreVcDataPtr)
    u8 xc3b_04 : 1;                       // 0xc3b, 0x04. set by Machine_SetFallDead path; cleared by Machine_InitRuntimeState
    u8 xc3b_02 : 1;                       // 0xc3b, 0x02. ground-type state flag
    u8 xc3b_01 : 1;                       // 0xc3b, 0x01. set for Wing Meta Knight (kind 0x12) variant
    u8 xc3c;                              // 0xc3c, flag byte; bits cleared by Machine_InitRuntimeState
    u8 pad_c3d[0x1b48 - 0xc3d];           // 0xc3d, unmapped interior gap
    // --- Fall-death / respawn block (0x1b48-0x1b68) ------------------------
    // Written by Machine_SetFallDead (0x801e6520) and Machine_RespawnStateEntry
    // (0x801e1ae8) when the machine falls off the course. respawn_spline_params
    // are mpColl spline params (segment index / progress / Y offset), not world
    // XYZ; ground_handle resolves the surface the respawn rides on.
    int fall_ground_handle;               // 0x1b48, ground handle for the fall-death respawn surface (Machine_SetFallDead arg)
    float respawn_spline_params[3];       // 0x1b4c, mpColl spline params at the fall-death respawn point
    int fall_timestamp;                   // 0x1b58, frame timestamp captured at fall death (GameData+0x16c)
    Vec3 respawn_pos_world;               // 0x1b5c, respawn world position (written by Machine_RespawnStateEntry)
    u8 pad_1b68[0x1bbd - 0x1b68];         // 0x1b68, unmapped interior gap
    u8 x1bbd;                             // 0x1bbd, flag byte; bit 0x01 set by Machine_StoreVcDataPtr at spawn
    u8 pad_1bbe[0x1bc0 - 0x1bbe];         // 0x1bbe, tail padding to the full 0x1bc0 allocation
    // NOTE: the real MachineData allocation is 0x1bc0 bytes (memset by
    // Machine_StoreVcDataPtr). Large interior gaps (the pad_* members) remain
    // unmapped; the fields above are those read/written by named functions.
} MachineData;

static vcDataCommon **stc_vcDataCommon = (vcDataCommon **)(0x805dd0e0 + 0x758);
static vcDataKindStar **stc_vcDataKindStar = (vcDataKindStar **)(0x805dd0e0 + 0x770);
static vcDataLookup *stc_vcDataLookup = (vcDataLookup *)0x8055a068;
static GOBJ *stc_machinespawn_gobj = (GOBJ *)(0x805dd0e0 + 0x780); // has data MachineSpawnData

GOBJ *Machine_Create(MachineSpawnDesc *desc);
int Machine_GetRiderPly(MachineData *md);
void Machine_SetMaxHP(MachineData *md);
void Machine_GiveIntangibility(MachineData *md, int time);
void Machine_ApplyColAnim(MachineData *md, int col_anim, int unk);
void Machine_ApplyStatClamped(float *stat_arr, int stat_idx, int delta); // 0x801e094c, adds delta to stat_arr[stat_idx] and clamps to [Patch_GetMinValue, Patch_GetMaxValue]
void Machine_ApplyAllStatsClamped(float *stat_arr, int delta); // 0x801e096c, adds delta to all 9 stats and clamps each
void Machine_SetStatBlockClamped(float *dst, float *src); // 0x80194f64, copies 9 stats src->dst, clamping each to [Patch_GetMinValue, Patch_GetMaxValue]. Used to push a rider's stat block into the machine's added-patch array (MachineData+0x9e8)
void cityTrial_setMasterStats(GOBJ *machine_gobj, float *stats); // 0x801c8258, clamps stats into the machine's added-patch array (-> Machine_SetStatBlockClamped) then Machine_AdjustAttributes; the rider->machine half of the City Trial stat sync (Ply_SetStatAux calls it)
void cityTrial_getMasterStats(GOBJ *machine_gobj, float *out_stats); // 0x801c81c0, copies the machine's master stat block (MachineData+0x94c) into out_stats; the machine->rider half, run each frame while riding
void Machine_UpdateAppearance(MachineData *md); // 0x801d6668, updates machine visual state: stat glow, candy, charge, invincibility, and vehicle-specific effects
void Machine_AdjustAttributes(MachineData *md); // 0x801c7278, recalculates derived machine attributes from stats. Dispatches per-vehicle via vcDataCommon+0x1c (attribute memcpy) and +0x20 (Machine_AdjustAttributes{Star,Bike})
float Machine_GetStatRatio(MachineData *md, int stat_idx);  // 0x801caa8c, returns sum(per-stat source contributions: floats at md+0x94C/+0x9E8 plus ints at +0x970/+0x994/+0x9B8) / Patch_GetMaxValue(), clamped to [0,1]
float Machine_GetStatRatio2(MachineData *md, int stat_idx); // 0x801cabd4, like Machine_GetStatRatio but returns the interpolated attribute value: lerps the summed ratio across the per-stat min/max attribute pair from stc_vcDataCommon+0x1c (fmadds), not the raw clamped ratio
float Machine_ScaleFromRatio(float *low_high_pair, float ratio);  // 0x801cab4c, bipolar interpolator: ratio>0 -> 1 + ratio*(high-1); ratio<0 -> 1 + (-ratio)*(low-1); ratio==0 -> 1.0
float Machine_ScaleFromRatio2(float *low_high_pair, float ratio); // 0x801cab94, sibling of Machine_ScaleFromRatio (near-identical structure, variant TBD)
void Machine_ApplyStarStatScaling(MachineData *md); // 0x801e81e4, per-stat scaling loop for star machines; calls Machine_GetStatRatio then Machine_ScaleFromRatio against attribute pairs in vcDataKindStar
void Machine_ApplyBikeStatScaling(MachineData *md); // 0x801f3d44, per-stat scaling loop for wheelie bikes; same pattern, bike-specific attribute offsets
void Machine_AdjustAttributesStar(MachineData *md); // 0x801e906c, vcDataCommon+0x20 callback for star machines; wraps Machine_ApplyStarStatScaling + post-adjustments
void Machine_AdjustAttributesBike(MachineData *md); // 0x801f4dac, vcDataCommon+0x20 callback for wheelie bikes; wraps Machine_ApplyBikeStatScaling + post-adjustments
void Machine_SetupModelWing(MachineData *md); // 0x801e7ad4, vcDataCommon+0x18 model-setup callback for the wing variants (Wing Kirby 0x11, Wing Meta Knight 0x12); configures model-section animations and writes the variant flag byte at 0xc3b (sets suppress_attr_recalc for Wing Kirby)
void Machine_SetupModelStar(MachineData *md); // 0x801f37d4, vcDataCommon+0x18 model-setup callback for star variants (e.g. Compact 0x1, Slick 0x6); same pattern, writes the variant flag byte at 0xc3b (sets suppress_attr_recalc for Compact)
void Machine_GivePatch(MachineData *, PatchKind, int num);
void Machine_GiveAllUp(MachineData *, int num);
void Machine_OnTouchItem(MachineData *, ItemData *);
int Machine_IsDead(MachineData *);
void Machine_SetFallDead(MachineData *md, int ground_handle, float *respawn_pos); // 0x801e6520. Triggers fall-off-course death: stores ground_handle at md+0x1B48, respawn_pos[3] at md+0x1B4C, timestamp at md+0x1B58. respawn_pos is mpColl spline params (not world XYZ). Vanilla callers: Machine_CheckFallDeath passes md->respawn_pos or md->backup_respawn_pos based on xc37 bit 6.
int Machine_GetGroundHandle(int surface_id); // 0x80247fac. Resolves a surface ID (from the collision result) to a ground handle for Machine_SetFallDead
void Machine_SetStatCap(MachineData *md, int stat_group_index); // types 13-19 handler, writes stat cap for kinds 21-26 (SPEEDMAX-CHARGENONE)
void Machine_ModifyStatByKind(MachineData *md, int kind, float value); // type 22 handler, modifies a stat by item kind
void Machine_GiveFood(MachineData *md, int flag, float amount); // heals HP, flag=1 triggers SFX
int Machine_IsLowHP(MachineData *md); // returns 1 if hp < threshold * hp_max (health threshold check, NOT invincibility)
void Machine_HealTick(MachineData *md); // fixed-amount heal, simplified variant of Machine_GiveFood
void Machine_GiveCandy(MachineData *md, int duration); // applies candy visual effect (rainbow color anim), clears hurt data. duration param unused
void Machine_GivePatchOrCandy(MachineData *md, int type, float amount); // dispatches type 27 = candy, types 21-24 = patches
void Machine_PatchPickupEffect(MachineData *md, int patch_kind); // visual/SFX effect on patch pickup
void Machine_ApplyHurt(void *hurt_subsystem, int index, HurtParams *hurt_params); // 0x8018d1a8. Applies hurt via HitColl system. hurt_subsystem = MachineData.hurt_data, index = 0, hurt_params = 0x34-byte struct from Trigger_ClearParameterStruct. Calls Trigger_InitParameters then HitColl_SetDamageLog (see hurt.h)
void Machine_GiveDamage(MachineData *md, float damage, GOBJ *source_gobj); // 0x801e1ee8. High-level damage: adds to dmg accumulator (MachineData+0x6AC), subtracts HP (MachineData+0xA18), triggers death if HP<=0, applies low-HP color anim. Checks Gm_IsDamageEnabled() before HP reduction. float is passed in f1. source_gobj is used by Machine_OnDamageVisual for hit spark direction (reads +0x20 as forward vector); must not be NULL in City Trial. Does NOT cause knockback/bounce
void Machine_EnterHitReaction(MachineData *md);        // 0x801e05bc. Transitions machine to hit reaction state (state 5). Causes the "bounce up" animation. Checks if not already in state 5. Calls Machine_OnEnterHitReaction which triggers MachineStateChange to substate 9, plays hit reaction animation. Set HurtData.kb_mag before calling for knockback physics
void Machine_ActOnHitCollision(MachineData *md);       // 0x801d7308. Per-frame post-collision step inside Machine_UpdateHitColl. Reads HurtData.kb_mag (offset 0x24); if 0, returns immediately. Otherwise looks up the strongest log entry via HurtData.hitcoll_log_idx (0x1C) and dispatches by attacker kind into the damage / knockback / state-transition handlers (HP loss, Machine_EnterHitReaction, etc.); fully consumes a logged hit.
void Machine_InitHurtData(MachineData *md);            // 0x801d6e84. Creates HurtData for machine via HurtData_Create(HURTKIND_MACHINE), sets callback at HurtData+0x8C, configures hurt descriptors from itData
HurtData *MachineGObj_GetHurtData(GOBJ *machine_gobj); // 0x801c8660. Returns *(MachineData+0x660) from GOBJ userdata

// Reads the machine's projectile_inherit_velocity Vec3 (md+0x354) into *out.
// Used by the rider-level projectile spawners (spawnBomb/spawnGordo/...) to
// seed the projectile's initial velocity before adding rider self_vel.
void MachineGObj_GetProjectileBaseVelocity(GOBJ *machine_gobj, Vec3 *out); // 0x801c7628

// Returns the machine's projectile_charge_scale (md+0x4e8), multiplied by a
// constant 1.0. Used by the rider-level projectile spawners to populate
// ProjectileDesc.charge.
float Machine_GetProjectileChargeScale(MachineData *md);                   // 0x801d7e28
float MachineGObj_GetProjectileChargeScale(GOBJ *machine_gobj);            // 0x801c868c, unwraps gobj->userdata and calls Machine_GetProjectileChargeScale

AudioEmitter Machine_AllocAudioEmitter(int index);
#endif