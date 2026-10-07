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
    ITPRI_POSTENVCOLL, // state callback, then CityItem_UpdatePosition
    ITPRI_TRIGGER, // collect powerup collision, also frees sounds
    ITPRI_8,
    ITPRI_HITCOLL,
    ITPRI_DMGAPPLY,
    ITPRI_ENDOFFRAME = 21, // CityItem_LifetimeThink
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
    ITKIND_COPYNEEDLE,
    ITKIND_COPYMIKE,
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
    [ITKIND_COPYNEEDLE]      = "Needle",
    [ITKIND_COPYMIKE]        = "Mic",
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

// The PatchKind an ItemKind belongs to, counting its up, down and fake variants;
// -1 for anything that is not a stat patch. HP has no down or fake variant.
static inline int Item_KindToPatchKind(ItemKind it_kind)
{
    switch (it_kind)
    {
        case ITKIND_WEIGHT:   case ITKIND_WEIGHTDOWN:   case ITKIND_WEIGHTFAKE:   return PATCHKIND_WEIGHT;
        case ITKIND_ACCEL:    case ITKIND_ACCELDOWN:    case ITKIND_ACCELFAKE:    return PATCHKIND_ACCEL;
        case ITKIND_TOPSPEED: case ITKIND_TOPSPEEDDOWN: case ITKIND_TOPSPEEDFAKE: return PATCHKIND_TOPSPEED;
        case ITKIND_TURN:     case ITKIND_TURNDOWN:     case ITKIND_TURNFAKE:     return PATCHKIND_TURN;
        case ITKIND_CHARGE:   case ITKIND_CHARGEDOWN:   case ITKIND_CHARGEFAKE:   return PATCHKIND_CHARGE;
        case ITKIND_GLIDE:    case ITKIND_GLIDEDOWN:    case ITKIND_GLIDEFAKE:    return PATCHKIND_GLIDE;
        case ITKIND_OFFENSE:  case ITKIND_OFFENSEDOWN:  case ITKIND_OFFENSEFAKE:  return PATCHKIND_OFFENSE;
        case ITKIND_DEFENSE:  case ITKIND_DEFENSEDOWN:  case ITKIND_DEFENSEFAKE:  return PATCHKIND_DEFENSE;
        case ITKIND_HP:       return PATCHKIND_HP;
        default:              return -1;
    }
}

// The kinds that raise a machine's stats: the nine up patches plus All-Up.
static inline int Item_IsStatUpKind(ItemKind it_kind)
{
    switch (it_kind)
    {
        case ITKIND_WEIGHT:
        case ITKIND_ACCEL:
        case ITKIND_TOPSPEED:
        case ITKIND_TURN:
        case ITKIND_CHARGE:
        case ITKIND_GLIDE:
        case ITKIND_OFFENSE:
        case ITKIND_DEFENSE:
        case ITKIND_HP:
        case ITKIND_ALLUP:
            return 1;
        default:
            return 0;
    }
}

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
// dead in retail (written by ItemGObj_CopyCommonAttr to ItemData+0x118 mirror,
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

// Per-kind tail data, copied at runtime into ItemData.unique_attr by the per-kind
// init function (state-table slot +0xC). The buffer is allocated by
// CityItem_AllocUniqueAttr from the pool descriptor at 0x8055dddc with a
// fixed 0x38-byte stride - the box family maximum.
//
// 53 of the 69 ItemKinds share a copy-1-int template that is never read back.
// The 3 box kinds (BOXBLUE/GREEN/RED) populate structured fields; only the box
// layout is documented here.
typedef union ItemUniqueAttr
{
    struct                      // BOXBLUE / BOXGREEN / BOXRED only - sizeof = 0x38
    {
        float x00;              // 0x00
        float x04;              // 0x04
        float rotation_rate;    // 0x08, applied to ItemData.x394 in box rotation update (0x80257a20)
        float x0c;              // 0x0c
        int   hp_min;           // 0x10, ItemData.hp is seeded in [hp_min, hp_max) (zz_80256ce4_)
        int   hp_max;           // 0x14
        float wrap_accumulator; // 0x18, summed into ItemData.x15c at 0x802576ac
        float x1c;              // 0x1c
        float x20;              // 0x20
        float x24;              // 0x24
        float x28;              // 0x28
        float x2c;              // 0x2c
        float x30;              // 0x30
        float effect_scale;     // 0x34, scaled into effect-spawn position at 0x8025710c
    } box;
    int x00;                    // all other 53 kinds - single int copied to unique_attr + 0x00; never read back
} ItemUniqueAttr;

