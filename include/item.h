#ifndef MEX_H_ITEM
#define MEX_H_ITEM

#include "structs.h"
#include "datatypes.h"
#include "os.h"
#include "audio.h"
#include "hurt.h"
#include "trigger.h"

typedef enum ItemPri
{
    ITPRI_0,
    ITPRI_ANIM,
    ITPRI_PHYS = 4,
    ITPRI_ENVCOLL,
    ITPRI_6,
    ITPRI_TRIGGER, // collect powerup collision, also frees sounds
    ITPRI_8,
    ITPRI_HITCOLL,
    ITPRI_DMGAPPLY,
    ITPRI_13 = 13,
    ITPRI_15 = 15,
} ItemPri;

typedef enum BoxKind
{
    BOXKIND_ALL = -1,
    BOXKIND_BLUE,
    BOXKIND_GREEN,
    BOXKIND_RED,
    BOXKIND_NUM,
} BoxKind;

static const char *const BoxKind_Names[BOXKIND_NUM] = {
    [BOXKIND_BLUE]  = "Blue Box",
    [BOXKIND_GREEN] = "Green Box",
    [BOXKIND_RED]   = "Red Box",
};

typedef enum BoxSize
{
    BOXSIZE_SMALL,  // spawns 1 item @ 80250bcc
    BOXSIZE_MEDIUM, // spawns 2 items @ 80250bb4
    BOXSIZE_LARGE,  // spawns 4 item @ 80250bc4
} BoxSize;

typedef enum ItemType
{
    ITTYPE_NULL = -1, //
    ITTYPE_PATCH,     //
    ITTYPE_QUICKFIX,  //
    ITTYPE_COPY,      //
    ITTYPE_NUM,       //
} ItemType;

typedef enum ItemGroup
{
    ITGROUP_ALL = -1, //
    ITGROUP_BAD,      //
    ITGROUP_GOOD,     //
    ITGROUP_FAKE,     //
    ITGROUP_NUM,      //
} ItemGroup;

typedef enum ItemKind
{
    ITKIND_BOXBLUE,
    ITKIND_BOXGREEN,
    ITKIND_BOXRED,
    ITKIND_ACCEL,
    ITKIND_ACCELDOWN,
    ITKIND_TOPSPEED,
    ITKIND_TOPSPEEDDOWN,
    ITKIND_OFFENSE,
    ITKIND_OFFENSEDOWN,
    ITKIND_DEFENSE,
    ITKIND_DEFENSEDOWN,
    ITKIND_TURN,
    ITKIND_TURNDOWN,
    ITKIND_GLIDE,
    ITKIND_GLIDEDOWN,
    ITKIND_CHARGE,
    ITKIND_CHARGEDOWN,
    ITKIND_WEIGHT,
    ITKIND_WEIGHTDOWN,
    ITKIND_HP,
    ITKIND_ALLUP,
    ITKIND_SPEEDMAX,
    ITKIND_SPEEDMIN,
    ITKIND_OFFENSEMAX,
    ITKIND_DEFENSEMAX,
    ITKIND_CHARGEMAX,
    ITKIND_CHARGENONE,
    ITKIND_CANDY,
    ITKIND_COPYBOMB,
    ITKIND_COPYFIRE,
    ITKIND_COPYICE,
    ITKIND_COPYSLEEP,
    ITKIND_COPYTIRE,
    ITKIND_COPYBIRD,
    ITKIND_COPYPLASMA,
    ITKIND_COPYTORNADO,
    ITKIND_COPYSWORD,
    ITKIND_COPYSPIKE,
    ITKIND_COPYMIC,
    ITKIND_FOODMAXIMTOMATO,
    ITKIND_FOODENERGYDRINK,
    ITKIND_FOODICECREAM,
    ITKIND_FOODRICEBALL,
    ITKIND_FOODCHICKEN,
    ITKIND_FOODCURRY,
    ITKIND_FOODRAMEN,
    ITKIND_FOODOMELET,
    ITKIND_FOODHAMBURGER,
    ITKIND_FOODSUSHI,
    ITKIND_FOODHOTDOG,
    ITKIND_FOODAPPLE,
    ITKIND_FIREWORKS,
    ITKIND_PANICSPIN,
    ITKIND_SENSORBOMB,
    ITKIND_GORDO,
    ITKIND_HYDRA1,
    ITKIND_HYDRA2,
    ITKIND_HYDRA3,
    ITKIND_DRAGOON1,
    ITKIND_DRAGOON2,
    ITKIND_DRAGOON3,
    ITKIND_ACCELFAKE,
    ITKIND_TOPSPEEDFAKE,
    ITKIND_OFFENSEFAKE,
    ITKIND_DEFENSEFAKE,
    ITKIND_TURNFAKE,
    ITKIND_GLIDEFAKE,
    ITKIND_CHARGEFAKE,
    ITKIND_WEIGHTFAKE,
    ITKIND_NUM,
} ItemKind;

static const char *const ItemKind_Names[ITKIND_NUM] = {
    [ITKIND_BOXBLUE]         = "Blue Box",
    [ITKIND_BOXGREEN]        = "Green Box",
    [ITKIND_BOXRED]          = "Red Box",
    [ITKIND_ACCEL]           = "Boost",
    [ITKIND_ACCELDOWN]       = "Boost Down",
    [ITKIND_TOPSPEED]        = "Top Speed",
    [ITKIND_TOPSPEEDDOWN]    = "Top Speed Down",
    [ITKIND_OFFENSE]         = "Offense",
    [ITKIND_OFFENSEDOWN]     = "Offense Down",
    [ITKIND_DEFENSE]         = "Defense",
    [ITKIND_DEFENSEDOWN]     = "Defense Down",
    [ITKIND_TURN]            = "Turn",
    [ITKIND_TURNDOWN]        = "Turn Down",
    [ITKIND_GLIDE]           = "Glide",
    [ITKIND_GLIDEDOWN]       = "Glide Down",
    [ITKIND_CHARGE]          = "Charge",
    [ITKIND_CHARGEDOWN]      = "Charge Down",
    [ITKIND_WEIGHT]          = "Weight",
    [ITKIND_WEIGHTDOWN]      = "Weight Down",
    [ITKIND_HP]              = "HP",
    [ITKIND_ALLUP]           = "All Up",
    [ITKIND_SPEEDMAX]        = "Speed Max",
    [ITKIND_SPEEDMIN]        = "Speed Min",
    [ITKIND_OFFENSEMAX]      = "Offense Max",
    [ITKIND_DEFENSEMAX]      = "Defense Max",
    [ITKIND_CHARGEMAX]       = "Charge Max",
    [ITKIND_CHARGENONE]      = "No Charge",
    [ITKIND_CANDY]           = "Candy",
    [ITKIND_COPYBOMB]        = "Bomb",
    [ITKIND_COPYFIRE]        = "Fire",
    [ITKIND_COPYICE]         = "Ice",
    [ITKIND_COPYSLEEP]       = "Sleep",
    [ITKIND_COPYTIRE]        = "Wheel",
    [ITKIND_COPYBIRD]        = "Wing",
    [ITKIND_COPYPLASMA]      = "Plasma",
    [ITKIND_COPYTORNADO]     = "Tornado",
    [ITKIND_COPYSWORD]       = "Sword",
    [ITKIND_COPYSPIKE]       = "Needle",
    [ITKIND_COPYMIC]         = "Mic",
    [ITKIND_FOODMAXIMTOMATO] = "Maxim Tomato",
    [ITKIND_FOODENERGYDRINK] = "Energy Drink",
    [ITKIND_FOODICECREAM]    = "Ice Cream",
    [ITKIND_FOODRICEBALL]    = "Rice Ball",
    [ITKIND_FOODCHICKEN]     = "Chicken",
    [ITKIND_FOODCURRY]       = "Curry",
    [ITKIND_FOODRAMEN]       = "Ramen",
    [ITKIND_FOODOMELET]      = "Omelet",
    [ITKIND_FOODHAMBURGER]   = "Hamburger",
    [ITKIND_FOODSUSHI]       = "Sushi",
    [ITKIND_FOODHOTDOG]      = "Hot Dog",
    [ITKIND_FOODAPPLE]       = "Apple",
    [ITKIND_FIREWORKS]       = "Fireworks",
    [ITKIND_PANICSPIN]       = "Panic Spin",
    [ITKIND_SENSORBOMB]      = "Sensor Bomb",
    [ITKIND_GORDO]           = "Gordo",
    [ITKIND_HYDRA1]          = "Hydra Part X",
    [ITKIND_HYDRA2]          = "Hydra Part Y",
    [ITKIND_HYDRA3]          = "Hydra Part Z",
    [ITKIND_DRAGOON1]        = "Dragoon Part A",
    [ITKIND_DRAGOON2]        = "Dragoon Part B",
    [ITKIND_DRAGOON3]        = "Dragoon Part C",
    [ITKIND_ACCELFAKE]       = "Boost Fake",
    [ITKIND_TOPSPEEDFAKE]    = "Top Speed Fake",
    [ITKIND_OFFENSEFAKE]     = "Offense Fake",
    [ITKIND_DEFENSEFAKE]     = "Defense Fake",
    [ITKIND_TURNFAKE]        = "Turn Fake",
    [ITKIND_GLIDEFAKE]       = "Glide Fake",
    [ITKIND_CHARGEFAKE]      = "Charge Fake",
    [ITKIND_WEIGHTFAKE]      = "Weight Fake",
};

