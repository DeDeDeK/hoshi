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

// Gr_YakuKind: YakumonoData.kind and the index into stc_yaku_descs. Kinds below
// YAKUKIND_COMMONTERMINATE are the common kinds a stage's YakumonoEntry list places;
// the rest come from per-instance creators. Kinds 21..40 are the break kinds
// PlayerStats.yakumono_break counts. Only confirmed kinds are named.
typedef enum YakuKind
{
    YAKUKIND_NONE             = -1,
    YAKUKIND_COMMONTERMINATE  = 16,
    YAKUKIND_DOWNFORCEZONE    = 16,
    YAKUKIND_CATCHZONE        = 17,
    YAKUKIND_RECOVERYZONE     = 20,
    YAKUKIND_BREAKHOUSE       = 22, // coll_func GrYakuBreakHouse_DropItems, shared with kind 23
    YAKUKIND_BREAKCORAL       = 24, // Sky Sands coral; coll_func hitBigStar
    YAKUKIND_STARPOLE         = 29, // a BreakCoral kind; coll_func hitBigStar
    YAKUKIND_BREAKFAN         = 30, // Machine Passage fans
    YAKUKIND_BREAKICICLE      = 31, // Frozen Hillside icicles
    YAKUKIND_BREAKFLOOR       = 32, // forest pitfall, ice platforms; coll_func hitBreakableFloor
    YAKUKIND_CORAL            = 33, // City Trial coral; coll_func hitWeakObject
    YAKUKIND_TREE             = 34, // coll_func hitWeakObject
    YAKUKIND_ROCK             = 35, // coll_func hitWeakObject
    YAKUKIND_BREAKHPCOLLDOOR  = 36, // volcano rock walls; coll_func hitStrongObject
    YAKUKIND_BREAKHPCOLLHOLE  = 37, // volcano-base holes; coll_func hitStrongObject
    YAKUKIND_BREAKHPCOLLHOUSE = 38, // houses; coll_func hitStrongObject
    YAKUKIND_EVENTPILLAR      = 40, // the Pillar event's huge pillars, a BreakRock kind
    YAKUKIND_ROTJUMPHILL      = 41,
    YAKUKIND_ROTJUMPHILLCTRL  = 42,
    YAKUKIND_INVISIBLEBALL    = 43,
    YAKUKIND_RISINGCUBE       = 44,
    YAKUKIND_RISINGCUBECTRL   = 45,
    YAKUKIND_GONDOLA          = 46,
    YAKUKIND_GONDOLACTRL      = 47,
    YAKUKIND_CANNON           = 48,
    YAKUKIND_PUSHOUTWALL      = 49,
    YAKUKIND_PUSHOUTWALLCTRL  = 50,
    YAKUKIND_LIGHTTUNNEL      = 51,
    YAKUKIND_PILLAR           = 52,
    YAKUKIND_PILLARCTRL       = 53,
    YAKUKIND_ANIMFLOOR        = 54,
    YAKUKIND_LASERGATE        = 57,
    YAKUKIND_LASERGATECTRL    = 58,
    YAKUKIND_RAILFIRE         = 65, // the Rail Fire stations; nothing else creates one
    YAKUKIND_SECRETCHAMBER    = 66, // created only by the Secret Chamber event
    YAKUKIND_UFO              = 67,
    YAKUKIND_LIGHTHOUSE       = 68,
    YAKUKIND_WHISPYWOODS      = 69,
    YAKUKIND_NUM              = 70,
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

// The table of placed props a multi-instance break family owns (hitWeakObject's
// param). Its index space matches YakumonoData.region_audio_arr, the family's
// parallel record array.
typedef struct YakuBreakPlacement
{
    YakuBreakEntry *entries; // 0x00
    int target_num;          // 0x04 - props this family places
    float *hp;               // 0x08 - GrYaku_TestImpactBreak's param; hp[0] is the break threshold
} YakuBreakPlacement;

// Per-instance parameter block. Layout is kind-specific - each arm below is a
// kind whose layout is known; other kinds pass the pointer through untouched.
typedef union YakumonoParam
{
    struct
    {
        int joint_idx;          // 0x00
        int *light_joints;      // 0x04, one lamp joint per light, the beam leaves along -Y
        int light_num;          // 0x08, 2 on GrCity1
        int *zones;             // 0x0c
        int zone_num;           // 0x10
        int start_anim_idx;     // 0x14
        int active_anim_idx;    // 0x18
        int end_anim_idx;       // 0x1c
        int inactive_anim_idx;  // 0x20
        float beam_len;         // 0x24
        float beam_slope;       // 0x28, beam radius per unit of t along the beam
        float beam_base;        // 0x2c, beam radius at the lamp
        float *heal_max;        // 0x30
    } *lighthouse;

    // Break kinds carry an optional drop descriptor whose offset varies by
    // family: rock +0x24, coral +0x28, house +0x30. NULL means no drop.

    // Multi-instance break families (rocks, trees, coral, houses).
    YakuBreakPlacement *break_family;

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
    YakuKind kind;          // 0x04 - index into stc_yaku_descs
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
    int common_state_num;   // 0x78 - states below it index common_state_table, the rest state_table
    int anim_idx;           // 0x7c - current anim_idx from Gr_StateChange, -1 initially
    void *common_state_table; // 0x80 - 16-byte entries indexed by state
    void *state_table;      // 0x84 - per-kind state table, 16-byte entries indexed by
                            //        state - common_state_num. All-zero for passive kinds (zones).
    Vec3 forward;           // 0x88 - init (0,0,1)
    Vec3 up;                // 0x94 - init (0,1,0)
    int xa0;                // 0xa0
    f32 scale;              // 0xa4 - hurtbox scale, = GR_DEFAULT_SCALE
    u8 xa8[4];              // 0xa8
    f32 dmg;                // 0xac - accumulated damage, capped at 9999
    int xb0;                // 0xb0 - init 5
    int xb4, xb8;           // 0xb4, 0xb8
    f32 xbc;                // 0xbc
    f32 state_frame;        // 0xc0 - anim_frame + anim_overflow
    u8 xc4[0x1c];           // 0xc4..0xdf
    f32 anim_frame;         // 0xe0 - advanced by anim_rate each frame
    f32 anim_overflow;      // 0xe4
    f32 anim_rate;          // 0xe8
    HurtData *hurt_data;    // 0xec

    // Per-type callbacks, populated by per-instance tail-init and by Gr_StateChange,
    // which also clears 0x100..0x108. NULL = no-op.
    void (*anim_callback)(GOBJ *gobj);           // 0xf0 - priority 1
    void (*phys_callback)(GOBJ *gobj);           // 0xf4 - priority 4
    void (*envcoll_callback)(GOBJ *gobj);        // 0xf8 - priority 5
    void (*post_envcoll_callback)(GOBJ *gobj);   // 0xfc - priority 6
    // Priority 10, when the HurtData took a hit; dmg = &hurt_data->hitcoll_log_idx.
    void (*on_damage_callback)(GOBJ *gobj, void *dmg); // 0x100
    void (*off_damage_callback)(GOBJ *gobj);     // 0x104 - priority 10, damage state ended
    void (*trigger_callback)(GOBJ *gobj);        // 0x108 - priority 7

    int efgroup;            // 0x10c - EfGroup from GrYaku_AllocEffectGroup
    int x110;               // 0x110
    int x114;               // 0x114
    void *fgm_iddata;       // 0x118 - gyp->fgm.idData
    int fgm_iddatanum;      // 0x11c - gyp->fgm.idDataNum
    void *audio_track;      // 0x120
    void *audio_emitter;    // 0x124
    u8 x128[4];             // 0x128
    u8 flags;               // 0x12c - bit 7 (0x80) = "ctrl" variant, and gates the per-frame
                            //         matrix rebuild in YakumonoGObj_Proc4. Static props leave it
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
    u8 x164[0x190 - 0x164]; // 0x164..0x18f
} YakumonoData;

// Kind-tagged spawn entry ("common data"). grInitYakumono creates each one through
// stc_yaku_common_create; City Trial ships none and uses grDataCity1_CreateYakumono.
typedef struct YakumonoEntry
{
    YakuKind kind;  // 0x00 - below YAKUKIND_COMMONTERMINATE; indexes stc_yaku_common_create
    void *param;    // 0x04 - kind-specific (often a data_idx or position)
    int x08;        // 0x08 - common-group id; -1 = none
} YakumonoEntry;

// Per-stage manifest at GrData+0x40.
typedef struct YakumonoTable
{
    void **data_array;        // 0x00 - per-instance param-block pointers, indexed by data_idx
    int data_count;           // 0x04
    void **spawn_data_array;  // 0x08 - runtime-spawned param blocks; grColl_Alloc reserves collision for each once
    int spawn_data_count;     // 0x0c - City Trial fills both from GrCity1Event.dat (fn_grSetupCityEventData)
    YakumonoEntry *entries;   // 0x10
    int entry_count;          // 0x14
} YakumonoTable;

// Create a yakumono GObj. data_idx indexes grdata->yakumono->data_array. Returns the
// new GObj so per-instance creators can run their tail-init on it.
GOBJ *GrYaku_Create(YakuKind kind, int data_idx);                              // 0x800f446c

// GrYaku_Create over spawn_data_array. Collision attaches are never returned to the
// pool, so spawning an entry more often than the table lists it asserts in grcoll.c.
GOBJ *GrYaku_CreateSpawn(YakuKind kind, int spawn_idx);                        // 0x800f48cc

// Cannon creator (YAKUKIND_CANNON). Hardcodes the kind, so grobj_unused is ignored.
void GrYakuCannon_Create(GrObj *grobj_unused, int data_idx);                   // 0x800fed20
void GrYakuCannon_TailInit(GOBJ *yaku_gobj);                                   // 0x800fed48

void YakumonoGObj_InitData(GOBJ *yaku_gobj, YakuKind kind, void *data_ptr);   // 0x800f4d50

HurtData *YakumonoGObj_GetHurtData(GOBJ *yaku_gobj);                           // 0x800f8248

// Asserts gobj is a yakumono.
YakuKind YakumonoGObj_GetKind(GOBJ *yaku_gobj);                                // 0x800f7a64
// Returns ydata->state, or -1 if gobj is not a yakumono.
int YakumonoGObj_GetState(GOBJ *yaku_gobj);                                    // 0x800f7ab8

// Registered automatically by GrYaku_Create; declared for hooking.
void YakumonoGObj_Think(GOBJ *yaku_gobj);                                      // 0x800f5284 (priority 1)
// Accumulates the frame's damage into ydata->dmg, then fires on_damage_callback if
// set. Every City Trial break family except the star pole leaves it NULL, so this
// cannot break them.
void YakumonoGObj_Proc10(GOBJ *yaku_gobj);                                     // 0x800f5454 (priority 10)
// Adds dmg to yd->dmg, clamped to <= 9999.
void GrYakumono_AccumulateDamage(YakumonoData *yd, float dmg);                 // 0x800f875c

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
// tri_idx is the prop's triangle index within gcp->tri. Returns the coll_func's
// result: 1 if the prop broke.
int  collideWithObject(GOBJ *yaku_gobj, CollData *other, GrCollParam *gcp,
                       int tri_idx, Vec3 *contact);                            // 0x800f5004
// Both return 1 on a break and then set other->yaku_break_speed_cap from
// param[2] * (1 - overshoot / param[1]), clamped at 0, plus req_yaku_break_effect.
// Threshold break: breaks iff force >= param[0], which stays unchanged.
int  GrYaku_TestImpactBreak(const float *param, CollData *other, GrCollParam *gcp,
                            int tri_idx, Vec3 *contact);                       // 0x80104cd4
// Subtractive: *hp -= force, breaks when *hp <= 0. param[0] is unused.
int  GrYaku_ApplyImpactDamage(const float *param, CollData *other, GrCollParam *gcp,
                              int tri_idx, Vec3 *contact, float *hp);          // 0x80104be0

// The family coll_funcs collideWithObject dispatches to. Compare a descriptor's
// coll_func against these to identify a prop's family. Both return 1 on a break.
// YAKUKIND_CORAL / TREE / ROCK. Threshold break. Spawns debris effects and
// credits the break, but state-changes into a broken-state model rather than
// hiding the original mesh inline.
int  hitWeakObject(GOBJ *yaku_gobj, CollData *other, GrCollParam *gcp,
                   int tri_idx, Vec3 *contact);                                // 0x80107914
// YAKUKIND_BREAKHPCOLLDOOR / HOLE / HOUSE. Subtractive break, and does the full
// visible break inline at the passed contact point.
int  hitStrongObject(GOBJ *yaku_gobj, CollData *other, GrCollParam *gcp,
                     int tri_idx, Vec3 *contact);                              // 0x801086d0

// Toggle every triangle of a placed-instance record between collidable
// (enabled != 0) and broken. The break path calls this with 0 to retire a prop.
void grScene_SetInstanceColl(GrCollRecord *record, int enabled);               // 0x800d7ad0
// Returns 1 iff every triangle of the record has its collidable bit == state.
int  grScene_IsInstanceCollAll(GrCollRecord *record, int state);               // 0x800d7b0c

// Credit one broken yakumono to a player's checklist stat.
void YakumonoGObj_IncrementBreakCount(GOBJ *yaku_gobj, int ply);              // 0x80105d80
void Ply_IncrementYakumonoBreakCount(int ply, YakuKind kind);                 // 0x8022fed8

void Gr_StateChange(YakumonoData *yd, int state_idx, int anim_idx, int joint_idx,
                    int flags, float start_frame, float anim_rate, float blend_rate); // 0x800f5548
#define GRSTATECHANGE_NOANIM (1 << 2)

// anim_frame becomes frame - rate, anim_rate becomes rate.
void YakumonoGObj_AddAnim(GOBJ *yaku_gobj, int anim_idx, float frame, float rate); // 0x800f5ce8
void YakumonoGObj_RemoveAnim(GOBJ *yaku_gobj, int anim_idx);                  // 0x800f5f3c

// Runs the per-grkind init hook (28-entry table at 0x804a322c, indexed by the
// physical GroundKind), then always walks grdata->yakumono->entries[].
void grInitYakumono(GrObj *grobj);                                             // 0x800f425c
void grLoadYakumono(void);                                                     // 0x800f440c - loads YkCommon.dat + Yakumono.dat
void Yakumono_Preload(void);                                                   // 0x800f82ec

// Yakumono placement table (category 8 of the stage placement system). Records
// are 0x24 bytes = position plus two orientation/scale vectors. CT breakables
// take their real per-prop transforms from the collision-record pool instead.
int grGetYakumonoposNum(void);                                                 // 0x800d1434
void grGetYakumonoPosition(int num, Vec3 *pos, Vec3 *fwd, Vec3 *up);          // 0x800d145c

// Finds the placed-instance record whose jobj matches `key`.
GrCollRecord *grScene_FindInstanceByKey(GrCollParam *gcp, int key);            // 0x800d7954

// Break drop emitters. All call City_SpawnMiscItems with the per-instance drop
// descriptor, using event_source_drop[].chance_destructible (source enum 3).
// YAKUKIND_EVENTPILLAR's on_damage_callback.
void GrYakuBreakRock_DropItems(GOBJ *yaku_gobj, void *dmg);                   // 0x8010203c
// coll_func of YAKUKIND_BREAKHOUSE and kind 23 (the Destruction Derby 1 rocks).
int  GrYakuBreakHouse_DropItems(GOBJ *yaku_gobj, CollData *other, GrCollParam *gcp,
                                int tri_idx, Vec3 *contact);                   // 0x80102794
// on_damage_callback of the placed BreakCoral kinds (BREAKCORAL, 28, STARPOLE).
void GrYakuBreakCoral_DropItems(GOBJ *yaku_gobj, void *dmg);                  // 0x801040fc

// Per-instance creators: create one yakumono and run its kind-specific tail-init.
void Lighthouse_Create(GrObj *grobj, int data_idx);  // 0x8010d228 (YAKUKIND_LIGHTHOUSE)
void Lighthouse_Init(GOBJ *yaku_gobj);               // 0x8010d260

// The lighthouse, stored by the Lighthouse event's start and left stale once it ends.
// Its lights are on in yakumono state 3.
static GOBJ **stc_lighthouse_gobj = (GOBJ **)(0x805dd0e0 + 0x670);

// Is pos inside the cone from origin to end? t is pos projected onto the segment, and
// the cone's radius there is slope * t + base. The game builds end as the lamp joint's
// position minus its normalized world Y axis times beam_len.
int CityLighthouse_InBeam(Vec3 *pos, Vec3 *origin, Vec3 *end, float slope, float base); // 0x8010d910

// YAKUKIND_UFO's five states, {anim, phys, 0, 0} each in its state table at 0x804a7390.
// Every anim callback drops its state's ring of items: slot 0 a hardcoded ITKIND_ALLUP,
// the rest CityItem_GetEventItem(EVDROP_UFO).
void CityUFO_State0Think(GOBJ *gobj); // 0x8010b024
void CityUFO_State1Think(GOBJ *gobj); // 0x8010b714
void CityUFO_State2Think(GOBJ *gobj); // 0x8010be88
void CityUFO_State3Think(GOBJ *gobj); // 0x8010c560
void CityUFO_State4Think(GOBJ *gobj); // 0x8010cca4
void whispyLogic(GrObj *grobj, int data_idx);        // 0x8010db64 (YAKUKIND_WHISPYWOODS)

// A prop's placed instances, its solid collision and its live transform all live
// in the stage collision pool, walked with Gr_GetCollRecords / Gr_GetCollTris:
// one GrCollRecord per visible prop, its yaku_gobj back-pointing at the owning
// yakumono GObj, and its GrCollTri slice being the solid collision itself -
// there is no separate static wall.

// The game's grYakuFuncTable entry.
typedef struct YakuDesc
{
    void *state_table;        // 0x00 - the per-kind state table immediately preceding this block
    void *coll_func;          // 0x04 - break coll_func, e.g. hitWeakObject / hitStrongObject
    void *x08;                // 0x08
    void *adhere_update_func; // 0x0c
    void *get_point_func;     // 0x10
} YakuDesc;

// The game's grYakuFuncTable: read-only, YAKUKIND_NUM descriptor pointers indexed by
// YakuKind. Each even/odd pair of common kinds shares one descriptor.
static YakuDesc **stc_yaku_descs = (YakuDesc **)0x804a5be8;

// Common-kind create table, 16 wrappers indexed by YakumonoEntry.kind and called with
// (grobj, kind, entry->param). Organized as 8 pairs: the even entry is the base kind,
// the odd its "ctrl" variant (bit 7 of ydata->flags). Both wrappers of a pair share a
// tail-init.
static GOBJ *(**stc_yaku_common_create)(GrObj *, int, void *) =
    (GOBJ *(**)(GrObj *, int, void *))0x804a5ba8;

// Unchecked - confirm gobj->entity_class == YAKUMONO_GOBJ_KIND first if the GObj
// did not come off the GAMEPLINK_YAKUMONO list.
static inline YakumonoData *Yaku_GetData(GOBJ *gobj)
{
    return (YakumonoData *)gobj->userdata;
}

static inline void *Yaku_GetDescCollFunc(YakuKind kind)
{
    YakuDesc *desc = stc_yaku_descs[kind];
    return desc ? desc->coll_func : (void *)0;
}

#endif // KAR_H_YAKUMONO