// One animation slot, picked by the anim_index a state names. Every vanilla kind
// has one except kind 20, which has two, and nothing records how many exist.
typedef struct ItemAnimEntry
{
    AnimJointDesc *joint_anim;  // 0x00
    MatAnimJointDesc *mat_anim; // 0x04
    void *script;               // 0x08, state script; becomes ItemData.script_data
    u32 flags;                  // 0x0c, bit 30 loops the AObjs, bit 29 accumulates wrap overflow
} ItemAnimEntry;

typedef struct ItemModelDesc
{
    JOBJDesc *j;                // 0x00, model root
    u32 flag;                   // 0x04, render flag (0x02000000 flat panels; 0x03/0x05/0x0b000000 legendary/skinned pieces)
    int parts[3];               // 0x08, per-group "item parts" counts; Item_InitPartsModel
                                //       (0x80252824, reached from CityItem_Create) asserts each <= 11
                                //       ("item parts model num over!"). Zero for every vanilla kind.
} ItemModelDesc;

typedef struct itData
{
    ItemCommonAttr *attr;       // 0x0
    ItemUniqueAttr *unique_attr; // 0x4, per-kind tail data - only the box family is structured
    ItemModelDesc *model;       // 0x8
    ItemAnimEntry *anim_data;   // 0xc, indexed by the state's anim_index
    struct
    {
        HurtDesc *desc;         // 0x0
        int num;                // 0x4
    } *hurt;                    // 0x10
    TriggerDesc *trigger;       // 0x14
} itData;

// Patch-toss descriptor. Two live inside ItemCommonParam (0x5c good, 0x80 bad),
// picked by CityItem_IsGoodPatch in ItemGObj_BeginPatchToss (0x80256254) when a
// machine or rider touches a patch.
typedef struct PatchTossDesc
{
    float scale_start;     // 0x00, written to ItemData.scale
    float scale_end;       // 0x04, reached after scale_frames
    float up_speed;        // 0x08, times the toucher's up vector -> ItemData.toss_vel
    float drift_scale;     // 0x0c, times a toucher vector, added to pos each frame
    int scale_frames;      // 0x10, length of the scale ramp -> ItemData.toss_scale_frames
    float up_accel;        // 0x14, times the up vector -> ItemData.toss_accel; negated for bad patches
    int hold_frames;       // 0x18 -> ItemData.toss_timer
    int fade_in_frames;    // 0x1c, alpha ramps from 0 over this many frames
    int x20;               // 0x20, frames of the alpha ramp from 1.0 once toss_timer runs out
} PatchTossDesc;

// Global City Trial item parameters loaded from files/ItCommon.dat (root
// symbol "itCommonDataAll", first member). Single instance; pointed to by
// stc_item_param. Fields beyond 0xA4 form a variable-length table indexed by
// PatchKind x history-position used by the patch-toss flight code.
typedef struct ItemCommonParam
{
    float scale;                   // 0x00, global scale multiplier on every spawned item
    float hardmode_scale_mult;     // 0x04, secondary multiplier when difficulty/checklist gate is active
    float shadow_scale_alt;        // 0x08, shadow-size multiplier when ItemData.item_category != 0
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
    float box_spawn_yaw_range;     // 0x40, max yaw rotation (passed to fctiwz -> HSD_Randi -> Vec3_RotateAboutUnitAxis)
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

// The loaded itCommonDataAll wrapper is reached via stc_it_common_data
// (r13+0x7F0 = 0x805dd8d0), set by Gm_LoadItCommon (0x8024feec).
// Item_GetItDataPtr (0x80250038) returns (*stc_it_common_data)->itData + kind*0x18.
// To extend the item table with custom kinds, allocate a larger itData array, copy
// the 69 vanilla entries, append new ones, and overwrite (*stc_it_common_data)->itData
// (the +0x8 member, not stc_item_param, which mirrors the +0x0 `param` member).

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
    GOBJ *parent_gobj;  // 0x00, [computed] always NULL. Maps to ItemData.parent_gobj
    ItemKind kind;      // 0x04, [param] item kind
    int spawn_type;     // 0x08, [param] ItemSpawnType. Maps to ItemData.spawn_type
    Vec3 pos;           // 0x0C, [param] spawn position
    Vec3 forward;       // 0x18, [param] normalized forward vector (can be NULL for defaults)
    Vec3 up;            // 0x24, [param] normalized up vector (can be NULL for defaults)
    float scale;        // 0x30, [param] item scale
    int spawn_serial;   // 0x34, [computed] City_GetItemSpawnNumber(). Maps to ItemData.spawn_serial
    int spawn_area;     // 0x38, [param] spawn slot area; usually -1. Maps to ItemData.spawn_area
    int spawn_coll_kind; // 0x3C, [param] spawn slot kind; usually -1. Maps to ItemData.spawn_coll_kind
    int box_color;      // 0x40, [param] BoxKind, usually -1. Maps to ItemData.box_color
    int box_size;       // 0x44, [param] BoxSize, usually -1. Maps to ItemData.box_size
    int lifetime;       // 0x48, [computed] HSD_Randi(variance) + min from ItemCommonParam
    int coll_kind;      // 0x4C, [param] 3 = point collision (most items), 1 = alloc CollData,
                        //       0 = requires an existing CollData
    int is_airborne;    // 0x50, [param] stack param 1 of Item_InitDesc. -1=skip initial raycast, other=do raycast. Maps to ItemData[0x1D4]
    int x54;            // 0x54, [computed] always 0. Stored as bit flag in ItemData[0x35B]
    int flags;          // 0x58, [computed] by Item_InitDesc from the item kind range. Maps to ItemData[0x48]
} ItemDesc;