typedef enum PatchKind
{
    PATCHKIND_WEIGHT,
    PATCHKIND_ACCEL,
    PATCHKIND_TOPSPEED,
    PATCHKIND_TURN,
    PATCHKIND_CHARGE,
    PATCHKIND_GLIDE,
    PATCHKIND_OFFENSE,
    PATCHKIND_DEFENSE,
    PATCHKIND_HP,
    PATCHKIND_NUM,
} PatchKind;

static const char *const PatchKind_Names[PATCHKIND_NUM] = {
    [PATCHKIND_WEIGHT]   = "Weight",
    [PATCHKIND_ACCEL]    = "Boost",
    [PATCHKIND_TOPSPEED] = "Top Speed",
    [PATCHKIND_TURN]     = "Turn",
    [PATCHKIND_CHARGE]   = "Charge",
    [PATCHKIND_GLIDE]    = "Glide",
    [PATCHKIND_OFFENSE]  = "Offense",
    [PATCHKIND_DEFENSE]  = "Defense",
    [PATCHKIND_HP]       = "HP",
};

// Per-kind static effect-info: list of stat changes a patch grants on pickup.
// NULL for non-patch items. The `group` field here (BAD/GOOD/FAKE) is the
// authoritative ItemGroup for the kind - not the dead mirror at ItemCommonAttr.x24.
typedef struct PatchEffectInfo
{
    struct
    {
        int type;           // 0x0, effect type: 0-8 = PatchKind, 9 = AllUp, 0xb-0x25 = special
        float value;        // 0x4, effect value (e.g. 1.0 for +1 stat)
    } *entries;             // 0x0
    int count;              // 0x4, number of entries
    ItemGroup group;        // 0x8, BAD=0, GOOD=1, FAKE=2
} PatchEffectInfo;

// Per-kind static attribute table loaded from ItCommon.dat. Most fields are
// dead in retail (written by CityItem_CopyCommonAttr to ItemData+0x118 mirror,
// never read). Only scale_factor, cull_distance, land_offset, box_kind, and
// effect_info are actually consumed.
typedef struct ItemCommonAttr
{
    float scale_factor;     // 0x00, multiplied with ItemData.scale for rendering
    int x4;                 // 0x04, dead - no readers
    float cull_distance;    // 0x08, used by Item_GX shadow-cull test (zz_80255fc4_)
    float land_offset;      // 0x0c, vertical offset above ground surface on landing/raycast
    int x10;                // 0x10, dead - no readers
    int x14;                // 0x14, dead - no readers
    int x18;                // 0x18, dead - no readers
    BoxKind box_kind;       // 0x1c, which color box pool this item spawns from (live)
    int x20;                // 0x20, dead - no readers
    int x24;                // 0x24, dead - real ItemGroup lives in effect_info->group
    PatchEffectInfo *effect_info; // 0x28, NULL for non-patch items
} ItemCommonAttr;

// Per-kind tail data, copied at runtime into ItemData.x144 by the per-kind
// init function (state-table slot +0xC). The buffer is allocated by
// CityItem_AllocUniqueAttr from the pool descriptor at 0x8055dddc with a
// fixed 0x38-byte stride - the box family maximum.
//
// Of the 68 ItemKinds, 53 share a copy-1-int template that's never read back
// (dead carry-over from the box family's init template). The 3 box kinds
// (BOXBLUE/GREEN/RED) actually populate structured fields. Only the box layout
// is documented here.
typedef union ItemUniqueAttr
{
    struct                      // BOXBLUE / BOXGREEN / BOXRED only - sizeof = 0x38
    {
        float x00;              // 0x00, copied to buffer (no readers found)
        float x04;              // 0x04, copied to buffer (no readers found)
        float rotation_rate;    // 0x08, applied to ItemData.x394 in box rotation update (0x80257a20)
        float x0c;              // 0x0c, copied to buffer (no readers found)
        int   timer_min;        // 0x10, used DIRECT in box pre_init for ItemData.x364 seed
        int   timer_max;        // 0x14, used DIRECT in box pre_init
        float wrap_accumulator; // 0x18, summed into ItemData.x15c at 0x802576ac
        float x1c;              // 0x1c
        float x20;              // 0x20
        float x24;              // 0x24
        float x28;              // 0x28
        float x2c;              // 0x2c
        float x30;              // 0x30
        float effect_scale;     // 0x34, scaled into effect-spawn position at 0x8025710c
    } box;
    int x00;                    // all other 53 kinds - single int copied to ItemData.x144 + 0x00; never read back
} ItemUniqueAttr;

typedef struct itData
{
    ItemCommonAttr *attr;       // 0x0
    ItemUniqueAttr *unique_attr; // 0x4, per-kind tail data - only the box family is structured
    struct
    {
        JOBJ *j;
        int x4;
    } *model;                   // 0x8
    struct
    {
        AnimJointDesc *joint_anim;
        MatAnimJointDesc *mat_anim;
        void *script;
        int joint_num;
    } *anim_data;               // 0xc
    struct
    {
        HurtDesc *desc;         // 0x0
        int num;                // 0x4
    } *hurt;                    // 0x10
    TriggerDesc *trigger;       // 0x14
} itData;

// Patch-toss physics descriptor (24 bytes). Two of these live inside
// ItemCommonParam (offsets 0x5C and 0x80) - one for "good" tosses, one for
// "bad". Consumed by CityItem_BeginPatchToss (0x80256254) when a machine
// hits/touches a patch and the patch is ejected from the box.
typedef struct PatchTossDesc
{
    float speed_min;       // 0x00, lower bound of horizontal toss-speed range
    float speed_max;       // 0x04, upper bound (range = max - min)
    float horiz_vel_scale; // 0x08, multiplier on rider direction → toss horizontal velocity
    float drift_accel;     // 0x0c, per-frame horizontal drift acceleration
    int initial_state;     // 0x10, state-machine kind written to ItemData+0x82
    float vert_vel_scale;  // 0x14, multiplier on vertical component (sign flipped between good/bad)
    int initial_timer;     // 0x18, frames-in-state seed for ItemData+0x85
    float anim_rate_div;   // 0x1c, divisor for animation rate
} PatchTossDesc;

// Global City Trial item parameters loaded from files/ItCommon.dat (root
// symbol "itCommonDataAll", first member). Single instance; pointed to by
// stc_item_param. Fields beyond 0xA4 form a variable-length table indexed by
// PatchKind × history-position used by the patch-toss flight code.
typedef struct ItemCommonParam
{
    float scale;                   // 0x00, global scale multiplier on every spawned item
    float hardmode_scale_mult;     // 0x04, secondary multiplier when difficulty/checklist gate is active
    float shadow_scale_alt;        // 0x08, shadow-size multiplier when ItemData.x24 != 0
    float shadow_scale;            // 0x0c, default shadow-size multiplier
    // 0x10..0x1c: dead padding - populated from ItCommon.dat at boot but never
    // read by any code path.
    float x10;                     // 0x10
    float x14;                     // 0x14
    float x18;                     // 0x18
    float x1c;                     // 0x1c
    int box_hit_intangibility_frames; // 0x20, post-hit invuln frames applied to a box's HurtData
    int lifetime_min;              // 0x24, base item lifetime (frames)
    int lifetime_variance;         // 0x28, HSD_Randi(variance) added to lifetime_min
    int child_item_lifetime;       // 0x2c, lifetime assigned to items popped out of a broken box
    float box_spawn_offset_min_h;  // 0x30, horizontal scatter range min (Box_OutcomeLogic)
    float box_spawn_offset_max_h;  // 0x34, horizontal scatter range max
    float box_spawn_offset_min_v;  // 0x38, vertical scatter range min (all-up multi-spawn)
    float box_spawn_offset_max_v;  // 0x3c, vertical scatter range max
    float box_spawn_yaw_range;     // 0x40, max yaw rotation (passed to fctiwz → HSD_Randi → RotateVecAroundAxis)
    float gravity;                 // 0x44, downward acceleration applied to falling items
    float terminal_fall_speed;     // 0x48, max fall speed enforced via Item_LimitFallSpeed
    float bounce_tangential_damping;  // 0x4c, friction along surface during bounce
    float bounce_min_speed_threshold; // 0x50, below this post-damping speed, item stops bouncing
    float bounce_normal_blend_amount; // 0x54, blend toward surface normal on bounce
    float enter_fall_initial_grav_scale; // 0x58, single-purpose float consumed by zz_802579d4_
    PatchTossDesc patch_toss_good; // 0x5c, toss params when CityItem_IsGoodPatch() returns 1
    PatchTossDesc patch_toss_bad;  // 0x80, toss params when CityItem_IsGoodPatch() returns 0
    float patch_kind_throw_table[1][4]; // 0xa4, [PatchKind-1][history_idx 0..3] flight perturbation
} ItemCommonParam;

