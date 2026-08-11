#ifndef KAR_H_YAKUMONO
#define KAR_H_YAKUMONO

// Yakumono - interactive stage props (destructible scenery, hazards, zones,
// boss-like fixed actors).

#include "datatypes.h"
#include "obj.h"
#include "hurt.h"
#include "collision.h"

typedef struct GrObj GrObj;

#define YAKUMONO_GOBJ_KIND 15 // gobj->entity_class for every yakumono GObj
#define GUDATA_YAKUMONO    14 // user-data slot kind holding YakumonoData

// Per-type kind, used as gyp->kind. Absolute values are unconfirmed; the enum
// may have gaps for kind families.
typedef enum YakuKind
{
    YAKUKIND_NONE = -1,

    YAKUKIND_DOWNFORCEZONE,
    YAKUKIND_CATCHZONE,
    YAKUKIND_RECOVERYZONE,
    YAKUKIND_ROTJUMPHILL,
    YAKUKIND_INVISIBLEBALL,
    YAKUKIND_RISINGCUBE,
    YAKUKIND_RISINGCUBECTRL,
    YAKUKIND_GONDOLA,
    YAKUKIND_CANNON,
    YAKUKIND_PUSHOUTWALL,
    YAKUKIND_PUSHOUTWALLCTRL,
    YAKUKIND_LIGHTTUNNEL,
    YAKUKIND_PILLAR,
    YAKUKIND_PILLARCTRL,

    YAKUKIND_BREAKROCK,   // volcano walls, event pillars
    YAKUKIND_BREAKHOUSE,
    YAKUKIND_ANIMFLOOR,
    YAKUKIND_BREAKCORAL,  // "BigStar" / star pole
    YAKUKIND_BREAKICICLE,
    YAKUKIND_LASERGATE,
    YAKUKIND_LASERGATECTRL,
    YAKUKIND_BREAKFLOOR,  // multi-stage cracking
    YAKUKIND_BREAKFAN,
    YAKUKIND_BREAKCOLL,   // shared collision base
    YAKUKIND_BREAKHPCOLLDOOR,
    YAKUKIND_BREAKHPCOLLWALL,
    YAKUKIND_BREAKHPCOLLPILLAR,
    YAKUKIND_BREAKHPCOLLROOF,
    YAKUKIND_BREAKHPCOLLHOUSE,

    YAKUKIND_WHISPYWOODS,

    YAKUKIND_COMMONTERMINATE, // sentinel - per-kind handlers require kind < this

    YAKUKIND_LIGHTHOUSE, // own creator (Lighthouse_Create); outside the sentinel range
} YakuKind;

#define GRYAKU_COMMON_GROUP_MAX      20
#define GRYAKU_COMMON_RANDOM_SET_NUM 10

#define GR_DEFAULT_SCALE 1.0f

// One placed prop of a multi-instance break family.
typedef struct YakuBreakEntry
{
    int x00;
    int x04;
    int node_id;     // 0x08 - index into GrObj.joint_table; the family's debris anchor joint
    int x0c;
} YakuBreakEntry;    // 0x10

// The table of placed props a multi-instance break family owns. Its index space
// matches YakumonoData.region_audio_arr, the family's parallel record array.
typedef struct YakuBreakPlacement
{
    YakuBreakEntry *entries; // 0x00
    int count;               // 0x04 - props this family places
} YakuBreakPlacement;

// Per-instance parameter block. Layout is kind-specific - each arm below is a
// kind whose layout is known; other kinds pass the pointer through untouched.
typedef union YakumonoParam
{
    struct
    {
        int joint_idx;          // 0x00
        int x4;                 // 0x04
        int x8;                 // 0x08
        int xc;                 // 0x0c
        int x10;                // 0x10
        int start_anim_idx;     // 0x14
        int active_anim_idx;    // 0x18
        int end_anim_idx;       // 0x1c
        int inactive_anim_idx;  // 0x20
    } *lighthouse;

    // Break kinds carry an optional drop descriptor whose offset varies by
    // family: rock +0x24, coral +0x28, house +0x30. NULL means no drop.

    // Multi-instance break families (rocks, trees, coral, houses).
    struct
    {
        YakuBreakPlacement *placement; // 0x00
    } *break_family;

    // Common gating fields read by the Create init pipeline. A zero field takes
    // the "no JObj / no model / no audio" branch of its consumer.
    struct
    {
        int x0;            // 0x00 - kind-specific (cannon: read by tail-init)
        void *jobj_data;   // 0x04 - gates JObj alloc + anim attach (anim data is bundled here)
        int x8;            // 0x08
        void *model_data;  // 0x0c - gates model attach
        int x10;           // 0x10
        void *audio_desc;  // 0x14 - gates audio/fgm init; {idData, idDataNum, track_param, ...}
    } *gates;

    struct
    {
        int x0;
        int x4;
    } *other;

    void *raw;
} YakumonoParam;