// ItemData.spawn_type - what spawned the item. Ply_IncrementItemCollectNum receives it
// as src_tag. A dropped patch picked back up respawns as ITSPAWN_RIDERDROP.
typedef enum ItemSpawnType
{
    ITSPAWN_DIRECT,            // 0, debug and mod spawns, and the Gourmet Race
    ITSPAWN_SKY,               // 1
    ITSPAWN_BOX,               // 2
    ITSPAWN_RIDERDROP,         // 3, Rider_TickDropAllUp, Rider_SpawnDropPatchSeq
    ITSPAWN_TAC,               // 4
    ITSPAWN_DYNABLADE,         // 5
    ITSPAWN_METEOR,            // 6
    ITSPAWN_SECRETCHAMBER = 8, // 8
    ITSPAWN_UFO = 9,           // 9, every stop of the UFO event
} ItemSpawnType;

typedef struct ItemData
{
    GOBJ *item_gobj;            // 0x0, this item's GObj
    GOBJ *parent_gobj;          // 0x4, box GObj that spawned this item (NULL for sky-spawned items)
    GOBJ *child_gobjs[4];       // 0x8, child items spawned when this box breaks (up to 4)
    GOBJ *shadow_gobj;          // 0x18

    ItemKind kind;              // 0x1c
    int spawn_type;             // 0x20, ItemSpawnType, from ItemDesc.spawn_type
    int item_category;          // 0x24, Item_GetCategory(kind): 0=box, non-0=powerup. Determines shadow size, bounce SFX
    JOBJDesc *jobjdesc;         // 0x28
    itData *itData;             // 0x2c
    int spawn_serial;           // 0x30, City_GetItemSpawnNumber() when the desc was built
    int spawn_area;             // 0x34, spawn slot area, usually -1
    int spawn_coll_kind;        // 0x38, spawn slot kind (CityItemSpawn_DetermineBoxPos), usually -1
    int box_color;              // 0x3c, BoxKind; the pool Box_OutcomeLogic rolls from, usually -1
    int box_size;               // 0x40, BoxSize, usually -1
    int lifetime;               // 0x44, decremented each frame in ShadowThink, expire at 0
    int flags;                  // 0x48, from ItemDesc.flags, cleared conditionally per frame

    int state;                  // 0x4c, current state index
    int common_state_num;       // 0x50, states below this index use common_state_table (3)
    int anim_index;             // 0x54, current animation index (-1 = none)
    void *common_state_table;   // 0x58, 20-byte entries shared by every kind
    void *state_table;          // 0x5c, the kind's own entries, from stc_item_state_tbl[kind]
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

    // Runtime copy of ItemCommonAttr fields, written by ItemGObj_CopyCommonAttr.
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
    PatchEffectInfo *effect_data; // 0x140, NULL for non-patch items; consumed by ItemGObj_GetEffectData / Machine_OnTouchItem
    ItemUniqueAttr *unique_attr; // 0x144, allocated by CityItem_AllocUniqueAttr, filled by the kind's init

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
    int toss_ply;               // 0x1a0, ply of the patch toss's toucher

    CollData *coll_data;        // 0x1a4, map collision data (NULL = uses point collision only)
    struct                      // 0x1a8, items use point collision for ground detection
    {
        int raycast_idx;        // 0x1a8, collision ID from raycast result
        Vec3 land_pos;          // 0x1ac, ground position calculated by raycast
    } point_coll;               //
    int x1b8;                   // 0x1b8
    int x1bc;                   // 0x1bc
    int x1c0;                   // 0x1c0
    float gravity_strength;     // 0x1c4, Gr_GetDownVector's return, refreshed by CityItem_UpdateGravity
    Vec3 fall_dir;              // 0x1c8, gravity/down direction vector, used for ground raycasting
    // 0x1d4, seeded from ItemDesc, then owned by the collision code:
    //   1  = airborne (falling, tossed, bouncing) - Item_SetAirborne
    //   0  = resting on a surface - Item_ClearAirborne, written at every land transition
    //   -1 = airborne with the ground raycast suppressed; the envcoll callbacks return early
    // The only reliable "is this item on the ground" test (ITEM_X35A_GROUNDED is not).
    int is_airborne;            // 0x1d4
    int x1d8;                   // 0x1d8, collision temp data (cleared as 3-word block)
    int x1dc;                   // 0x1dc
    int x1e0;                   // 0x1e0
    // Patch toss, set up by ItemGObj_BeginPatchToss
    PatchTossDesc *toss_desc;   // 0x1e4, ItemCommonParam.patch_toss_good or _bad
    GOBJ *toss_toucher;         // 0x1e8, machine or rider GObj that touched the patch
    s8 toss_phase;              // 0x1ec, 0..2, advanced by the per-frame toss update
    u8 x1ed[3];                 // 0x1ed
    Vec3 toss_vel;              // 0x1f0, added to pos each frame
    Vec3 toss_accel;            // 0x1fc, added to toss_vel each frame
    int toss_scale_frames;      // 0x208, frames left in the scale ramp
    int x20c;                   // 0x20c
    float toss_scale_step;      // 0x210, added to scale while toss_scale_frames > 0
    int toss_timer;             // 0x214, phase 1 countdown, from PatchTossDesc.hold_frames
    int x218;                   // 0x218
    int dmg;                    // 0x21c, damage taken, accumulated from HurtData.dmg_taken and capped at 9999 (setBoxDamage)
    int x220;                   // 0x220
    int damage_processed;       // 0x224, "damage taken this frame" latch - cleared at end of CityItem_ApplyDamageFromHurtData (proc priority 10)
    float anim_start_frame;     // 0x228, frame the next CityItem_StateChange starts its animation on;
                                //        cleared by CityItem_InitData, and the box family counts damaging
                                //        hits into it so it doubles as the crack stage its texture shows
    int efgroup;                // 0x22c, EfGroup this item's effects spawn into
    int efgroup2;               // 0x230, second EfGroup
    int x234;                   // 0x234
    s64 effect_id;              // 0x238, from Effect_SpawnSync, or ItemGObj_BoxSpawnImpactEffect sign-extended

    int audio_source;           // 0x240, audio emitter, -1 = not allocated. Set by Item_AllocAudioEmitter
    int audio_track;            // 0x244, audio track ID. Set by AudioTrack_Alloc
    int audio_timer;            // 0x248, per-frame countdown paired with ITEM_X35A_PICKUP_BUSY; at 0 the
                                //        pickup-lock bit is cleared and the audio freed
    int bounce_num;             // 0x24c, incremented when bouncing @ 80255a70

    TriggerData trigger;        // 0x250, item pickup/touch collision

    // Set from the current state entry (5 words per entry: anim_index, 4 callbacks)
    void (*anim_callback)(GOBJ *);         // 0x328, ITPRI_ANIM
    void (*phys_callback)(GOBJ *);         // 0x32c, ITPRI_PHYS
    void (*envcoll_callback)(GOBJ *);      // 0x330, ITPRI_ENVCOLL
    void (*post_envcoll_callback)(GOBJ *); // 0x334, ITPRI_POSTENVCOLL
    void (*trigger_callback)(GOBJ *);      // 0x338, ITPRI_TRIGGER; cleared on each state change
    // ITPRI_DMGAPPLY, when the HurtData took a hit; dmg = &hurt_data->hitcoll_log_idx
    void (*on_damage_callback)(GOBJ *, void *dmg); // 0x33c
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
    u8 flags_x35a;              // 0x35a, see ITEM_X35A_*
    // x35b bits:
    //   bit 5 (0x20) = model_hidden (mirrors JOBJ_HIDDEN on the rendered jobj)
    //   bit 6 (0x40) = ItemDesc.x54
    //   bit 7 (0x80) = set by CityItem_SetX35bBit7; ItemGObj_BeginPatchToss skips the toss when set
    u8 x35b;                    // 0x35b

    int forced_item;            // 0x35c, predetermined ItemKind for box contents. -1 = random, -2 = no items
    int break_timer;            // 0x360, set to 8 on ItemGObj_BoxBreak
    int hp;                     // 0x364, boxes: break threshold, breaks once dmg >= hp (Box_OnTakeDamage)
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

    float x394;                 // 0x394, box family; scaled by ItemUniqueAttr.box.rotation_rate
    u8 x398;                    // 0x398
    u8 x399[3];                 // 0x399
    u8 x39c;                    // 0x39c
    u8 x39d[3];                 // 0x39d
} ItemData;