// Wrapper struct loaded from ItCommon.dat / "itCommonDataAll" symbol.
// stc_item_param, stc_it_common_data point at this and its first member.
typedef struct itCommonDataAll
{
    ItemCommonParam *param;     // 0x0, copied to stc_item_param at boot
    void *x4;                   // 0x4, secondary table - purpose unknown
    itData *itData;             // 0x8, per-kind data array (0x18-byte stride)
} itCommonDataAll;

typedef struct ItemFallDesc
{
    float match_progress; // time
    int item_max;         // maximum amount of items present
    int spawn_time_min;   // min
    int spawn_time_max;   // max
} ItemFallDesc;

typedef struct ItemDesc // used to spawn an item
{
    // Fields marked [computed] are set by Item_InitDesc internally.
    // Fields marked [param] come from Item_InitDesc parameters.
    // Fields marked [param?] come from stack params (r9/r10), purpose unclear.
    int x0;             // 0x00, [computed] always 0. Maps to ItemData[0x04]
    ItemKind kind;      // 0x04, [param] item kind
    int x8;             // 0x08, [param] r5 param of Item_InitDesc. Maps to ItemData[0x20]
    Vec3 pos;           // 0x0C, [param] spawn position
    Vec3 forward;       // 0x18, [param] normalized forward vector (can be NULL for defaults)
    Vec3 up;            // 0x24, [param] normalized up vector (can be NULL for defaults)
    float scale;        // 0x30, [param] item scale
    int exist_index;    // 0x34, [computed] City_GetItemExistNum(). This item is the nth to exist
    int x38;            // 0x38, [param] stack param 3 of Item_InitDesc. Maps to ItemData[0x34]. Usually -1
    int x3c;            // 0x3C, [param] stack param 4 of Item_InitDesc. Maps to ItemData[0x38]. Usually -1
    int x40;            // 0x40, [param] r9 of Item_InitDesc. Maps to ItemData[0x3C]. Usually -1
    int x44;            // 0x44, [param] r10 of Item_InitDesc. Maps to ItemData[0x40]. Usually -1
    int lifetime;       // 0x48, [computed] HSD_Randi(variance) + min from ItemCommonParam
    int coll_kind;      // 0x4C, [param] stack param 2 of Item_InitDesc. Stored to ItemData[0x359] bits 2-4. 3=point coll (most items), 1=alloc CollData, 0=requires CollData
    int is_airborne;    // 0x50, [param] stack param 1 of Item_InitDesc. -1=skip initial raycast, other=do raycast. Maps to ItemData[0x1D4]
    int x54;            // 0x54, [computed] always 0. Stored as bit flag in ItemData[0x35B]
    int flags;          // 0x58, [computed] by Item_InitDesc from the item kind range. Maps to ItemData[0x48]
} ItemDesc;

