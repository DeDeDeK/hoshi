#ifndef KAR_H_YAKUMONO
#define KAR_H_YAKUMONO

// Yakumono - interactive stage props (destructible scenery, hazards, zones,
// boss-like fixed actors). See docs/yakumono-system.md for the full architecture.
//
// All yakumono GObjs share:
//   gobj->kind   = 15  (YAKUMONO_GOBJ_KIND)
//   gobj->p_link = 8   (GAMEPLINK_YAKUMONO)
//   user_data[0] = YakumonoData * (slot kind 14 = GUDATA_YAKUMONO)

#include "datatypes.h"
#include "obj.h"
#include "hurt.h"

// Forward declaration: GrObj is fully defined in stage.h, which itself
// includes this header. Most yakumono APIs only need a pointer to it.
typedef struct GrObj GrObj;

// === GObj wiring ===
// gobj->kind for yakumono GObjs (set by GObj_Create(0xF, 8, 0) in GrYaku_Create).
// Use this for `gobj->kind == YAKUMONO_GOBJ_KIND` to detect a yakumono GObj.
#define YAKUMONO_GOBJ_KIND 15
// User-data slot kind passed to GObj_AddUserData when binding YakumonoData.
#define GUDATA_YAKUMONO    14

// === YakuKind ===
// Per-type kind enum used internally as `gyp->kind`. Source filenames
// (gryaku<X>.c) and assert strings give the names below; absolute integer
// values are not yet confirmed (the enum may have gaps for kind families).
// Kinds are grouped by file. _CommonTerminate is the sentinel above which
// per-kind handlers are invalid (asserted in gryaku.c).
typedef enum YakuKind
{
    YAKUKIND_NONE = -1,

    // gryakudownforcezone.c
    YAKUKIND_DOWNFORCEZONE,
    // gryakucatchzone.c
    YAKUKIND_CATCHZONE,
    // gryakurecoveryzone.c
    YAKUKIND_RECOVERYZONE,
    // gryakurotjumphill.c
    YAKUKIND_ROTJUMPHILL,
    // gryakuinvisibleball.c
    YAKUKIND_INVISIBLEBALL,
    // gryakurisingcube.c (paired Ctrl variant)
    YAKUKIND_RISINGCUBE,
    YAKUKIND_RISINGCUBECTRL,
    // gryakugondola.c
    YAKUKIND_GONDOLA,
    // gryakucannon.c
    YAKUKIND_CANNON,
    // gryakupushoutwall.c
    YAKUKIND_PUSHOUTWALL,
    YAKUKIND_PUSHOUTWALLCTRL,
    // gryakulighttunnel.c
    YAKUKIND_LIGHTTUNNEL,
    // gryakupillar.c
    YAKUKIND_PILLAR,
    YAKUKIND_PILLARCTRL,

    // === Break family ===
    // gryakubreakrock.c - volcano walls, event pillars
    YAKUKIND_BREAKROCK,
    // gryakubreakhouse.c - destructible houses
    YAKUKIND_BREAKHOUSE,
    // gryakuanimfloor.c
    YAKUKIND_ANIMFLOOR,
    // gryakubreakcoral.c - "BigStar" / star pole
    YAKUKIND_BREAKCORAL,
    // gryakubreakicicle.c
    YAKUKIND_BREAKICICLE,
    // gryakulasergate.c
    YAKUKIND_LASERGATE,
    YAKUKIND_LASERGATECTRL,
    // gryakubreakfloor.c - multi-stage cracking
    YAKUKIND_BREAKFLOOR,
    // gryakubreakfan.c
    YAKUKIND_BREAKFAN,
    // gryakubreakcoll.c - shared collision base
    YAKUKIND_BREAKCOLL,
    // gryakubreakhpcoll.c - HP-based contiguous range Door...House
    YAKUKIND_BREAKHPCOLLDOOR,
    YAKUKIND_BREAKHPCOLLWALL,
    YAKUKIND_BREAKHPCOLLPILLAR,
    YAKUKIND_BREAKHPCOLLROOF,
    YAKUKIND_BREAKHPCOLLHOUSE,

    // gryakuwhispywoods.c
    YAKUKIND_WHISPYWOODS,

    YAKUKIND_COMMONTERMINATE, // sentinel - `kind < CommonTerminate` is the bound check

    // Lighthouse uses its own creator (Lighthouse_Create) and doesn't appear
    // in the gryaku<X>.c file family. Its kind constant lives outside the
    // _CommonTerminate range.
    YAKUKIND_LIGHTHOUSE,
} YakuKind;