// ItemData.flags_x35a bits.
#define ITEM_X35A_SKY_SPAWNED  0x01  // set by ItemGObj_SetSkySpawned, read by the power-up handlers
// Set at spawn for coll_kind 3 items and when the envcoll raycast first finds ground,
// and never cleared. A sky drop has it hundreds of units up, so this is NOT a
// "resting on the ground" test - use ItemData.is_airborne for that.
#define ITEM_X35A_GROUNDED     0x10
#define ITEM_X35A_PICKUP_LOCK  0x20  // persistent: "this is a box, never collectible" (set by box init)
#define ITEM_X35A_PICKUP_BUSY  0x40  // temporary: spawn-anim or audio busy (paired with audio_timer)
#define ITEM_X35A_DEBUG_DRAW   0x80  // cleared on every state change; gates the trigger/coll overlay in Item_GX

// City Trial event-mode flag bits (CityItemMgr.flags). Each bit has paired
// set/clear functions at 0x80254144..0x802542C0; setters are called from the
// matching event_*_start handler and clearers from the corresponding _end.
typedef enum CityEventSpawnFlag
{
    CTEVF_DYNABLADE     = 1 << 0, // event_dynablade_start  -> SetDynabladeEventFlag
    CTEVF_METEOR        = 1 << 1, // event_meteor_start     -> SetMeteorEventFlag
    CTEVF_LOCATOR       = 1 << 2, // event_rubberyItems_start -> InitLocatorEvent (also sets loc_pos/params)
    CTEVF_FAKEITEMS     = 1 << 3, // event_fakeItems_start  -> InitFakeEvent
    CTEVF_SAMEITEMS     = 1 << 4, // event_sameItems_start  -> SetSameItemsEventFlag
} CityEventSpawnFlag;