// User-data block at gobj->user_data[0] (GObj +0x2c).
typedef struct YakumonoData
{
    GOBJ *gobj;             // 0x00 - back-reference
    int desc_id;            // 0x04 - index into stc_yaku_descs[70]
    YakumonoParam *data_ptr;// 0x08 - = grdata->yakumono->data_array[data_idx]
    u8 x0c[0x10];           // 0x0c..0x1b
    Vec3 pos;               // 0x1c - position
    u8 x28[0x18];           // 0x28..0x3f
    u8 x40[0x24];           // 0x40..0x63 - local transform scratch
    void *model_jobj;       // 0x64 - model JObj root
    u8 x68[8];              // 0x68..0x6f
    void *xform_jobj;       // 0x70 - transform JObj whose world matrix (+0x44) is copied into
                            //        the GObj render object. NULL for City Trial break-family
                            //        props (their geometry lives in the stage model by joint
                            //        index), so it is not a usable move handle for them.
    int state;              // 0x74 - state-machine state (-1 initially)
    int prev_anim;          // 0x78
    int prev_joint;         // 0x7c
    int x80;
    void *state_table;      // 0x84 - per-kind state table, array of 16-byte entries indexed
                            //        by state. All-zero for passive kinds (zones).
    Vec3 axis_right;        // 0x88 - init (0,0,1)
    Vec3 axis_up;           // 0x94 - init (0,0,1)
    int xa0;                // 0xa0
    f32 scale;              // 0xa4 - hurtbox scale, = GR_DEFAULT_SCALE
    u8 xa8[4];              // 0xa8
    f32 xac;                // 0xac - accumulated damage
    int xb0;                // 0xb0 - init 5
    int xb4, xb8;           // 0xb4, 0xb8
    u8 xbc[0x24];           // 0xbc..0xdf
    Vec3 bbox_center;       // 0xe0 - bbox / model offset
    HurtData *hurt_data;    // 0xec

    // Per-type callbacks, populated by per-instance tail-init. NULL = no-op.
    void (*proc1)(GOBJ *gobj);                   // 0xf0 - Think (priority 1)
    void (*proc2)(GOBJ *gobj);                   // 0xf4 - priority 4
    void (*proc3)(GOBJ *gobj);                   // 0xf8 - priority 5
    void (*proc4)(GOBJ *gobj);                   // 0xfc - priority 6
    void (*on_damage)(GOBJ *gobj, void *hurt);   // 0x100 - priority 10, damage this frame
    void (*off_damage)(GOBJ *gobj);              // 0x104 - priority 10, damage state ended
    void (*proc5)(GOBJ *gobj);                   // 0x108 - priority 7

    void *effect_group;     // 0x10c - Effect-module group handle (not a collision entry)
    int x110;               // 0x110
    int x114;               // 0x114
    void *fgm_iddata;       // 0x118 - gyp->fgm.idData
    int fgm_iddatanum;      // 0x11c - gyp->fgm.idDataNum
    void *audio_track;      // 0x120
    void *audio_emitter;    // 0x124
    u8 x128[4];             // 0x128
    u8 flags;               // 0x12c - bit 7 (0x80) = "ctrl" variant, and gates the per-frame
                            //         matrix rebuild in GrYakumono_Proc4. Static props leave it
                            //         clear and build their matrix once at spawn.
    u8 x12d[3];             // 0x12d..0x12f

    // Tail region. The layout below is the BREAK families' overlay; other kinds
    // overlay it differently.
    void *region_audio_arr; // 0x130 - per-region audio handle array. For the multi-instance
                            //         break families this is instead the child array of
                            //         count*4 scene-instance records, one per visible prop
                            //         (each record+0x90 back-points to this parent GObj).
    void *region_state_arr; // 0x134 - per-region damage/HP state array; also the child-array
                            //         count for trees/coral (strong/house keep it at +0x140)
    int x138;               // 0x138 - transient: gobj backref / per-region model handle
    int x13c;               // 0x13c - transient: per-region proc handle
    int x140;               // 0x140 - BREAK-hp-coll: per-stage audio handle counter
    int x144;               // 0x144
    int x148;               // 0x148 - BREAK-hp-coll: transient proc handle
    int x14c;               // 0x14c - BREAK-hp-coll: transient model handle
    void *region_src_arr;   // 0x150 - BREAK-coll: per-region audio-source ptr array
    int x154, x158, x15c;
    void *region_src_arr_b; // 0x160 - BREAK-hp-coll: per-region audio-source ptr array
    // sizeof >= 0x164; the true class size is fixed at runtime by HSD_ObjAlloc.
} YakumonoData;