typedef struct ItemData
{
    GOBJ *item_gobj;            // 0x0, this item's GObj
    GOBJ *parent_gobj;          // 0x4, box GObj that spawned this item (NULL for sky-spawned items)
    GOBJ *child_gobjs[4];       // 0x8, child items spawned when this box breaks (up to 4)
    GOBJ *shadow_gobj;          // 0x18

    ItemKind kind;              // 0x1c
    int spawn_type;             // 0x20, from ItemDesc.x8, differentiates spawn contexts
    int item_category;          // 0x24, 0=box, non-0=powerup. Determines shadow size, bounce SFX
    JOBJDesc *jobjdesc;         // 0x28
    itData *itData;             // 0x2c
    int exist_index;            // 0x30, this item is the nth item to exist
    int x34;                    // 0x34, from ItemDesc.x38, usually -1
    int x38;                    // 0x38, from ItemDesc.x3c, usually -1
    int x3c;                    // 0x3c, from ItemDesc.x40, usually -1
    int x40;                    // 0x40, from ItemDesc.x44, usually -1
    int lifetime;               // 0x44, decremented each frame in ShadowThink, expire at 0
    int flags;                  // 0x48, from ItemDesc.flags, cleared conditionally per frame

    int state;                  // 0x4c, current state index
    int num_common_states;      // 0x50, count of states in common state table (typically 3)
    int anim_index;             // 0x54, current animation index (-1 = none)
    void *common_state_table;   // 0x58, pointer to shared state descriptors (all item kinds)
    void *kind_data;            // 0x5c, per-kind function/state data from item kind table
    void *current_anim;         // 0x60, pointer to current animation descriptor
    float script_timer;         // 0x64, script command countdown, decremented by anim_speed
    float state_frame;          // 0x68, = anim_frame + anim_overflow
    void *script_data;          // 0x6c, pointer to state script/command data
    int x70;                    // 0x70
    int x74;                    // 0x74
    int x78;                    // 0x78
    int x7c;                    // 0x7c
    int x80;                    // 0x80
    int x84;                    // 0x84

    float anim_frame;           // 0x88, current animation frame
    float anim_overflow;        // 0x8c, accumulated overflow from animation wrapping
    float anim_speed;           // 0x90, animation playback speed (1.0 = normal)
    void *jobj_array;           // 0x94, allocated array of JOBJ pointers for animation
    int x98;                    // 0x98
    void *mat_anim_array;       // 0x9c, allocated material animation data
    int xa0;                    // 0xa0
    void *shape_anim_array;     // 0xa4, allocated shape animation data

    float base_scale;           // 0xa8, from ItemDesc.scale * game_scale_factor
    float scale;                // 0xac, render scale (copied from base_scale, can be modified)
    float alpha;                // 0xb0
    float alpha_addend;         // 0xb4, added to alpha each frame (for fade in/out)

    Vec3 accel;                 // 0xb8, acceleration, added to vel each physics frame
    Vec3 vel;                   // 0xc4, velocity, added to pos each physics frame
    Vec3 pos_delta;             // 0xd0, pos - prev_pos, computed each frame in ShadowThink
    Vec3 pos;                   // 0xdc, current position
    Vec3 prev_pos;              // 0xe8, previous frame position, updated each frame
    int xf4;                    // 0xf4
    int xf8;                    // 0xf8
    int xfc;                    // 0xfc

    Vec3 forward;               // 0x100, normalized forward direction
    Vec3 up;                    // 0x10c, normalized up direction

    // Runtime copy of ItemCommonAttr fields, written by CityItem_CopyCommonAttr.
    // Most are dead mirrors - listed for offset accuracy, not for use.
    float attr_scale;           // 0x118, from ItemCommonAttr.scale_factor, used by transform helpers
    int attr_x04;               // 0x11c, dead mirror
    float attr_cull_distance;   // 0x120, from ItemCommonAttr.cull_distance, used in Item_GX shadow-cull test
    float attr_land_offset;     // 0x124, from ItemCommonAttr.land_offset, surface offset on landing
    int attr_x10;               // 0x128, dead mirror
    int attr_x14;               // 0x12c, dead mirror
    int attr_x18;               // 0x130, dead mirror
    int attr_box_kind;          // 0x134, dead mirror - box_kind is read directly via Item_GetCommonAttr
    int attr_x20;               // 0x138, dead mirror
    int attr_x24;               // 0x13c, dead mirror - real ItemGroup is read from effect_data->group when needed
    PatchEffectInfo *effect_data; // 0x140, NULL for non-patch items; consumed by Patch_GetEffectData / Machine_OnTouchItem
    void *x144;                 // 0x144, per-kind unique_attr buffer - allocated by CityItem_AllocUnkData and populated by per-kind init

    HurtData *hurt_data;        // 0x148, created by HurtData_Create during CityItem_InitHurtData
    int x14c;                   // 0x14c
    int x150;                   // 0x150
    int x154;                   // 0x154
    int x158;                   // 0x158
    int x15c;                   // 0x15c
    int x160;                   // 0x160
    int x164;                   // 0x164
    int x168;                   // 0x168
    int x16c;                   // 0x16c
    int x170;                   // 0x170
    int x174;                   // 0x174
    int x178;                   // 0x178
    int x17c;                   // 0x17c
    int x180;                   // 0x180
    int x184;                   // 0x184
    int x188;                   // 0x188
    int x18c;                   // 0x18c
    int x190;                   // 0x190
    int x194;                   // 0x194
    int x198;                   // 0x198
    int x19c;                   // 0x19c
    int x1a0;                   // 0x1a0

    CollData *coll_data;        // 0x1a4, map collision data (NULL = uses point collision only)
    struct                      // 0x1a8, items use point collision for ground detection
    {
        int raycast_idx;        // 0x1a8, collision ID from raycast result
        Vec3 land_pos;          // 0x1ac, ground position calculated by raycast
    } point_coll;               //
    int x1b8;                   // 0x1b8
    int x1bc;                   // 0x1bc
    int x1c0;                   // 0x1c0
    float x1c4;                 // 0x1c4
    Vec3 fall_dir;              // 0x1c8, gravity/down direction vector, used for ground raycasting
    int is_airborne;            // 0x1d4, from ItemDesc. if not -1, raycast/ground check is performed
    int x1d8;                   // 0x1d8, collision temp data (cleared as 3-word block)
    int x1dc;                   // 0x1dc
    int x1e0;                   // 0x1e0
    int x1e4;                   // 0x1e4
    int x1e8;                   // 0x1e8
    int x1ec;                   // 0x1ec
    int x1f0;                   // 0x1f0
    int x1f4;                   // 0x1f4
    int x1f8;                   // 0x1f8
    int x1fc;                   // 0x1fc
    int x200;                   // 0x200
    int x204;                   // 0x204
    int x208;                   // 0x208
    int x20c;                   // 0x20c
    int x210;                   // 0x210
    int x214;                   // 0x214
    int x218;                   // 0x218
    int x21c;                   // 0x21c
    int x220;                   // 0x220
    int damage_processed;       // 0x224, "damage taken this frame" latch - cleared at end of CityItem_ApplyDamageFromHurtData (proc priority 10)
    int x228;                   // 0x228
    int effect_gfx_a;           // 0x22c, particle/GFX effect handle
    int effect_gfx_b;           // 0x230, particle/GFX effect handle
    int x234;                   // 0x234
    int effect_timer_a;         // 0x238, effect animation timer
    int effect_timer_b;         // 0x23c, effect animation timer

    int audio_source;           // 0x240, audio source ID, -1 = not allocated. Set by Item_AllocAudioSource
    int audio_track;            // 0x244, audio track ID. Set by CityItem_AllocAudioTrack
    int audio_timer;            // 0x248, decrement-each-frame timer paired with x35a bit 6. When it hits 0, CityItem_FrameStartTick clears the pickup-lock bit and frees audio
    int bounce_num;             // 0x24c, incremented when bouncing @ 80255a70

    TriggerData trigger;        // 0x250, item pickup/touch collision
    int x2b0;                   // 0x2b0
    int x2b4;                   // 0x2b4
    int x2b8;                   // 0x2b8
    int x2bc;                   // 0x2bc
    int x2c0;                   // 0x2c0
    int x2c4;                   // 0x2c4
    int x2c8;                   // 0x2c8
    int x2cc;                   // 0x2cc
    int x2d0;                   // 0x2d0
    int x2d4;                   // 0x2d4
    int x2d8;                   // 0x2d8
    int x2dc;                   // 0x2dc
    int x2e0;                   // 0x2e0
    int x2e4;                   // 0x2e4
    int x2e8;                   // 0x2e8
    int x2ec;                   // 0x2ec
    int x2f0;                   // 0x2f0
    int x2f4;                   // 0x2f4
    int x2f8;                   // 0x2f8
    int x2fc;                   // 0x2fc
    int x300;                   // 0x300
    int x304;                   // 0x304
    int x308;                   // 0x308
    int x30c;                   // 0x30c
    int x310;                   // 0x310
    int x314;                   // 0x314
    int x318;                   // 0x318
    int x31c;                   // 0x31c
    int x320;                   // 0x320
    int x324;                   // 0x324

    // Set from the current state entry (5 ints per entry: anim_idx, 4 callbacks)
    void (*anim_callback)(GOBJ *); // 0x328, called each frame during AnimThink
    void (*physics_callback)();    // 0x32c, called each frame during PhysicsThink
    void (*envcoll_callback)();    // 0x330, called each frame during EnvColl
    void (*state_callback_d)();    // 0x334, 4th state callback
    int state_counter;          // 0x338, reset to 0 on each state change
    void (*on_damage_callback)(GOBJ *, void *); // 0x33c, called in HitColl when item takes damage
    void *x340;                 // 0x340
    int x344;                   // 0x344
    int x348;                   // 0x348
    int x34c;                   // 0x34c
    int x350;                   // 0x350
    float x354;                 // 0x354

    u8 x358;                    // 0x358, bit 5 (0x20) = visible_this_frame (set by Item_GX cull test)
    // 0x359 bitfield (big-endian, MSB-first allocation):
    //   bits 5-7 (0xE0) = x359_hi
    //   bits 2-4 (0x1C) = coll_kind (set in CityItem_AllocCollData, read in
    //                     Item_GenericEnvColl). 3=point coll (most items), 1=alloc
    //                     CollData, 0=requires CollData (dangerous).
    //   bits 0-1 (0x03) = x359_lo
    u8 x359_hi : 3;             // 0x359, 0xE0
    u8 coll_kind : 3;           // 0x359, 0x1C, collision kind from ItemDesc
    u8 x359_lo : 2;             // 0x359, 0x03
    // x35a bits:
    //   bit 0 (0x01) = spawned-from-sky (set by CityItem_MarkAsSkySpawned, used by power-up handlers)
    //   bit 4 (0x10) = is_grounded (set when item lands; cleared on state change)
    //   bit 5 (0x20) = persistent pickup-lock - "this is a box, never collectible" (set by box init)
    //   bit 6 (0x40) = temporary pickup-lock - spawn-anim or audio busy (paired with audio_timer)
    //   bit 7 (0x80) = cleared on every state change; gates trigger/coll debug overlay in Item_GX
    // CityItem_CanCollect returns 1 iff bits 5 AND 6 are both clear.
    u8 x35a;                    // 0x35a
    // x35b bits:
    //   bit 5 (0x20) = model_hidden (mirrors JOBJ_HIDDEN on the rendered jobj)
    //   bit 6 (0x40) = caller-supplied init flag from ItemDesc (purpose unclear)
    //   bit 7 (0x80) = settled / no-further-state-work - set by CityItem_SetX35bBit7
    //                  from per-state-kind callbacks at end-of-state (suspected: rest state)
    u8 x35b;                    // 0x35b

    int forced_item;            // 0x35c, predetermined ItemKind for box contents. -1 = random, -2 = no items
    int break_timer;            // 0x360, set to 8 on Box_Break
    int x364;                   // 0x364
    int x368;                   // 0x368
    int x36c;                   // 0x36c
    int x370;                   // 0x370
    int x374;                   // 0x374
    int x378;                   // 0x378
    int x37c;                   // 0x37c
    int x380;                   // 0x380
    int x384;                   // 0x384
    int x388;                   // 0x388
    int x38c;                   // 0x38c
    int x390;                   // 0x390

    // CityItem_Create allocates 0x3a0 bytes via HSD_ObjAlloc
    u16 time_seconds;           // 0x394
    u8 stadium_kind;            // 0x396
    u8 game_tempo : 2;          // 0x397, 0xC0 (1 = normal = 2 is slow)
    u8 events_enable : 1;       // 0x397, 0x20
    u8 x397_x10 : 1;            // 0x397, 0x10
    u8 x397_x08 : 1;            // 0x397, 0x08
    u8 x397_x04 : 1;            // 0x397, 0x04
    u8 x397_x02 : 1;            // 0x397, 0x02
    u8 x397_x01 : 1;            // 0x397, 0x01
    int x398;                   // 0x398
    int x39c;                   // 0x39c
    int x3a8;                   // 0x3a8
    int x3ac;                   // 0x3ac
    int x3b0;                   // 0x3b0
    int x3b4;                   // 0x3b4
    int x3b8;                   // 0x3b8
    int x3bc;                   // 0x3bc
    int x3c0;                   // 0x3c0
    int x3c4;                   // 0x3c4
    int x3c8;                   // 0x3c8
    int x3cc;                   // 0x3cc
    int x3d0;                   // 0x3d0
    int x3d4;                   // 0x3d4
    int x3d8;                   // 0x3d8
    int x3dc;                   // 0x3dc
    int x3e0;                   // 0x3e0
    int x3e4;                   // 0x3e4
    int x3e8;                   // 0x3e8
    int x3ec;                   // 0x3ec
    int x3f0;                   // 0x3f0
    int x3f4;                   // 0x3f4
    int x3f8;                   // 0x3f8
    int x3fc;                   // 0x3fc
    int x400;                   // 0x400
    int x404;                   // 0x404
    int x408;                   // 0x408
    int x40c;                   // 0x40c
    int x410;                   // 0x410
    int x414;                   // 0x414
    int x418;                   // 0x418
    int x41c;                   // 0x41c
    int x420;                   // 0x420
    int x424;                   // 0x424
    int x428;                   // 0x428
    int x42c;                   // 0x42c
    int x430;                   // 0x430
    int x434;                   // 0x434
    int x438;                   // 0x438
    int x43c;                   // 0x43c
    int x440;                   // 0x440
    int x444;                   // 0x444
    int x448;                   // 0x448
    int x44c;                   // 0x44c
    int x450;                   // 0x450
    int x454;                   // 0x454
    int x458;                   // 0x458
    int x45c;                   // 0x45c
    int x460;                   // 0x460
    int x464;                   // 0x464
    int x468;                   // 0x468
    int x46c;                   // 0x46c
    int x470;                   // 0x470
    int x474;                   // 0x474
    int x478;                   // 0x478
    int x47c;                   // 0x47c
    int x480;                   // 0x480
    int x484;                   // 0x484
    int x488;                   // 0x488
    int x48c;                   // 0x48c
    int x490;                   // 0x490
    int x494;                   // 0x494
    int x498;                   // 0x498
    int x49c;                   // 0x49c
    int x4a0;                   // 0x4a0
    int x4a4;                   // 0x4a4
    int x4a8;                   // 0x4a8
    int x4ac;                   // 0x4ac
    int x4b0;                   // 0x4b0
    int x4b4;                   // 0x4b4
    int x4b8;                   // 0x4b8
    int x4bc;                   // 0x4bc
    int x4c0;                   // 0x4c0
    int x4c4;                   // 0x4c4
    int x4c8;                   // 0x4c8
    int x4cc;                   // 0x4cc
    int x4d0;                   // 0x4d0
    int x4d4;                   // 0x4d4
    int x4d8;                   // 0x4d8
    int x4dc;                   // 0x4dc
    int x4e0;                   // 0x4e0
    int x4e4;                   // 0x4e4
    int x4e8;                   // 0x4e8
    int x4ec;                   // 0x4ec
    int x4f0;                   // 0x4f0
    int x4f4;                   // 0x4f4
    int x4f8;                   // 0x4f8
    int x4fc;                   // 0x4fc
    int x500;                   // 0x500
    int x504;                   // 0x504
    int x508;                   // 0x508
    int x50c;                   // 0x50c
    int x510;                   // 0x510
    int x514;                   // 0x514
    int x518;                   // 0x518
    int x51c;                   // 0x51c
    int x520;                   // 0x520
    int x524;                   // 0x524
    int x528;                   // 0x528
    int x52c;                   // 0x52c
    int x530;                   // 0x530
    int x534;                   // 0x534
    int x538;                   // 0x538
    int x53c;                   // 0x53c
    int x540;                   // 0x540
    int x544;                   // 0x544
    int x548;                   // 0x548
    int x54c;                   // 0x54c
    int x550;                   // 0x550
    int x554;                   // 0x554
    int x558;                   // 0x558
    int x55c;                   // 0x55c
    int x560;                   // 0x560
    int x564;                   // 0x564
    int x568;                   // 0x568
    int x56c;                   // 0x56c
    int x570;                   // 0x570
    int x574;                   // 0x574
    int x578;                   // 0x578
    int x57c;                   // 0x57c
    int x580;                   // 0x580
    int x584;                   // 0x584
    int x588;                   // 0x588
    int x58c;                   // 0x58c
    int x590;                   // 0x590
    int x594;                   // 0x594
    int x598;                   // 0x598
    int x59c;                   // 0x59c
    int x5a0;                   // 0x5a0
    int x5a4;                   // 0x5a4
    int x5a8;                   // 0x5a8
    int x5ac;                   // 0x5ac
    int x5b0;                   // 0x5b0
    int x5b4;                   // 0x5b4
    int x5b8;                   // 0x5b8
    int x5bc;                   // 0x5bc
    int x5c0;                   // 0x5c0
    int x5c4;                   // 0x5c4
    int x5c8;                   // 0x5c8
    int x5cc;                   // 0x5cc
    int x5d0;                   // 0x5d0
    int x5d4;                   // 0x5d4
    int x5d8;                   // 0x5d8
    int x5dc;                   // 0x5dc
    int x5e0;                   // 0x5e0
    int x5e4;                   // 0x5e4
    int x5e8;                   // 0x5e8
    int x5ec;                   // 0x5ec
    int x5f0;                   // 0x5f0
    int x5f4;                   // 0x5f4
    int x5f8;                   // 0x5f8
    int x5fc;                   // 0x5fc
    int x600;                   // 0x600
    int x604;                   // 0x604
    int x608;                   // 0x608
    int x60c;                   // 0x60c
    int x610;                   // 0x610
    int x614;                   // 0x614
    int x618;                   // 0x618
    int x61c;                   // 0x61c
    int x620;                   // 0x620
    int x624;                   // 0x624
    int x628;                   // 0x628
    int x62c;                   // 0x62c
    int x630;                   // 0x630
    int x634;                   // 0x634
    int x638;                   // 0x638
    int x63c;                   // 0x63c
    int x640;                   // 0x640
    int x644;                   // 0x644
    int x648;                   // 0x648
    int x64c;                   // 0x64c
    int x650;                   // 0x650
    int x654;                   // 0x654
    int x658;                   // 0x658
    int x65c;                   // 0x65c
    int x660;                   // 0x660
    int x664;                   // 0x664
    int x668;                   // 0x668
    int x66c;                   // 0x66c
    int x670;                   // 0x670
    int x674;                   // 0x674
    int x678;                   // 0x678
    int x67c;                   // 0x67c
    int x680;                   // 0x680
    int x684;                   // 0x684
    int x688;                   // 0x688
    int x68c;                   // 0x68c
    int x690;                   // 0x690
    int x694;                   // 0x694
    int x698;                   // 0x698
    int x69c;                   // 0x69c
    int x6a0;                   // 0x6a0
    int x6a4;                   // 0x6a4
    int x6a8;                   // 0x6a8
    int x6ac;                   // 0x6ac
    int x6b0;                   // 0x6b0
    int x6b4;                   // 0x6b4
    int x6b8;                   // 0x6b8
    int x6bc;                   // 0x6bc
    int x6c0;                   // 0x6c0
    int x6c4;                   // 0x6c4
    int x6c8;                   // 0x6c8
    int x6cc;                   // 0x6cc
    int x6d0;                   // 0x6d0
    int x6d4;                   // 0x6d4
    int x6d8;                   // 0x6d8
    int x6dc;                   // 0x6dc
    int x6e0;                   // 0x6e0
    int x6e4;                   // 0x6e4
    int x6e8;                   // 0x6e8
    int x6ec;                   // 0x6ec
    int x6f0;                   // 0x6f0
    int x6f4;                   // 0x6f4
    int x6f8;                   // 0x6f8
    int x6fc;                   // 0x6fc
    int x700;                   // 0x700
    int x704;                   // 0x704
    int x708;                   // 0x708
    int x70c;                   // 0x70c
    int x710;                   // 0x710
    int x714;                   // 0x714
    int x718;                   // 0x718
    int x71c;                   // 0x71c
    int x720;                   // 0x720
    int x724;                   // 0x724
    int x728;                   // 0x728
    int x72c;                   // 0x72c
    int x730;                   // 0x730
    int x734;                   // 0x734
    int x738;                   // 0x738
    int x73c;                   // 0x73c
    int x740;                   // 0x740
    int x744;                   // 0x744
    int x748;                   // 0x748
    int x74c;                   // 0x74c
    int x750;                   // 0x750
    int x754;                   // 0x754
    int x758;                   // 0x758
    int x75c;                   // 0x75c
    int x760;                   // 0x760
    int x764;                   // 0x764
    int x768;                   // 0x768
    int x76c;                   // 0x76c
    int x770;                   // 0x770
    int x774;                   // 0x774
    int x778;                   // 0x778
    int x77c;                   // 0x77c
    int x780;                   // 0x780
    int x784;                   // 0x784
    int x788;                   // 0x788
    int x78c;                   // 0x78c
    int x790;                   // 0x790
    int x794;                   // 0x794
    int x798;                   // 0x798
    int x79c;                   // 0x79c
    int x7a0;                   // 0x7a0
    int x7a4;                   // 0x7a4
    int x7a8;                   // 0x7a8
    int x7ac;                   // 0x7ac
    int x7b0;                   // 0x7b0
    int x7b4;                   // 0x7b4
    int x7b8;                   // 0x7b8
    int x7bc;                   // 0x7bc
    int x7c0;                   // 0x7c0
    int x7c4;                   // 0x7c4
    int x7c8;                   // 0x7c8
    int x7cc;                   // 0x7cc
    int x7d0;                   // 0x7d0
    int x7d4;                   // 0x7d4
    int x7d8;                   // 0x7d8
    int x7dc;                   // 0x7dc
    int x7e0;                   // 0x7e0
    int x7e4;                   // 0x7e4
    int x7e8;                   // 0x7e8
    int x7ec;                   // 0x7ec
    int x7f0;                   // 0x7f0
    int x7f4;                   // 0x7f4
    int x7f8;                   // 0x7f8
    int x7fc;                   // 0x7fc
    int x800;                   // 0x800
    int x804;                   // 0x804
    int x808;                   // 0x808
    int x80c;                   // 0x80c
    int x810;                   // 0x810
    int x814;                   // 0x814
    int x818;                   // 0x818
    int x81c;                   // 0x81c
    int x820;                   // 0x820
    int x824;                   // 0x824
    int x828;                   // 0x828
    int x82c;                   // 0x82c
    u8 x830;                    // 0x830
    u8 x831;                    // 0x831
    u8 x832;                    // 0x832
    u8 frames_in_second;        // 0x833
    int seconds_passed;         // 0x834
    int x838;                   // 0x838
    int x83c;                   // 0x83c
    int x840;                   // 0x840
    int x844;                   // 0x844
    int x848;                   // 0x848
    int x84c;                   // 0x84c
    int x850;                   // 0x850
    int x854;                   // 0x854
    int x858;                   // 0x858
    int x85c;                   // 0x85c
    int x860;                   // 0x860
    int x864;                   // 0x864
    int x868;                   // 0x868
    int x86c;                   // 0x86c
    int x870;                   // 0x870
    int x874;                   // 0x874
    int x878;                   // 0x878
    int x87c;                   // 0x87c
    int x880;                   // 0x880
    int x884;                   // 0x884
    int x888;                   // 0x888
    int x88c;                   // 0x88c
    int x890;                   // 0x890
    int x894;                   // 0x894
    int x898;                   // 0x898
    int x89c;                   // 0x89c
    int x8a0;                   // 0x8a0
    int x8a4;                   // 0x8a4
    int x8a8;                   // 0x8a8
    int x8ac;                   // 0x8ac
    int x8b0;                   // 0x8b0
    int x8b4;                   // 0x8b4
    int x8b8;                   // 0x8b8
    int x8bc;                   // 0x8bc
    int x8c0;                   // 0x8c0
    int x8c4;                   // 0x8c4
    int x8c8;                   // 0x8c8
    int x8cc;                   // 0x8cc
    int x8d0;                   // 0x8d0
    int x8d4;                   // 0x8d4
    int x8d8;                   // 0x8d8
    int x8dc;                   // 0x8dc
    int x8e0;                   // 0x8e0
    int x8e4;                   // 0x8e4
    int x8e8;                   // 0x8e8
    int x8ec;                   // 0x8ec
    int x8f0;                   // 0x8f0
    int x8f4;                   // 0x8f4
    int x8f8;                   // 0x8f8
    int x8fc;                   // 0x8fc
    int x900;                   // 0x900
    int x904;                   // 0x904
    int x908;                   // 0x908
    int x90c;                   // 0x90c
    int x910;                   // 0x910
    int x914;                   // 0x914
    int x918;                   // 0x918
    int x91c;                   // 0x91c
    int x920;                   // 0x920
    int x924;                   // 0x924
    int x928;                   // 0x928
    int x92c;                   // 0x92c
    int x930;                   // 0x930
    int x934;                   // 0x934
    int x938;                   // 0x938
    int x93c;                   // 0x93c
    int x940;                   // 0x940
    int x944;                   // 0x944
    int x948;                   // 0x948
    int x94c;                   // 0x94c
    int x950;                   // 0x950
    int x954;                   // 0x954
    int x958;                   // 0x958
    int x95c;                   // 0x95c
    int x960;                   // 0x960
    int x964;                   // 0x964
    int x968;                   // 0x968
    int x96c;                   // 0x96c
    int x970;                   // 0x970
    int x974;                   // 0x974
    int x978;                   // 0x978
    int x97c;                   // 0x97c
    int x980;                   // 0x980
    int x984;                   // 0x984
    int x988;                   // 0x988
    int x98c;                   // 0x98c
    int x990;                   // 0x990
    int x994;                   // 0x994
    int x998;                   // 0x998
    int x99c;                   // 0x99c
    int x9a0;                   // 0x9a0
    int x9a4;                   // 0x9a4
    int x9a8;                   // 0x9a8
    int x9ac;                   // 0x9ac
    int x9b0;                   // 0x9b0
    int x9b4;                   // 0x9b4
    int x9b8;                   // 0x9b8
    int x9bc;                   // 0x9bc
    int x9c0;                   // 0x9c0
    int x9c4;                   // 0x9c4
    int x9c8;                   // 0x9c8
    int x9cc;                   // 0x9cc
    int x9d0;                   // 0x9d0
    int x9d4;                   // 0x9d4
    int x9d8;                   // 0x9d8
    int x9dc;                   // 0x9dc
    int x9e0;                   // 0x9e0
    int x9e4;                   // 0x9e4
    int x9e8;                   // 0x9e8
    int x9ec;                   // 0x9ec
    int x9f0;                   // 0x9f0
    int x9f4;                   // 0x9f4
    int x9f8;                   // 0x9f8
    int x9fc;                   // 0x9fc
    int xa00;                   // 0xa00
    int xa04;                   // 0xa04
    int xa08;                   // 0xa08
    int xa0c;                   // 0xa0c
    int xa10;                   // 0xa10
    int xa14;                   // 0xa14
    int xa18;                   // 0xa18
    int xa1c;                   // 0xa1c
    int xa20;                   // 0xa20
    int xa24;                   // 0xa24
    int xa28;                   // 0xa28
    int xa2c;                   // 0xa2c
    int xa30;                   // 0xa30
    int xa34;                   // 0xa34
    int xa38;                   // 0xa38
    int xa3c;                   // 0xa3c
    int xa40;                   // 0xa40
    int xa44;                   // 0xa44
    int xa48;                   // 0xa48
    int xa4c;                   // 0xa4c
    int xa50;                   // 0xa50
    int xa54;                   // 0xa54
    int xa58;                   // 0xa58
    int xa5c;                   // 0xa5c
    int xa60;                   // 0xa60
    int xa64;                   // 0xa64
    int xa68;                   // 0xa68
    int xa6c;                   // 0xa6c
    int xa70;                   // 0xa70
    int xa74;                   // 0xa74
    int xa78;                   // 0xa78
    int xa7c;                   // 0xa7c
    int xa80;                   // 0xa80
    int xa84;                   // 0xa84
    int xa88;                   // 0xa88
    int xa8c;                   // 0xa8c
    int xa90;                   // 0xa90
    int xa94;                   // 0xa94
    int xa98;                   // 0xa98
    int xa9c;                   // 0xa9c
    int xaa0;                   // 0xaa0
    int xaa4;                   // 0xaa4
    int xaa8;                   // 0xaa8
    int xaac;                   // 0xaac
    int xab0;                   // 0xab0
    int xab4;                   // 0xab4
    int xab8;                   // 0xab8
    int xabc;                   // 0xabc
    int xac0;                   // 0xac0
    int xac4;                   // 0xac4
    int xac8;                   // 0xac8
    int xacc;                   // 0xacc
    int xad0;                   // 0xad0
    int xad4;                   // 0xad4
    int xad8;                   // 0xad8
    int xadc;                   // 0xadc
    int xae0;                   // 0xae0
    int xae4;                   // 0xae4
    int xae8;                   // 0xae8
    int xaec;                   // 0xaec
    int xaf0;                   // 0xaf0
    int xaf4;                   // 0xaf4
    int xaf8;                   // 0xaf8
    int xafc;                   // 0xafc
    int xb00;                   // 0xb00
    int xb04;                   // 0xb04
    int xb08;                   // 0xb08
    int xb0c;                   // 0xb0c
    int xb10;                   // 0xb10
    int xb14;                   // 0xb14
    int xb18;                   // 0xb18
    int xb1c;                   // 0xb1c
    int xb20;                   // 0xb20
    int xb24;                   // 0xb24
    int xb28;                   // 0xb28
    int xb2c;                   // 0xb2c
    int xb30;                   // 0xb30
    int xb34;                   // 0xb34
    int xb38;                   // 0xb38
    int xb3c;                   // 0xb3c
    int xb40;                   // 0xb40
    int xb44;                   // 0xb44
    int xb48;                   // 0xb48
    int xb4c;                   // 0xb4c
    int xb50;                   // 0xb50
    int xb54;                   // 0xb54
    int xb58;                   // 0xb58
    int xb5c;                   // 0xb5c
    int xb60;                   // 0xb60
} ItemData;