// City Trial item manager (0x1c8 bytes), allocated and zeroed by Item_InitObj on
// every 3D scene load and reached via stc_city_item_mgr. Holds CT-wide item
// bookkeeping: live count, lifetime spawn counter, per-event flag word, and
// locator/fake-event payloads.
typedef struct CityItemMgr
{
    s32 live_item_count;        // 0x000  ++ in CityItem_Create, -- on destruction. Create proceeds while <= 100, so up to 101 live
    s32 lifetime_spawn_count;   // 0x004  ++ only, read by City_GetItemSpawnNumber
    u8 _scaffold[0x1A4];        // 0x008  5 x 84-byte structures, each a doubly-linked chain of four
                                //         16-byte chunks plus a self-ptr at +0x40 and 16 reserved
                                //         bytes. Initialized by Item_InitObj but never read elsewhere
                                //         in retail (vestigial / scaffolded subsystem).
    u32 flags;                  // 0x1ac  CityEventSpawnFlag bitmask of currently-active CT events
    Vec3 loc_pos;               // 0x1b0  locator-event spawn position; valid iff flags & CTEVF_LOCATOR
    f32 loc_param0;             // 0x1bc  locator-event aux (valid iff flags & CTEVF_LOCATOR)
    f32 loc_param1;             // 0x1c0  locator-event aux (valid iff flags & CTEVF_LOCATOR)
    void *fake_event_data;      // 0x1c4  set by CityItem_InitFakeEvent; stays set for the scene (ClearFakeEvent leaves it)
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
    TRITEM_MISSILE,          // 10 a2dIT07 AC_BoostUp_Missile (fired missile)
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
    u32 enabled_mask;           // 0x24, bitmask of which items can spawn (bits 0-21)
    float x28;                  // 0x28
} TopRideItemMgr;

// Top Ride ItemMgr singleton pointer (r13 + 0xAC4). Set during Top Ride 3D scene init.
static TopRideItemMgr **stc_topride_itemmgr = (TopRideItemMgr **)(0x805dd0e0 + 0xAC4);

// Spawns a Top Ride item GObj at a given position and links it into the
// ItemMgr's active list; it then behaves like any other spawned TR item and is
// collected on kirby collision. Vanilla callers pass flag1=0, flag2=1.
void TopRideItem_SpawnAtPosition(TopRideItemMgr *mgr, int item_kind, Vec3 *pos, Vec3 *orient, uint flag1, uint flag2); // 0x8034bf50

// Per-item data blob for a Top Ride item kind.
typedef struct TopRideItemData
{
    u8 x00[0x10];       // 0x00
    float spawn_weight; // 0x10, read by the weighted pickers in TopRideItem_SpawnTimed and TopRideItem_PartyBallUpdate
} TopRideItemData;