// Kind-tagged spawn entry. Stages using the generic walker dispatch these
// through grYakuFuncTable; City Trial bypasses it via grDataCity1_CreateYakumono.
typedef struct YakumonoEntry
{
    int kind;       // 0x00 - small 0..15 enum, distinct from YakuKind
    void *param;    // 0x04 - kind-specific (often a data_idx or position)
    int x08;        // 0x08 - common-group id; -1 = none
} YakumonoEntry;

// Per-stage manifest at GrData+0x40.
typedef struct YakumonoTable
{
    void **data_array;        // 0x00 - per-instance param-block pointers, indexed by data_idx
    int data_count;           // 0x04
    int x08;                  // 0x08
    int x0c;                  // 0x0c
    YakumonoEntry *entries;   // 0x10
    int entry_count;          // 0x14
} YakumonoTable;

// Create a yakumono GObj. desc_id indexes stc_yaku_descs, data_idx indexes
// grdata->yakumono->data_array. Returns the new GObj so per-instance creators
// can run their tail-init on it.
GOBJ *GrYaku_Create(int desc_id, int data_idx);                                // 0x800f446c

// Cannon creator (desc_id 48). Hardcodes desc_id, so grobj_unused is ignored.
void GrYakuCannon_Create(GrObj *grobj_unused, int data_idx);                   // 0x800fed20
void GrYakuCannon_TailInit(GOBJ *yaku_gobj);                                   // 0x800fed48

void GrYaku_InitData(GOBJ *gobj, int desc_id, void *data_ptr);                 // 0x800f4d50

HurtData *GrYaku_GetHurtData(GOBJ *gobj);                                      // 0x800f8248

// Returns ydata->state, or -1 if gobj is not a yakumono.
int GrYakumono_GetState(GOBJ *gobj);                                           // 0x800f7ab8

// Registered automatically by GrYaku_Create; declared for hooking.
void GrYakumono_Think(GOBJ *gobj);                                             // 0x800f5284 (priority 1)
// Accumulates the frame's damage into ydata+0xac, then fires on_damage (+0x100)
// if set. The City Trial break families leave +0x100 NULL, so this cannot break
// them - only an already-armed BigStar/star pole.
void GrYakumono_Proc10(GOBJ *gobj);                                            // 0x800f5454 (priority 10)
// Adds dmg to ydata+0xac, clamped to <= 9999.
void GrYakumono_AccumulateDamage(GOBJ *gobj, float dmg);                       // 0x800f875c

// The real break path for CT props: resolves the prop's descriptor coll_func and
// calls it with the impacting collider, which computes force = collider radius
// (CollData+0x344) x impactSpeed^2 and compares it to the prop's HP.
//
// impactSpeed is the collider's frame delta (CollData+0x14) projected onto the
// contacted region's outward normal and negated, clamped at 0 - so the delta must
// point into the surface to register at all.
//
// Calling this directly synthesizes a break with all genuine consequences
// (collision retire, mesh hide, debris + drops, SFX, break credit, state change).
// tri_idx is the prop's triangle index within gcp->tri.
int  collideWithObject(GOBJ *yaku_gobj, CollData *other, GrCollParam *gcp,
                       int tri_idx, Vec3 *contact);                            // 0x800f5004
// Threshold break: breaks iff force > HP, leaving HP unchanged.
int  GrYaku_TestImpactBreak(float *hp, CollData *other, GrCollParam *gcp, Vec3 *contact); // 0x80104cd4
// Subtractive: HP -= force, breaks when HP <= 0.
int  GrYaku_ApplyImpactDamage(float *hp, CollData *other, GrCollParam *gcp, Vec3 *contact); // 0x80104be0

// The family coll_funcs collideWithObject dispatches to. Compare a descriptor's
// coll_func against these to identify a prop's family.
// Coral 33 / trees 34 / rocks 35. Threshold break. Spawns debris effects and
// credits the break, but state-changes into a broken-state model rather than
// hiding the original mesh inline.
void hitWeakObject(GOBJ *yaku_gobj, CollData *other, GrCollParam *gcp,
                   int tri_idx, Vec3 *contact);                                // 0x80107914
// Walls 36 / holes 37 / houses 38. Subtractive break, and does the full visible
// break inline at the passed contact point.
void hitStrongObject(GOBJ *yaku_gobj, CollData *other, GrCollParam *gcp,
                     int tri_idx, Vec3 *contact);                              // 0x801086d0