// === Common groups / random sets ===
// gryakucommon.c bounds. Used by GrYakuCommon_SelectRandomGroup at the end of grInitYakumono
// to randomly enable a subset of common-grouped yakumono entries.
#define GRYAKU_COMMON_GROUP_MAX     20  // GrYakuCommon_Group_Max (asserted in gryaku.c)
#define GRYAKU_COMMON_RANDOM_SET_NUM 10 // GrYakuCommon_Random_Set_Num (upper bound)

// Default scale for all yakumono (asserted in many checks).
#define GR_DEFAULT_SCALE 1.0f

// === YakumonoParam ===
// Per-instance parameter block, pointed to by data_ptr below. Layout is
// kind-specific (each YakuKind imposes its own struct on this memory).
// The arms below are the kinds whose layout is documented; for the rest
// the framework just passes the pointer through without dereferencing it.
typedef union YakumonoParam
{
    // Lighthouse_Init reads these; see Lighthouse_Create at 0x8010d228.
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

    // Most break-* kinds have an optional drop descriptor pointer in their
    // param block. The slot offset varies by family - see event-source-drops.md:
    //   gryakubreakrock.c   → param->[+0x24]
    //   gryakubreakcoral.c  → param->[+0x28]
    //   gryakubreakhouse.c  → param->[+0x30]
    // NULL means "no drop".

    // Common gating fields read by Create's init pipeline. Populated for
    // every visible yakumono in shipped stage data; NULL/empty for kinds
    // the stage doesn't actually use.
    //
    //   +0x04  - read by GrYaku_AllocJObj (0x800f7308). Non-zero → full
    //            JObj alloc with model joint data. Zero → empty alloc,
    //            ydata+0x64 stays NULL (yakumono is invisible).
    //   +0x0c  - read by GrYaku_AttachModel (0x800f6274). Non-zero →
    //            model attach via grdata->model_section / grdata->motion.
    //            Zero → no-model branch (no DObj/MObj/PObj attached).
    //
    // See docs/yakumono-system.md "Spawning yakumono in stages they don't
    // normally appear in" for the full breakdown.
    struct
    {
        int x0;          // 0x00 - kind-specific (cannon: read by tail-init)
        void *jobj_data; // 0x04 - gates JObj alloc; typically a JObjDesc *
        int x8;          // 0x08
        void *model_data;// 0x0c - gates model attach
    } *gates;

    // Generic catch-all when the kind layout is unknown.
    struct
    {
        int x0;
        int x4;
    } *other;

    // Raw bytes for sizing (not authoritative; varies by kind).
    void *raw;
} YakumonoParam;