// City Trial event-mode flag bits (CityItemMgr.flags). Each bit has paired
// set/clear functions at 0x80254144..0x802542C0; setters are called from the
// matching event_*_start handler and clearers from the corresponding _end.
typedef enum CityEventSpawnFlag
{
    CTEVF_DYNABLADE     = 1 << 0, // event_dynablade_start  → SetDynabladeEventFlag
    CTEVF_METEOR        = 1 << 1, // event_meteor_start     → SetMeteorEventFlag
    CTEVF_LOCATOR       = 1 << 2, // event_rubberyItems_start → InitLocatorEvent (also sets loc_pos/params)
    CTEVF_FAKEITEMS     = 1 << 3, // event_fakeItems_start  → InitFakeEvent
    CTEVF_SAMEITEMS     = 1 << 4, // event_sameItems_start  → SetSameItemsEventFlag
} CityEventSpawnFlag;

// City Trial item manager (0x1c8 bytes), allocated once at boot by Item_InitObj
// and reached via stc_city_item_mgr. Holds CT-wide item bookkeeping: live count,
// lifetime spawn counter, per-event flag word, and locator/fake-event payloads.
typedef struct CityItemMgr
{
    s32 live_item_count;        // 0x000  ++ in CityItem_Create, -- on destruction (cap = 100)
    s32 lifetime_spawn_count;   // 0x004  ++ only, read by City_GetItemSpawnNumber
    u8 _scaffold[0x1A4];        // 0x008  5 × 84-byte structures, each a doubly-linked chain of four
                                //         16-byte chunks plus a self-ptr at +0x40 and 16 reserved
                                //         bytes. Initialized by Item_InitObj but never read elsewhere
                                //         in retail (vestigial / scaffolded subsystem).
    u32 flags;                  // 0x1ac  CityEventSpawnFlag bitmask of currently-active CT events
    Vec3 loc_pos;               // 0x1b0  locator-event spawn position; valid iff flags & CTEVF_LOCATOR
    f32 loc_param0;             // 0x1bc  locator-event aux (valid iff flags & CTEVF_LOCATOR)
    f32 loc_param1;             // 0x1c0  locator-event aux (valid iff flags & CTEVF_LOCATOR)
    void *fake_event_data;      // 0x1c4  set by CityItem_InitFakeEvent; sticky (not cleared by ClearFakeEvent)
} CityItemMgr;