// Out-of-range kinds fall through to `return kind` (an invalid pointer), so only call
// with 0..TRITEM_NUM-1.
const TopRideItemData *TopRideItem_GetDataByIndex(int kind); // 0x8034d204

typedef struct TopRideKirby TopRideKirby;

// Per-kind effect dispatcher: applies a TopRide item's effect to a Kirby
// directly. This is the same dispatcher invoked by the per-frame consume path
// (TopRide_KirbyUpdate -> Absorber consume). Out-of-range kinds (< 0 or >= 22)
// silently no-op. Requires kirby+0x7c (held item GObj) to be non-null - true
// during active gameplay (round_state == 2). Calling this skips the absorber
// pickup animation but applies the gameplay effect immediately.
void TopRide_KirbyApplyItem(TopRideKirby *kirby, int item_kind); // 0x802d8cb4

// box_kind: -1 sky, 0-2 box color. group: -1 all, 0 bad, 1 good.
// spawn_flags: 0x2 patch, 0x4 box.
ItemKind CityItemSpawn_GetRandomItemID(BoxKind box_kind, ItemGroup group, int spawn_flags); // 0x800eb7e4
// Creates a City Trial item GObj, allocates ItemData and initializes every
// subsystem. desc->kind must be in [0, ITKIND_NUM) or it asserts; the upper
// bound is the `cmpwi r4,69` at 0x8024efb4, so custom kinds need that immediate
// patched higher. Top Ride uses TopRideItem_Create (0x8034ad08) instead.
GOBJ *CityItem_Create(ItemDesc *desc);                // 0x8024eef4
// Fills the item's ItemData from desc: kind, threshold category, itData, state table.
void CityItem_InitData(GOBJ *item_gobj, ItemDesc *desc); // 0x8024eaf4
// spawn_type defaults to 0, up/forward may be NULL, and box_color/box_size/spawn_area/
// spawn_coll_kind are usually -1. is_airborne -1 skips the ground raycast. coll_kind:
// 3 = point collision (most items), 1 = alloc CollData, 0 = requires one (dangerous).
void Item_InitDesc(ItemDesc *desc, ItemKind kind, float scale, int spawn_type, Vec3 *pos, Vec3 *up, Vec3 *forward, int box_color, int box_size, int is_airborne, int coll_kind, int spawn_area, int spawn_coll_kind); // 0x802509a0
ItemCommonAttr *Item_GetCommonAttr(ItemKind kind);    // 0x802500b0. Returns itData[kind].attr
PatchEffectInfo *Item_GetEffectInfo(ItemKind kind);   // 0x80250114. Returns itData[kind].attr->effect_info (NULL for non-patch kinds)
itData *Item_GetItDataPtr(ItemKind kind);             // 0x80250038. Returns itData entry (0x18 bytes per kind)
int Item_CheckIsLoaded();                             // 0x80250098. Returns 1 if per-kind itData is loaded, 0 otherwise (e.g. AR, CT Free Run, possibly stadiums)
ItemGroup Item_GetGroup(ItemKind kind);               // 0x802540f0. Returns attr->effect_info->group (BAD/GOOD/FAKE) for the kind
int CityItem_IsGoodPatch(ItemKind kind);              // 0x802540a8. Returns 1 iff group == ITGROUP_GOOD (returns 0 for NULL, BAD, FAKE)

// Fills hurt_params from CityItemMgr.fake_event_data and returns 1 while it is set.
// The pointer outlives the event; item_gobj is unused.
int ItemGObj_ProcessFakeItem(GOBJ *item_gobj, void *hurt_params); // 0x802542dc
void ItemGObj_CopyCommonAttr(GOBJ *item_gobj);        // 0x80251294. Copies ItemCommonAttr fields to ItemData (0x118-0x140), then calls per-kind init
// JObj_AddAnimAll of ItemData.current_anim's joint/mat animation onto the model, starting
// at frame; rate goes to ItemData.anim_speed.
void ItemGObj_BindStateAnim(GOBJ *item_gobj, float frame, float rate); // 0x80251894