// === YakumonoData ===
// User-data block at gobj->user_data[0] (offset +0x2c on GObj). All seven
// per-frame procs read it as `ydata = *(YakumonoData**)(gobj + 0x2c)`.
//
// Field offsets are confirmed via decompile of GrYaku_Create, GrYaku_InitData,
// and the 7 procs (priorities 1, 4, 5, 6, 7, 9, 10).
typedef struct YakumonoData
{
    GOBJ *gobj;             // 0x00 - back-reference
    int desc_id;            // 0x04 - index into stc_yaku_descs[70] (passed as r3 to GrYaku_Create)
    YakumonoParam *data_ptr;// 0x08 - per-instance param block (= grdata->yakumono->data_array[data_idx])
    u8 x0c[0x10];           // 0x0c..0x1b
    Vec3 pos;               // 0x1c - position (read by GrYaku_AttachModel for model attach)
    u8 x28[0x18];           // 0x28..0x3f
    f32 local_mtx[12];      // 0x40..0x6f - 3x4 matrix (used by GrYaku_InitMatrix)
    int x70;                // 0x70 - status field (zeroed in Create after JObj setup)
    int state;              // 0x74 - current state-machine state (read by GrYakumono_GetState; -1 initially)
    int prev_anim;          // 0x78
    int prev_joint;         // 0x7c
    int x80;
    void *desc_subblock;    // 0x84 - descriptor's first int (set in InitData from stc_yaku_descs[desc_id][0])
    Vec3 axis_right;        // 0x88 - right/forward axis (init (0,0,1) in InitData)
    Vec3 axis_up;           // 0x94 - up axis (init (0,0,1) in InitData)
    int xa0;                // 0xa0 - (init 0)
    f32 scale;              // 0xa4 - = GR_DEFAULT_SCALE (1.0f)
    u8 xa8[4];              // 0xa8
    f32 xac;                // 0xac - (init 0.0)
    int xb0;                // 0xb0 - (init 5)
    int xb4, xb8;           // 0xb4, 0xb8 - (init 0)
    u8 xbc[0x24];           // 0xbc..0xdf
    Vec3 bbox_center;       // 0xe0 - bbox / model offset (init from constants in GrYaku_FinalSetup)
    HurtData *hurt_data;    // 0xec - HurtData * (HurtData_Create with regions, in GrYaku_InitHurtData)

    // Per-type callbacks. Populated by per-instance tail-init after
    // GrYaku_Create returns. NULL means "no-op" - every proc null-checks.
    void (*proc1)(GOBJ *gobj);                   // 0xf0 - Think (priority 1)
    void (*proc2)(GOBJ *gobj);                   // 0xf4 - priority 4
    void (*proc3)(GOBJ *gobj);                   // 0xf8 - priority 5
    void (*proc4)(GOBJ *gobj);                   // 0xfc - priority 6
    void (*on_damage)(GOBJ *gobj, void *hurt);   // 0x100 - priority 10, fires when this frame had damage
    void (*off_damage)(GOBJ *gobj);              // 0x104 - priority 10, fires when not damaged but had damage state
    void (*proc5)(GOBJ *gobj);                   // 0x108 - priority 7, before HurtData_UpdatePerFrame

    void *misc_table;       // 0x10c - alloc'd by zz_802364e0_ (purpose: ?)
    int x110;               // 0x110 - (init 0)
    int x114;               // 0x114 - (init 0)
    int audio_anim;         // 0x118 - set from anim event list (GrYaku_InitAudio)
    int audio_loop;         // 0x11c
    void *audio_track;      // 0x120 - Map_AllocAudioTrack
    void *audio_source;     // 0x124 - Map_AllocAudioSource
    u8 x128[4];             // 0x128
    u8 flags;               // 0x12c - flag byte. Bit 7 = "ctrl" variant of the
                            //         generic dispatch kind (set by GrYakuFlags_SetCtrl,
                            //         cleared by GrYakuFlags_SetBase). Bits 3, 4, 6 also
                            //         cleared in InitData.
    u8 x12d[3];             // 0x12d..0x12f
    // === Tail (used by per-kind data, mostly the BREAK families) ===
    // Layout below is what gryakubreakcoll.c / gryakubreakhpcoll.c reach into;
    // other YakuKinds use different overlays of this region. See
    // docs/yakumono-system.md "gyp->fgm substruct" for the bracketed extent.
    void *region_audio_arr; // 0x130 - ptr to per-region audio handle array
    void *region_state_arr; // 0x134 - ptr to per-region damage/HP state array
                            //         (BREAK-hp-coll uses this slot as a counter)
    int x138;               // 0x138 - transient: gobj backref / per-region model
                            //         handle (set by GrYaku_BaseKind0_TailInit; rewritten by
                            //         break-coll iteration)
    int x13c;               // 0x13c - transient: per-region proc handle
    int x140;               // 0x140 - BREAK-hp-coll: per-stage audio handle counter
    int x144;               // 0x144 - (?)
    int x148;               // 0x148 - BREAK-hp-coll: transient proc handle (variant slot)
    int x14c;               // 0x14c - BREAK-hp-coll: transient model handle (variant slot)
    void *region_src_arr;   // 0x150 - BREAK-coll: per-region audio-source ptr array
    int x154, x158, x15c;   // (?)
    void *region_src_arr_b; // 0x160 - BREAK-hp-coll: per-region audio-source ptr array
    // sizeof >= 0x164 (lower bound from BREAK family disasm). The actual
    // class size at 0x80557584 is fixed at runtime by HSD_ObjAlloc; static
    // memory is zero-initialized so the size cannot be read directly here.
    // The `gyp->fgm.idDataNum` field referenced in gryakubreakcoll.c /
    // gryakubreakhpcoll.c asserts lives somewhere in this 0x118..0x14c
    // region; bounds check is dead-code-eliminated under -O1.
} YakumonoData;