static ItemCommonParam **stc_item_param      = (ItemCommonParam **)(0x805dd0e0 + 0x7E8); // 0x805dd8c8
static CityItemMgr     **stc_city_item_mgr   = (CityItemMgr **)(0x805dd0e0 + 0x7EC);     // 0x805dd8cc

// Top Ride item kinds - bitmask indices for TopRideItemMgr.enabled_mask (+0x24).
// Mystery (a2dIT21 "?") is NOT in the bitmask - it's always available as the roulette item.
// Slot 12 (PARTY_BALL_ALT, KirbyKusdama) is the engine's twin Party Ball variant.
typedef enum TopRideItemKind
{
    TRITEM_HAMMER,           // 0  a2dIT1e AC_hammer
    TRITEM_BIG_CAKE,         // 1  a2dIT01 AC_macron
    TRITEM_SPEED_UP,         // 2  a2dIT02 AC_speedUp
    TRITEM_SPEED_DOWN,       // 3  a2dIT03 AC_speedDown
    TRITEM_SPINNER,          // 4  a2dIT04 AC_BoostUp_Missile (charge saw attack)
    TRITEM_CHARGE_TANK,      // 5  a2dIT0c AC_chargeUp
    TRITEM_INVINCIBLE_CANDY, // 6  a2dIT0d AC_muteki
    TRITEM_BUZZ_SAW,         // 7  a2dIT0a AC_Sdrill_kusudama
    TRITEM_DRILL,            // 8  a2dIT05 AC_FrontSpeer
    TRITEM_FREEZE_FAN,       // 9  a2dIT1b AC_ice
    TRITEM_MISSILE,          // 10 a2dIT07 AC_BoostUp_Missile (projectile missile)
    TRITEM_FIRE,             // 11 a2dIT06 AC_AfterFlame
    TRITEM_PARTY_BALL_ALT,   // 12 a2dIT0b AC_Sdrill_kusudama - KirbyKusdama, twin Party Ball variant
    TRITEM_BOMB,             // 13 a2dIT08 AC_bomb
    TRITEM_STEP_BOOM,        // 14 a2dIT10 AC_landbomb
    TRITEM_LANTERN,          // 15 a2dIT11 AC_lanthanum - "New Item: Lantern" (TR checklist reward 10)
    TRITEM_WALKY,            // 16 a2dIT16 AC_mike
    TRITEM_KRACKO,           // 17 a2dIT12 AC_clakko
    TRITEM_WHO_PAINT,        // 18 a2dIT13 AC_meta - "New Item: Who? Paint" (TR checklist reward 9)
    TRITEM_SMOKESCREEN,      // 19 a2dIT17 AC_kemuron
    TRITEM_CHICKIE,          // 20 a2dIT18 AC_piyo - "New Item: Chickie" (TR checklist reward 8)
    TRITEM_PARTY_BALL,       // 21 a2dIT20 AC_usiro - KirbyUshiroyurerun, canonical Party Ball slot
    TRITEM_NUM,
} TopRideItemKind;