// Toggle every triangle of a placed-instance record between collidable
// (enabled != 0) and broken. The break path calls this with 0 to retire a prop.
void grScene_SetInstanceColl(GrCollRecord *record, int enabled);               // 0x800d7ad0
// Returns 1 iff every triangle of the record has its collidable bit == state.
int  grScene_IsInstanceCollAll(GrCollRecord *record, int state);               // 0x800d7b0c

// Credit one broken yakumono to a player's checklist stat.
void GrYaku_IncrementBreakCount(GOBJ *yaku_gobj, int player_idx);              // 0x80105d80
void Ply_IncrementYakumonoBreakCount(int player_idx, int desc_id);            // 0x8022fed8

void Gr_StateChange(YakumonoData *yd, int state_idx, int anim_idx, int joint_idx,
                    int flags, float start_frame, float anim_rate, float blend_rate);
#define GRSTATECHANGE_NOANIM (1 << 2)

void Gr_AddAnim(YakumonoData *yd, int anim_idx);                               // 0x800f5ce8
void Gr_RemoveAnim(YakumonoData *yd, int anim_idx);                            // 0x800f5f3c

// Runs the per-grkind init hook (28-entry table at 0x804a322c, indexed by the
// physical GroundKind), then always walks grdata->yakumono->entries[].
void grInitYakumono(GrObj *grobj);                                             // 0x800f425c
void grLoadYakumono(void);                                                     // 0x800f440c - loads YkCommon.dat + Yakumono.dat
void Yakumono_Preload(void);                                                   // 0x800f82ec

// Yakumono placement table (category 8 of the stage placement system). Records
// are 0x24 bytes = position plus two orientation/scale vectors. CT breakables
// take their real per-prop transforms from the collision-record pool instead.
int grGetYakumonoposNum(void);                                                 // 0x800d1434
void loadYakumonoLocations(int index, Vec3 *out0, Vec3 *out1, Vec3 *out2);     // 0x800d145c

// Finds the placed-instance record whose jobj matches `key`.
GrCollRecord *grScene_FindInstanceByKey(GrCollParam *gcp, int key);            // 0x800d7954

// Break drop emitters. All call City_SpawnMiscItems with the per-instance drop
// descriptor, using event_source_drop[].chance_destructible (source enum 3).
void GrYakuBreakRock_DropItems(int param);    // 0x8010203c - volcano walls, event pillars
void GrYakuBreakHouse_DropItems(int param);   // 0x80102794 - destructible houses
void GrYakuBreakCoral_DropItems(int param);   // 0x801040fc - "BigStar" / star pole

// Per-instance creators: create one yakumono and run its kind-specific tail-init.
void Lighthouse_Create(GrObj *grobj, int data_idx);  // 0x8010d228 (desc_id 68)
void Lighthouse_Init(GOBJ *yaku_gobj);               // 0x8010d260
void whispyLogic(GrObj *grobj, int data_idx);        // 0x8010db64 (desc_id 69) - WhispyWoods

// A prop's placed instances, its solid collision and its live transform all live
// in the stage collision pool, walked with Gr_GetCollRecords / Gr_GetCollTris:
// one GrCollRecord per visible prop, its yaku_gobj back-pointing at the owning
// yakumono GObj, and its GrCollTri slice being the solid collision itself -
// there is no separate static wall.

typedef struct YakuDesc
{
    void *state_table; // 0x00 - the per-kind state table immediately preceding this block
    void *coll_func;   // 0x04 - break coll_func, e.g. hitWeakObject / hitStrongObject
} YakuDesc;

// Read-only descriptor table, 70 pointers indexed by desc_id. Indices 0..15 are
// paired generic descriptors (8 unique 40-byte blocks); 16..69 are per-instance
// descriptors of varying size, hardcoded by the per-grkind hooks.
static YakuDesc **stc_yaku_descs = (YakuDesc **)0x804a5be8;

// 16-entry Create-wrapper table indexed by YakumonoEntry.kind, organized as 8
// pairs: the even entry is the base kind, the odd its "ctrl" variant (bit 7 of
// ydata->flags). Both wrappers of a pair share a tail-init.
static void (**stc_yaku_create_dispatch)(GrObj *, int, void *) =
    (void (**)(GrObj *, int, void *))0x804a5ba8;

// Unchecked - confirm gobj->entity_class == YAKUMONO_GOBJ_KIND first if the GObj
// did not come off the GAMEPLINK_YAKUMONO list.
static inline YakumonoData *Yaku_GetData(GOBJ *gobj)
{
    return (YakumonoData *)gobj->userdata;
}

static inline void *Yaku_GetDescCollFunc(int desc_id)
{
    YakuDesc *desc = stc_yaku_descs[desc_id];
    return desc ? desc->coll_func : (void *)0;
}

#endif // KAR_H_YAKUMONO