// === YakumonoTable (per-stage manifest, at GrData+0x40) ===
//
// Two parallel arrays:
//   data_array[] - per-instance param blocks indexed by data_idx in GrYaku_Create
//   entries[]    - kind-tagged spawn entries (16-kind small enum, dispatched
//                  through grYakuFuncTable @ 0x804a5ba8). Used by stages that
//                  rely on the generic walker. City Trial bypasses this via
//                  grDataCity1_CreateYakumono.
typedef struct YakumonoEntry
{
    int kind;       // 0x00 - small 0..15 enum used by grYakuFuncTable
    void *param;    // 0x04 - kind-specific (often a data_idx or position)
    int x08;        // 0x08 - common-group id (used by GrYakuCommon_SelectRandomGroup random subset selector); -1 = none
} YakumonoEntry;

typedef struct YakumonoTable
{
    void **data_array;        // 0x00 - array of per-instance param-block pointers
    int data_count;           // 0x04 - entries in data_array
    int x08;                  // 0x08
    int x0c;                  // 0x0c
    YakumonoEntry *entries;   // 0x10 - kind-tagged spawn entries
    int entry_count;          // 0x14
} YakumonoTable;

// === Per-grkind init hook table (indexed by GroundKind) ===
// 28 entries at 0x804a322c, indexed by gr_kind 0..27 (note: enum
// GRKIND_NUM = 34, so entries 28..33 don't exist in this table - those
// gr_kinds presumably never reach grInitYakumono).
//
// grInitYakumono calls (&PTR_PTR_804a322c)[gr_kind].fn_at_4(grobj) if
// non-NULL, then ALWAYS walks grdata->yakumono->entries[] afterward
// (the hook does not replace the generic walker - both run).
//
// Of the 28 entries, 15 have real init hooks (notably gr_kind 9 =
// GRKIND_CITY1 → grDataCity1_CreateYakumono), 12 have NULL hooks
// (gr_kinds 6, 11, 12, 13, 14, 16, 18, 20, 22, 23, 24, 25), and one is
// a 4-byte `blr` stub (gr_kind 17 = GRKIND_KIRBYMELEE1, functionally
// equivalent to NULL). See docs/yakumono-system.md for the full table.

// === Lifecycle API ===
//
// Create a yakumono GObj. desc_id is the index into the 70-entry descriptor
// table at 0x804a5be8. data_idx indexes grdata->yakumono->data_array. Per-
// instance creator helpers hardcode desc_id and forward data_idx. Returns
// the newly created GObj (in r3) - per-instance creators rely on this so
// they can call their kind-specific tail-init on it immediately after.
GOBJ *GrYaku_Create(int desc_id, int data_idx);                                // 0x800f446c