static const char *const TopRideItemKind_Names[TRITEM_NUM] = {
    [TRITEM_HAMMER]           = "Hammer",
    [TRITEM_BIG_CAKE]         = "Big Cake",
    [TRITEM_SPEED_UP]         = "Speed Up",
    [TRITEM_SPEED_DOWN]       = "Speed Down",
    [TRITEM_SPINNER]          = "Spinner",
    [TRITEM_CHARGE_TANK]      = "Charge Tank",
    [TRITEM_INVINCIBLE_CANDY] = "Invincible Candy",
    [TRITEM_BUZZ_SAW]         = "Buzz Saw",
    [TRITEM_DRILL]            = "Drill",
    [TRITEM_FREEZE_FAN]       = "Freeze Fan",
    [TRITEM_MISSILE]          = "Missile",
    [TRITEM_FIRE]             = "Fire",
    [TRITEM_PARTY_BALL_ALT]   = "Party Ball (alt)",
    [TRITEM_BOMB]             = "Bomb",
    [TRITEM_STEP_BOOM]        = "Step-boom",
    [TRITEM_LANTERN]          = "Lantern",
    [TRITEM_WALKY]            = "Walky",
    [TRITEM_KRACKO]           = "Kracko",
    [TRITEM_WHO_PAINT]        = "Who? Paint",
    [TRITEM_SMOKESCREEN]      = "Smokescreen",
    [TRITEM_CHICKIE]          = "Chickie",
    [TRITEM_PARTY_BALL]       = "Party Ball",
};