// Two distinct per-kind lookups:
//
// 1. Threshold category (0..24, stored to ItemData.item_category). CityItem_InitData
//    (0x8024eaf4) and Item_GetCategory scan stc_item_threshold
//    for the first entry >= the kind and store that entry's index. The scan is a
//    linear value search, not an index, so an out-of-range kind falls off the
//    end and yields -1 rather than reading past the table. Callers of the helper
//    only test this for nonzero (a "real placeable kind" guard).
//
// 2. Per-kind state descriptors, stc_item_state_tbl, indexed by the raw ItemKind
//    at ItemData+0x1c. CityItem_InitData (0x8024ec74) and CityItem_Create
//    (0x8024f12c, 0x8024f330) read tbl[kind] for the state fn pointers. A kind
//    >= ITKIND_NUM indexes past the table (the bound at CityItem_Create
//    0x8024efb4 stops those first). To give a synthetic kind valid state
//    behavior, rewrite the instance's ItemData+0x1c to an in-range base kind
//    after InitData writes it.
// Ascending, 25 entries: {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,26,27,38,
// 50,54,57,60,68}.
static s32 *stc_item_threshold = (s32 *)0x804b5f18;
#define ITEM_THRESHOLD_NUM 25
// ITKIND_NUM pointers to 0x58-byte descriptors, followed by the "ItCommon.dat"
// string. Descriptor +0x0 is the kind's state table, +0x8 its init, +0x18 its post-init.
static void **stc_item_state_tbl = (void **)0x804b6088;
// Threshold category for a kind, -1 out of range; callers only test for nonzero.
int Item_GetCategory(ItemKind kind);                  // 0x8024ea54
int ItemGObj_CanCollect(GOBJ *item_gobj);             // 0x80252df0. Returns 1 iff ITEM_X35A_PICKUP_LOCK and ITEM_X35A_PICKUP_BUSY are both clear
void CityItem_ResetQueuedVelocity(ItemData *id);      // 0x80250340. Zeros both accel and vel vectors
void Item_ClearAirborne(ItemData *id);                // 0x80254ccc. is_airborne = 0 (landed)
void Item_SetAirborne(ItemData *id);                  // 0x80254cd8. is_airborne = 1 (falling/tossed)
void Item_SetGroundedFlag(ItemData *id);              // 0x802557a8. Sets ITEM_X35A_GROUNDED
void ItemGObj_SetSkySpawned(GOBJ *item_gobj);        // 0x80254040. Sets ITEM_X35A_SKY_SPAWNED
void ItemGObj_EnterExpire(GOBJ *item_gobj);           // 0x8025611c. Transitions item to expire/flicker state
void ItemGObj_EnterFall(GOBJ *item_gobj);             // 0x802578c8. Transitions item to falling state
// Tosses a patch off its toucher, picking the good or bad PatchTossDesc from
// CityItem_IsGoodPatch. is_machine says whether toucher is a machine or a rider GObj.
void ItemGObj_BeginPatchToss(GOBJ *item_gobj, GOBJ *toucher, int is_machine); // 0x80256254

u32  CityItem_TestEventFlag(u32 mask);                // 0x80254134. Returns mgr->flags & mask
void CityItem_SetDynabladeEventFlag();                // 0x80254144 - bit 0 (called from event_dynablade_start)
void CityItem_ClearDynabladeEventFlag();              // 0x80254158
void CityItem_SetMeteorEventFlag();                   // 0x80254174 - bit 1 (called from event_meteor_start)
void CityItem_ClearMeteorEventFlag();                 // 0x80254188
void CityItem_InitLocatorEvent(float x, float y, float z, float p0, float p1); // 0x802541a4 - bit 2 + writes loc_pos/params (called from event_rubberyItems_start)
void CityItem_ClearLocatorEvent();                    // 0x8025421c
void CityItem_InitFakeEvent(void *event_data);        // 0x80254238 - bit 3 + sticky fake_event_data (called from event_fakeItems_start)
void CityItem_ClearFakeEvent();                       // 0x80254290 - clears bit 3 only; fake_event_data stays
void CityItem_SetSameItemsEventFlag();                // 0x802542ac - bit 4 (called from event_sameItems_start)
void CityItem_ClearSameItemsEventFlag();              // 0x802542c0

void ItemGObj_BoxBreak(GOBJ *item_gobj);               // 0x802582dc. Breaks box, spawns contents via Box_OutcomeLogic
void ItemGObj_BoxEnterSpawn(GOBJ *item_gobj);          // 0x80256ec0. Initial box spawn state (falling from sky)
void ItemGObj_BoxOnLand(GOBJ *item_gobj);              // 0x80257020. Called when box lands on ground

// Copies the patch's 8-byte {int type; float value} entries into out_entries and
// returns the count, 0 if this is not a patch item.
int ItemGObj_GetEffectData(GOBJ *item_gobj, void *out_entries); // 0x80252e90
int Patch_GetMaxValue();                               // 0x8000aaf0. Returns max patch stat value from gmGameParams
int Patch_GetMinValue();                               // 0x8000ab1c. Returns min patch stat value from gmGameParams

// Picks a random entry from fake_data ({entries_ptr, count}, 0x14 bytes each) and
// fills the 0x34-byte hurt_params with its damage/knockback values.
void Event_FakeItems_FillHurtParams(void *fake_data, void *hurt_params); // 0x80111a60