// Cannon yakumono creator (desc_id 48). Hardcodes desc_id, so its first
// argument is unused - pass NULL or any dummy value. Forwards data_idx (r4)
// to GrYaku_Create, then runs the cannon tail-init that populates proc
// callbacks and zeros the lc.cannon.userInfo[] array.
//
// Vanilla call sites: gr_kind 5 (Machine Passage, data_idx=1) and
// grDataSingleRace4_CreateYakumono (data_idx=18). Both pass a grobj as the
// first arg purely as a convention.
void GrYakuCannon_Create(GrObj *grobj_unused, int data_idx);                   // 0x800fed20
void GrYakuCannon_TailInit(GOBJ *yaku_gobj);                                   // 0x800fed48

// Initialize the YakumonoData fields (called from GrYaku_Create).
void GrYaku_InitData(GOBJ *gobj, int desc_id, void *data_ptr);                 // 0x800f4d50

// Returns ydata->hurt_data (+0xec). NULL-safe: returns the raw value.
HurtData *GrYaku_GetHurtData(GOBJ *gobj);                                      // 0x800f8248

// Returns ydata->state if gobj is a yakumono (gobj->kind == 15), else -1.
int GrYakumono_GetState(GOBJ *gobj);                                           // 0x800f7ab8

// The 7 procs (registered automatically by GrYaku_Create). Listed for
// reference / hooking. Priorities: 1, 4, 5, 6, 7, 9, 10.
void GrYakumono_Think(GOBJ *gobj);                                             // 0x800f5284 (priority 1)
// Priority 4..10 are unnamed (GrYakumono_Proc4/5340_/5374_/53a8_/53fc_/5454_) -
// see docs/yakumono-system.md for the breakdown.

// State / animation transition. Used by per-kind handlers and from the
// init pipeline tail to enter the initial state.
void Gr_StateChange(YakumonoData *yd, int state_idx, int anim_idx, int joint_idx,
                    int flags, float start_frame, float anim_rate, float blend_rate);
#define GRSTATECHANGE_NOANIM (1 << 2) // already in stage.h; kept here as documentation

void Gr_AddAnim(YakumonoData *yd, int anim_idx);                               // 0x800f5ce8
void Gr_RemoveAnim(YakumonoData *yd, int anim_idx);                            // 0x800f5f3c

// === Stage init / load ===
void grInitYakumono(GrObj *grobj);                                             // 0x800f425c
void grLoadYakumono(void);                                                     // 0x800f440c - loads YkCommon.dat + Yakumono.dat
int grGetYakumonoposNum(void);                                                 // 0x800d1434
void Yakumono_Preload(void);                                                   // 0x800f82ec

// === Yaku-break drop emitters (see event-source-drops.md) ===
// All three call into City_SpawnMiscItems with the per-instance drop
// descriptor (gated by NULL check on param[+0x24/0x28/0x30] respectively).
// Drop column is event_source_drop[].chance_destructible (source enum 3).
void GrYakuBreakRock_DropItems(int param);    // 0x8010203c - volcano walls, event pillars
void GrYakuBreakHouse_DropItems(int param);   // 0x80102794 - destructible houses
void GrYakuBreakCoral_DropItems(int param);   // 0x801040fc - "BigStar" / star pole

// === Named per-instance creators ===
// These create a single yakumono of the corresponding kind and run the
// kind-specific tail-init.
void Lighthouse_Create(GrObj *grobj, int data_idx);  // 0x8010d228 (desc_id 68)
void Lighthouse_Init(GOBJ *yaku_gobj);               // 0x8010d260
void whispyLogic(GrObj *grobj, int data_idx);        // 0x8010db64 (desc_id 69) - WhispyWoods