// Top Ride ItemMgr - C++ singleton (RTTI name "ItemMgr"). Manages which items
// can spawn during a Top Ride match. Initialized by TopRideItem_MgrInit (0x8034b5f4).
typedef struct TopRideItemMgr
{
    void *vtable;               // 0x00
    u8 *stage_data;             // 0x04, pointer to per-stage config table; bytes at +0x38/+0x40/+0x41
                                //       select stage-specific spawn modes / weight tables. Stage
                                //       configuration, not runtime race state.
    int timer;                  // 0x08
    int archive_data;           // 0x0C
    int x10;                    // 0x10
    int x14;                    // 0x14
    int x18;                    // 0x18
    int x1c;                    // 0x1C
    int x20;                    // 0x20
    u32 enabled_mask;           // 0x24, bitmask of which items can spawn (bits 0-22)
    float x28;                  // 0x28
} TopRideItemMgr;

// Top Ride ItemMgr singleton pointer (r13 + 0xAC4). Set during Top Ride 3D scene init.
static TopRideItemMgr **stc_topride_itemmgr = (TopRideItemMgr **)(0x805dd0e0 + 0xAC4);

// Spawns a Top Ride item GObj at a given position and links it into the
// ItemMgr's active list; it then behaves like any other spawned TR item and is
// collected on kirby collision. Vanilla callers pass flag1=0, flag2=1.
void TopRideItem_SpawnAtPosition(TopRideItemMgr *mgr, int item_kind, Vec3 *pos, Vec3 *orient, uint flag1, uint flag2); // 0x8034bf50

// Returns a pointer to the per-item data blob for a TopRide item kind (0..21).
// Offset +0x10 of the returned struct is the float spawn weight used by the
// weighted-random pickers in TopRideItem_SpawnTimed and TopRideItem_PartyBallUpdate.
// Out-of-range kinds fall through to `return kind` (invalid pointer), so only
// call with 0..TRITEM_NUM-1.
const void *TopRideItem_GetDataByIndex(int kind); // 0x8034d204

// Forward decl - full definition in topride.h. Declared here as a typedef
// (matching topride.h) so callers don't have to include topride.h just to
// use TopRide_KirbyApplyItem.
typedef struct TopRideKirby TopRideKirby;

// Per-kind effect dispatcher: applies a TopRide item's effect to a Kirby
// directly. This is the same dispatcher invoked by the per-frame consume path
// (TopRide_KirbyUpdate -> Absorber consume). Out-of-range kinds (< 0 or >= 22)
// silently no-op. Requires kirby+0x7c (held item GObj) to be non-null - true
// during active gameplay (round_state == 2). Calling this skips the absorber
// pickup animation but applies the gameplay effect immediately.
void TopRide_KirbyApplyItem(TopRideKirby *kirby, int item_kind); // 0x802d8cb4

ItemKind Gm_GetRandomItem(BoxKind box_kind, ItemGroup group, int spawn_flags); // 0x800eb7e4. box_kind: -1=sky, 0-2=box color. group: -1=all, 0=bad, 1=good. spawn_flags: 0x2=patch, 0x4=box
GOBJ *Item_Create(ItemDesc *desc);                    // 0x8024eef4. Creates item GObj, allocates ItemData, initializes all subsystems
void Item_InitDesc(ItemDesc *, ItemKind kind, float scale, int spawn_type, Vec3 *pos, Vec3 *up, Vec3 *forward, int x40, int x44, int is_airborne, int coll_kind, int x38, int x3c); // 0x802509a0. spawn_type=0 default. up/forward can be NULL. x40/x44/x38/x3c usually -1. is_airborne: -1=skip raycast, other=do raycast. coll_kind: 3=point collision (most items), 1=alloc CollData, 0=requires CollData (dangerous)
ItemCommonAttr *Item_GetCommonAttr(ItemKind kind);    // 0x802500b0. Returns itData[kind].attr
PatchEffectInfo *Item_GetEffectInfo(ItemKind kind);   // 0x80250114. Returns itData[kind].attr->effect_info (NULL for non-patch kinds)
itData *Item_GetItDataPtr(ItemKind kind);             // 0x80250038. Returns itData entry (0x18 bytes per kind)
int Item_CheckIsLoaded();                             // 0x80250098. Returns 1 if per-kind itData is loaded, 0 otherwise (e.g. AR, CT Free Run, possibly stadiums)
ItemGroup Gm_GetItemGroup(ItemKind kind);             // 0x802540f0. Returns attr->effect_info->group (BAD/GOOD/FAKE) for the kind
int CityItem_IsGoodPatch(ItemKind kind);              // 0x802540a8. Returns 1 iff group == ITGROUP_GOOD (returns 0 for NULL, BAD, FAKE)

int CityItem_ProcessFakeItem(GOBJ *item_gobj, void *hurt_params); // 0x802542dc. If CTEVF_FAKEITEMS active, fills hurt_params via Event_FakeItems_FillHurtParams. Returns 1 if active, 0 if not
void CityItem_CopyCommonAttr(GOBJ *item_gobj);        // 0x80251294. Copies ItemCommonAttr fields to ItemData (0x118-0x140), then calls per-kind init
int CityItem_CanCollect(GOBJ *item_gobj);             // 0x80252df0. Returns 1 iff bits 5 and 6 of ItemData.x35a are both clear
void CityItem_ResetQueuedVelocity(ItemData *id);      // 0x80250340. Zeros both accel and vel vectors
void CityItem_EnterExpire(GOBJ *gobj);                // 0x8025611c. Transitions item to expire/flicker state
void CityItem_EnterFall(GOBJ *gobj);                  // 0x802578c8. Transitions item to falling state
void CityItem_BeginPatchToss(GOBJ *gobj, ItemKind kind, int param); // 0x80256254. Ejects a patch from a box on machine touch; selects patch_toss_good vs patch_toss_bad based on CityItem_IsGoodPatch.

u32  CityItem_TestEventFlag(u32 mask);                // 0x80254134. Returns mgr->flags & mask
void CityItem_SetDynabladeEventFlag();                // 0x80254144 - bit 0 (called from event_dynablade_start)
void CityItem_ClearDynabladeEventFlag();              // 0x80254158
void CityItem_SetMeteorEventFlag();                   // 0x80254174 - bit 1 (called from event_meteor_start)
void CityItem_ClearMeteorEventFlag();                 // 0x80254188
void CityItem_InitLocatorEvent(Vec3 *pos, float p0, float p1); // 0x802541a4 - bit 2 + writes loc_pos/params (called from event_rubberyItems_start)
void CityItem_ClearLocatorEvent();                    // 0x8025421c
void CityItem_InitFakeEvent(void *event_data);        // 0x80254238 - bit 3 + sticky fake_event_data (called from event_fakeItems_start)
void CityItem_ClearFakeEvent();                       // 0x80254290 - clears bit 3 only; fake_event_data stays
void CityItem_SetSameItemsEventFlag();                // 0x802542ac - bit 4 (called from event_sameItems_start)
void CityItem_ClearSameItemsEventFlag();              // 0x802542c0

void Box_Break(GOBJ *gobj);                            // 0x802582dc. Breaks box, spawns contents via OutcomeLogic
void Box_EnterSpawn(GOBJ *gobj);                       // 0x80256ec0. Initial box spawn state (falling from sky)
void Box_OnLandCallback(GOBJ *gobj);                   // 0x80257020. Called when box lands on ground

int Patch_GetEffectData(ItemData *id, void *out_entries); // 0x80252e90. Copies id->effect_data->entries (count x 8-byte {int type; float value;}) into out_entries; returns the entry count (0 if not a patch item).
int Patch_GetMaxValue();                               // 0x8000aaf0. Returns max patch stat value from gmGameParams
int Patch_GetMinValue();                               // 0x8000ab1c. Returns min patch stat value from gmGameParams
int Patch_GetPlySavedValue();                          // 0x8000ab48

void *CityEvent_GetFakeItemData(void *event_struct);   // 0x800ee73c. Returns fake item data pointer from the current event entry. Used by event_fakeItems_start to get data for CityItem_InitFakeEvent
void Event_FakeItems_FillHurtParams(void *fake_data, void *hurt_params); // 0x80111a60. Picks a random entry from fake_data, fills 0x34-byte hurt_params struct with damage/knockback values. fake_data: [0]=entries_ptr, [1]=count. Each entry is 0x14 bytes

void CityItemSpawn_Create();                           // 0x800ec4cc. Creates item spawn system GObj
void CityItemSpawn_Init();                             // 0x800ebf70. Initializes spawn parameters
void CityItemSpawn_InitItemFallChances(int stadium_group); // 0x800eb374. Populates grBoxGeneObj spawn tables from item data
void GrBoxGeneratorDetermine(int *box_color, int *box_size); // 0x800ebc04. Picks box color (BoxKind 0-2) and size (0-2) from weighted chance table

AudioEmitter Item_AllocAudioEmitter(int index);
#endif