void CityItemSpawn_Create();                           // 0x800ec4cc. Creates item spawn system GObj
void CityItemSpawn_Init();                             // 0x800ebf70. Initializes spawn parameters
void CityItemSpawn_InitItemFallChances(int stadium_group); // 0x800eb374. Populates grBoxGeneObj spawn tables from item data
// The stadium counterparts of InitItemFallChances, for a stage whose GrItemNode
// carries a GrItemPool instead of an item_desc. Each clears grBoxGeneObj and
// files every pool entry into item_group_spawn[ItemCommonAttr.box_kind], taking
// the round's chance column; entries whose box_kind is outside 0..2 are dropped.
// A and B differ only in reading GrItemNode.pool_a vs pool_b.
void CityItemSpawn_InitStadiumPoolsA(); // 0x800ed8b0
void CityItemSpawn_InitStadiumPoolsB(); // 0x800eda0c
int GrBoxGeneratorDetermine(int *box_color, int *box_size);  // 0x800ebc04. Picks box color (BoxKind 0-2) and size (0-2) from the weighted chance table, and returns the box's ItemKind (ITKIND_BOXBLUE + color)

// The per-tick spawn preamble CityItemSpawn_Think runs before it picks a kind.
// DetermineBoxPos rolls an unoccupied spawn slot: coll_kind 0 routes the position
// through grLoadItemPosition, 1 through CityItemSpawn_DeterminePos, 2 means the
// field is full and nothing spawns. spawn_style selects the descriptor's
// is_airborne / coll_kind pair inside PowerUp_SpawnFromSky.
void CityItemSpawn_DetermineBoxPos(int picked, int *out_coll_kind, int *out_area, int *out_spawn_style); // 0x800eab7c
void CityItemSpawn_DeterminePos(int area, Vec3 *out_pos, Vec3 *out_forward, Vec3 *out_scratch, int *out_exist_index); // 0x800ec800
void grLoadItemPosition(int area, Vec3 *out_pos, Vec3 *out_forward, Vec3 *out_scratch); // 0x800d10dc
// Adds delta to grBoxGeneInfo.cur_num_items, and to total_spawn_count when positive.
void CityItemSpawn_IncrementNum(int delta);            // 0x800ec57c
// Bumps the per-area occupancy byte at grBoxGeneInfo + 0x4c + area, ground slots only.
void CityItemSpawn_IncrementAreaCount(int area, int coll_kind); // 0x800ec600
// Rolls the 2nd..4th item of a multi-item box from the subsequent-patch pool.
ItemKind CityBox_DecideSubsequentPatch(ItemKind first_kind, int group); // 0x800eba70
// Drops one item into the city from a spawn slot. area, coll_kind, box_color and
// box_size land in ItemData.spawn_area, spawn_coll_kind, box_color and box_size.
GOBJ *PowerUp_SpawnFromSky(ItemKind kind, int box_color, int box_size, Vec3 *pos, Vec3 *forward, int area, int coll_kind, int spawn_style); // 0x800ecdf4

// Rolls a broken box's contents into ItemData.child_gobjs[]. forced_item (+0x35c)
// wins when set; otherwise the kind comes from the box color's pool and the count
// (1 / 2 / 4) from box_size, but only for vanilla patch kinds 3..0x12.
void Box_OutcomeLogic(ItemData *id);                   // 0x80250ae8
// Box_OutcomeLogic's per-item yaw offset in degrees: 0, 180, 90, -90. A 0 skips the rotation.
static const float *const stc_box_slot_yaw = (const float *)0x80489f48;
// The box's hit / break particle burst: picks one of the six yakumono-bank effects
// 50000..50005 from ItemData.kind and is_break, then Effect_SpawnSync's it onto joint
// 1 in anchor mode 205. Returns the effect id, which the caller sign-extends into
// ItemData.effect_id; -1 selects nothing and returns 0.
int ItemGObj_BoxSpawnImpactEffect(GOBJ *item_gobj, int is_break); // 0x80251f64
// Spawns one item out of a breaking box: horiz is the scatter distance, angle the
// launch pitch in radians (clamped to pi/2), dir the yaw-rotated forward vector,
// and multi selects the vertical-spread variant.
GOBJ *Box_SpawnContents(ItemKind kind, int spawn_type, Vec3 *pos, Vec3 *dir, int multi, float horiz, float angle); // 0x80253378
// 1 if n more items fit under the field's simultaneous-item cap.
int CityItem_CanSpawnNMore(int n);                     // 0x80252d40

AudioEmitter Item_AllocAudioEmitter(int index); // 0x8005de3c
#endif