// === Per-grobj globals ===
// These live inside the GrObj struct that *stc_grobj points to (the GrObj
// typedef in stage.h is truncated - the real struct extends past +0x710).
// Confirmed against GrYaku_Create's body (`lwz r27, 1516(r13)` followed by
// `stw r0, 1788(r27)` increments the count) and grInitYakumono's `stw r3,
// 1808(r27)` storing the index array. Both r27 = *(r13 + 0x5ec) = *stc_grobj.

// Live yakumono count (incremented on every successful GrYaku_Create).
// Returns NULL when no GrObj is loaded (between scenes). Uses raw byte
// arithmetic to avoid pulling in stage.h (circular: stage.h includes us).
static inline int *Yaku_GetCountPtr(void)
{
    void *grobj = *(void **)(0x805dd0e0 + 0x5ec); // *stc_grobj
    return grobj ? (int *)((char *)grobj + 0x6fc) : (int *)0;
}

// Per-stage array of YakumonoData pointers, length = yakumono_table->entry_count,
// allocated in grInitYakumono. Filled in spawn order. NULL between scenes.
static inline YakumonoData **Yaku_GetArray(void)
{
    void *grobj = *(void **)(0x805dd0e0 + 0x5ec); // *stc_grobj
    return grobj ? *(YakumonoData ***)((char *)grobj + 0x710) : (YakumonoData **)0;
}

// Read-only descriptor table - 70 pointers, indexed by desc_id.
// Two distinct sections (see docs/yakumono-system.md "Descriptor table"):
//   indices 0..15  - paired generic descriptors (8 unique 40-byte blocks,
//                    each used by two consecutive desc_ids; function ptr at
//                    +0x1c). Used by the entries[] walker via grYakuFuncTable.
//   indices 16..69 - per-instance descriptors (variable-size blocks; function
//                    ptr at +0x08; embedded source-file + assertion strings
//                    starting at +0x1c). Hardcoded by per-grkind hooks like
//                    grDataCity1_CreateYakumono.
// GrYaku_InitData stores *(stc_yaku_descs[desc_id]) into yd->desc_subblock
// (+0x84) - that field is the descriptor's "back-pointer" (16 bytes earlier).
static void **stc_yaku_descs = (void **)0x804a5be8;

// 16-entry function-pointer table indexed by YakumonoEntry.kind (the small
// 0..15 enum, distinct from YakuKind). Organized as 8 pairs - one pair per
// generic base kind, with the odd entry being the "ctrl" variant of the even
// entry. The variant flag is bit 7 of yd->flags (+0x12c): the even wrapper
// calls GrYakuFlags_SetBase (clears it), the odd wrapper calls GrYakuFlags_SetCtrl (sets
// it). Both wrappers in a pair share the same tail-init.
//
// Each wrapper calls GrYaku_Create_Generic (a GrYaku_Create variant that reads its
// param array from r13[0x5e4] = the loaded Yakumono.dat archive, NOT from
// grdata->yakumono->data_array[]).
//
// See docs/yakumono-system.md "Generic dispatch table" for the per-pair
// breakdown.
static void (**stc_yaku_create_dispatch)(GrObj *, int, void *) =
    (void (**)(GrObj *, int, void *))0x804a5ba8;

// === Helpers ===

// Returns true iff the given GObj is a yakumono.
static inline int Yaku_IsYakumonoGObj(GOBJ *gobj)
{
    // Safe early-init read: even pre-Create gobjs have kind set by GObj_Create.
    return gobj && (*(unsigned short *)gobj == YAKUMONO_GOBJ_KIND);
}

// Convenience: pull YakumonoData from a yakumono GObj. Caller must verify
// Yaku_IsYakumonoGObj() first if uncertain - this is unchecked.
static inline YakumonoData *Yaku_GetData(GOBJ *gobj)
{
    return *(YakumonoData **)((char *)gobj + 0x2c);
}

#endif // KAR_H_YAKUMONO
