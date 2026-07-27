#ifndef KAR_H_YAKUMONO
#define KAR_H_YAKUMONO

// Yakumono - interactive stage props (destructible scenery, hazards, zones,
// boss-like fixed actors).
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
// values are unconfirmed (the enum may have gaps for kind families).
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
    // param block. The slot offset varies by family:
    //   gryakubreakrock.c   -> param->[+0x24]
    //   gryakubreakcoral.c  -> param->[+0x28]
    //   gryakubreakhouse.c  -> param->[+0x30]
    // NULL means "no drop".

    // Common gating fields read by Create's init pipeline. Populated for
    // every visible yakumono in shipped stage data; NULL/empty for kinds
    // the stage doesn't actually use.
    //
    //   +0x04  - JObj data (a JObjDesc-ish block). Read by GrYaku_AllocJObj
    //            (0x800f7308): non-zero -> full JObj alloc; zero -> empty alloc,
    //            ydata+0x64 stays NULL (yakumono invisible). Doubles as the
    //            anim gate: GrYaku_AttachAnim (0x800f6394) re-reads this same
    //            field and checks jobj_data[0][0x10] - anim data is bundled
    //            under the JObj descriptor, there is no separate param field
    //            for animation.
    //   +0x0c  - model data. Read by GrYaku_AttachModel (0x800f6274): non-zero
    //            -> model attach via grdata->model_section / grdata->motion;
    //            zero -> no-model branch (no DObj/MObj/PObj attached).
    //   +0x14  - audio descriptor (the gyp->fgm source). Read by
    //            GrYaku_InitAudio (0x800f77dc): non-zero -> fills the fgm/audio
    //            block at ydata+0x118. Layout {idData @0x00, idDataNum @0x04,
    //            track_param @0x08, ... @0x0c/0x10}.
    struct
    {
        int x0;            // 0x00 - kind-specific (cannon: read by tail-init)
        void *jobj_data;   // 0x04 - gates JObj alloc + anim attach
        int x8;            // 0x08
        void *model_data;  // 0x0c - gates model attach
        int x10;           // 0x10
        void *audio_desc;  // 0x14 - gates audio/fgm init (GrYaku_InitAudio)
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
typedef struct YakumonoData
{
    GOBJ *gobj;             // 0x00 - back-reference
    int desc_id;            // 0x04 - index into stc_yaku_descs[70] (passed as r3 to GrYaku_Create)
    YakumonoParam *data_ptr;// 0x08 - per-instance param block (= grdata->yakumono->data_array[data_idx])
    u8 x0c[0x10];           // 0x0c..0x1b
    Vec3 pos;               // 0x1c - position (read by GrYaku_AttachModel for model attach)
    u8 x28[0x18];           // 0x28..0x3f
    u8 x40[0x24];           // 0x40..0x63 - local transform scratch (3x4-ish; copied by GrYaku_InitMatrix path)
    void *model_jobj;       // 0x64 - model JObj root, allocated by GrYaku_AllocJObj (0x800f7308)
                            //        and positioned by GrYaku_AttachModel (0x800f6274, from pos +0x1c)
    u8 x68[8];              // 0x68..0x6f
    void *xform_jobj;       // 0x70 - transform JObj that GrYaku_InitMatrix (0x800f73fc) transforms:
                            //        its world matrix (jobj+0x44) is copied into the GObj render
                            //        object (gobj+0x28) +0x44. CAUTION: GrYaku_Create NULLs this
                            //        field at creation (0x800f4594 stw r0,112), and for City Trial
                            //        break-family props it stays NULL (pos +0x1c, hsd_object
                            //        gobj+0x28, and model_jobj +0x64 are all 0 for them too). Those
                            //        props own no positioned JObj - geometry lives in the stage model
                            //        by joint index - so this is NOT a usable runtime MOVE handle for
                            //        them. It is only non-NULL for dynamic/ctrl kinds whose per-frame
                            //        Proc4 (gated on +0x12c bit 7) runs InitMatrix.
    int state;              // 0x74 - current state-machine state (read by GrYakumono_GetState; -1 initially)
    int prev_anim;          // 0x78
    int prev_joint;         // 0x7c
    int x80;
    void *state_table;      // 0x84 - per-kind STATE TABLE base (array of 16-byte
                            //        state entries). Set in InitData from
                            //        stc_yaku_descs[desc_id][0] (the descriptor's
                            //        "back-pointer"). Gr_StateChange indexes it by
                            //        state (entry = base + state*16) and installs the
                            //        active state's handler into proc1 (+0xf0).
                            //        All-zero for passive kinds (zones); holds real
                            //        handlers for active kinds (cannon state 0 =
                            //        GrYakuCannon_State0, state 1 = GrYakuCannon_State1).
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

    void *effect_group;     // 0x10c - Effect-module group handle, alloc'd by
                            //         GrYaku_AllocEffectGroup (0x800f666c -> 0x802364e0);
                            //         freed in GrYaku_DestroyCallback. (NOT a collision entry.)
    int x110;               // 0x110 - (init 0)
    int x114;               // 0x114 - (init 0)
    // gyp->fgm substruct (FGM = field SFX / effect-id manager). Populated by
    // GrYaku_InitAudio when param+0x14 (the audio descriptor) is non-NULL; the
    // descriptor is {idData @0x00, idDataNum @0x04, track_param @0x08, ...}.
    // The BREAK families read this region as gyp->fgm. fgm.idDataNum lives at
    // +0x11c: the gryakubreakcoll.c bounds check `0 <= fgmId && fgmId <
    // gyp->fgm.idDataNum` (fgmId statically 0) reduces to `idDataNum > 0`,
    // which the compiler emits as a compare of +0x11c.
    void *fgm_iddata;       // 0x118 - gyp->fgm.idData    (= audio descriptor[0x00])
    int fgm_iddatanum;      // 0x11c - gyp->fgm.idDataNum  (= audio descriptor[0x04])
    void *audio_track;      // 0x120 - Map_AllocAudioTrack(audio descriptor[0x08])
    void *audio_emitter;    // 0x124 - Map_AllocAudioEmitter(1)
    u8 x128[4];             // 0x128
    u8 flags;               // 0x12c - flag byte. Bit 7 (0x80) = "ctrl" variant of the
                            //         generic dispatch kind (set by GrYakuFlags_SetCtrl,
                            //         cleared by GrYakuFlags_SetBase); it ALSO gates the
                            //         per-frame matrix rebuild - GrYakumono_Proc4 calls
                            //         GrYaku_InitMatrix only when this bit is set, so static
                            //         props (bit clear) build their matrix once at spawn.
                            //         Bits 3, 4, 6 also cleared in InitData.
    u8 x12d[3];             // 0x12d..0x12f
    // === Tail (used by per-kind data, mostly the BREAK families) ===
    // Layout below is what gryakubreakcoll.c / gryakubreakhpcoll.c reach into;
    // other YakuKinds use different overlays of this region.
    void *region_audio_arr; // 0x130 - ptr to per-region audio handle array.
                            //         For the MULTI-INSTANCE break families
                            //         (CT trees/rocks/houses/holes/coral) this same
                            //         slot is the per-prop CHILD ARRAY: the break
                            //         creator allocates count*4 here and fills it with
                            //         the 0x98-byte scene-instance records (one per
                            //         visible prop, each with its own JObj/world matrix
                            //         and hurt region; record+0x90 = this parent gobj).
                            //         So ONE yakumono GObj manages N placed props - the
                            //         parent's pos/model_jobj/xform_jobj are all 0; the
                            //         real per-prop position is in each child record's
                            //         JObj (record[0]->JObj+0x44) or its cached matrix
                            //         (record+0x2c; translation at +0x38/+0x48/+0x58).
                            //         Records are found via grScene_FindInstanceByKey
                            //         (0x800d7954) over the pool at stc_grobj+0x54.
    void *region_state_arr; // 0x134 - ptr to per-region damage/HP state array, AND the
                            //         child-array count for multi-instance trees/coral
                            //         (the strong/house families keep the count at +0x140
                            //         instead). BREAK-hp-coll uses this slot as a counter.
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
    // sizeof >= 0x164 (lower bound from the BREAK families). The actual
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

// === Per-grkind init hook table ===
// 28 entries at 0x804a322c, indexed 0..27 by the physical GroundKind
// (grobj->gr_kind, file order - so Machine Passage = 5 / GrMachine2 here, NOT its
// StageKind 6). Physical GroundKinds 28+ are absent from this table - those grounds
// presumably never reach grInitYakumono.
//
// grInitYakumono calls (&PTR_PTR_804a322c)[gr_kind].fn_at_4(grobj) if non-NULL, then
// ALWAYS walks grdata->yakumono->entries[] afterward (the hook does not replace
// the generic walker - both run).
//
// Of the 28 entries, 15 have real init hooks (notably GroundKind 9 = GrCity1 ->
// grDataCity1_CreateYakumono), 12 have NULL hooks (GroundKinds 6, 11, 12, 13, 14,
// 16, 18, 20, 22, 23, 24, 25), and one is a 4-byte `blr` stub (GroundKind 17 =
// GrColosseum5 / Kirby Melee 2, functionally equivalent to NULL).

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
// Vanilla call sites: GroundKind 5 (Machine Passage / GrMachine2, data_idx=1) and
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
// Priority-10 on-damage proc. When the prop registered a hit this frame
// (HurtData+0x24 != 0; sentinel is 0.0), it accumulates the damage (HurtData+0x28)
// into YakumonoData+0xac (via GrYakumono_AccumulateDamage) and, IF the per-kind
// on_damage (+0x100) handler is non-NULL, fires it. NOTE: the City Trial break
// families leave +0x100 NULL, so seeding HurtData + calling this does NOT break
// them - the real break is collision-force-driven via the descriptor coll_func
// (see collideWithObject below). Only an already-armed BigStar/star pole sets
// +0x100 and can be finished this way.
void GrYakumono_Proc10(GOBJ *gobj);                                            // 0x800f5454 (priority 10)
// Adds dmg to YakumonoData+0xac (clamped to <= 9999). Called by Proc10 on a hit.
void GrYakumono_AccumulateDamage(GOBJ *gobj, float dmg);                       // 0x800f875c
// Priority 4..9 are unnamed except GrYakumono_Proc5/6/7.

// === Break path (collision-force-driven) ===
// The real break for CT props. collideWithObject resolves the prop's descriptor
// coll_func (stc_yaku_descs[YakumonoData+0x04] -> +0x04) and calls it with the
// impacting collider's CollData, which computes a force and compares it to the
// prop's HP. The two force helpers below are what the coll_funcs call:
//   GrYaku_TestImpactBreak  - one-shot threshold (break iff force > HP; HP unchanged)
//   GrYaku_ApplyImpactDamage - subtractive HP (HP -= force; break when HP <= 0)
// where force = collider.radius (CollData+0x344) x impactSpeed^2.
//
// impactSpeed is NOT |pos_delta|: grScene_GetImpactSpeed (0x800d8edc) projects the
// collider's frame delta (CollData+0x14) onto the contacted region's outward normal
// (region+0x0c), NEGATES it (the internal scale const at 0x805df634 is -1.0), and
// clamps a non-positive result to 0. So the delta must point INTO the surface
// (against the outward normal) to register as a real impact; a delta whose projection
// onto the normal is >= 0 yields impactSpeed 0 -> force 0 -> no break.
//
// A no-contact break can be SYNTHESIZED by calling collideWithObject directly. It
// dispatches by family (weak/strong/floor/BigStar), and each handler runs the full
// break tail - collision retire, mesh hide, debris + item drops, SFX, break-count
// credit, broken-state transition - so synthesizing one call breaks a prop with all
// genuine consequences. Arguments:
//   yaku_gobj  - the prop's parent GObj (= record+0x90).
//   collB      - the ground scene-collision holder (stc_grobj+0x54): collB+0x08 is the
//                base of the global mpColl region array (0x40 stride), collB+0x10/+0x14
//                the placed-instance record pool/count. This is the holder the engine's
//                own break-dispatch path indexes regions from (it is also what
//                grScene_FindInstanceByKey takes). NOTE: stc_grobj+0x454 is an UNRELATED
//                struct - using it makes every region index negative and nothing breaks.
//   regionIdx  - the prop's first region's global index =
//                (record+0x0c - *(collB+0x08)) / 0x40.
//   collC      - a contact-point Vec3 (the prop's cached world pos works).
//   otherCollData - a SYNTHETIC collider (a zeroed buffer with these fields set):
//                +0x344 radius - the reliable force lever (force scales linearly with
//                   it); crank far above any HP for a one-hit break even at low speed.
//                +0x14 frame-delta - MUST point into the surface: set it to
//                   -normalize(region+0x0c normal) x M (any M>0). An arbitrary delta
//                   can project >= 0 onto the normal and yield impactSpeed 0 (no break).
//                +0x04 its GObj - read by the gryakubreakcommon attacker->player
//                   attribution for the break-count credit; pass the human rider GObj.
//                +0x44 an mpCollInfo the BigStar guard (destroyBigStar 0x800d7b8c)
//                   walks: a zeroed buffer works, and setting collInfo+0x1d0 = -1
//                   ("no BigStar region") makes destroyBigStar cleanly return 0 so the
//                   break proceeds.
//                The impact-speed calc takes a geometry-refined path when the target
//                region's +0x34 bit 5 is set (which can rewrite a synthetic delta from
//                the prop's matrices); clear it for the call to keep the flat path.
//                The break tail's "still collidable?" guard (grScene_IsInstanceCollAll
//                with 1) must pass, so if the prop's collision was retired beforehand
//                re-arm it (grScene_SetInstanceColl(record, 1)) right before the call.
int  collideWithObject(GOBJ *yaku_gobj, void *otherCollData, void *collB,
                       int regionIdx, void *collC);                            // 0x800f5004
int  GrYaku_TestImpactBreak(void *hp, void *otherCollData, void *collB, void *collC); // 0x80104cd4
int  GrYaku_ApplyImpactDamage(void *hp, void *otherCollData, void *collB, void *collC); // 0x80104be0

// The family coll_funcs collideWithObject dispatches to (resolved from
// stc_yaku_descs[desc]->[+0x04]). The CT break families split into:
//   hitWeakObject    - coral 33 / trees 34 / rocks 35. Threshold break
//                      (GrYaku_TestImpactBreak). Spawns its break debris EFFECTS and
//                      credits the break, but does NOT hide/shatter the original model
//                      inline - it Gr_StateChanges the prop into a broken-state model
//                      that is meant to render at the prop's baked spot. (Its mesh-hide
//                      branch is gated on hp_block[5], which is 0 for these.)
//   hitStrongObject  - walls 36 / holes 37 / houses 38. Subtractive break
//                      (GrYaku_ApplyImpactDamage). Does the full visible break INLINE
//                      using the passed contact point (debris + the per-desc drop
//                      handler from the DAT_804a70b4 table + broken state), so a
//                      synthesized break renders correctly wherever the contact is.
// Declared so mod code can identify the weak families by coll_func (the robust test
// for "does this break hide its own model").
void hitWeakObject(GOBJ *yaku_gobj, void *otherCollData, void *collB,
                   int regionIdx, void *collC);                                // 0x80107914
void hitStrongObject(GOBJ *yaku_gobj, void *otherCollData, void *collB,
                     int regionIdx, void *collC);                              // 0x801086d0

// === Break consequences (driven by the coll_func break tail; reusable from mod
// code to break a prop without a real collision) ===
//
// Each placed-prop scene-instance record (see "Ground scene-instance pool" below)
// owns its mpColl regions inline: an array at record+0x0c, count at record+0x10,
// 0x40-byte stride; region+0x3c bit 6 (0x40) is the "collidable/intact" flag, and
// region+0x38 back-points to the owning record. These regions ARE the prop's solid
// collision - there is NO separate static wall (a record's +0x0c regions are a
// contiguous slice of the global region array at *(stc_grobj+0x5c), and
// the rider's penetration response mpResponse_DispatchSceneObjColl (0x80248bb4) drops
// any contact whose region bit 6 is clear). Only break/init code writes the bit -
// nothing re-arms it per frame - so a mod that clears it (to retire a swept-up prop's
// collision) can rely on the clear sticking until it re-arms it.
//
// Toggle every region's collidable bit on a scene/collision instance record.
// enabled != 0 sets the bit (intact), 0 clears it (broken / non-collidable) -
// exactly what the break path does to retire a prop's collision (the coll_func
// calls this with 0 once force > HP). No-op for a record with no regions.
void grScene_SetInstanceColl(void *record, int enabled);                       // 0x800d7ad0
// Returns 1 iff every region of the record has its collidable bit == state.
int  grScene_IsInstanceCollAll(void *record, int state);                       // 0x800d7b0c

// Credit one broken-yakumono to a player's checklist stat: reads the GObj's
// desc_id and forwards (player_idx, desc_id) to Ply_IncrementYakumonoBreakCount.
void GrYaku_IncrementBreakCount(GOBJ *yaku_gobj, int player_idx);              // 0x80105d80
void Ply_IncrementYakumonoBreakCount(int player_idx, int desc_id);            // 0x8022fed8

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
void Yakumono_Preload(void);                                                   // 0x800f82ec

// === Yakumono placement table (category 8 of the unified 9-category stage
// placement system). Records
// are 0x24 bytes = 3 Vec3 (position + two orientation/scale vectors). Count
// comes from grdata->yakumono_pos (GrData+0x20)->[+0x2c]->[+0x8]; the per-stage
// record array base is cached at stc_grobj+0x15c. Read by grResolvePlacementRef
// (the per-descriptor placement resolver) and the dbPosition debug editor.
// NOTE: CT breakables get their actual per-prop transforms from the scene/
// collision instance pool (sourced from grdata->pos_data, GrData+0x18), NOT
// necessarily from this table - this table may not be the source for relocating
// a specific prop.
int grGetYakumonoposNum(void);                                                 // 0x800d1434
void loadYakumonoLocations(int index, Vec3 *out0, Vec3 *out1, Vec3 *out2);     // 0x800d145c

// Searches the ground scene/collision instance pool (0x98-byte records at
// stc_grobj+0x64, count at +0x68) for the record whose key (record[+0]) matches
// `key`, returning the record pointer. `pool_base` is stc_grobj+0x54. Used by
// the multi-instance break creators to bind each placed prop.
void *grScene_FindInstanceByKey(void *pool_base, int key);                     // 0x800d7954

// === Yaku-break drop emitters ===
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
// GrYaku_Create increments the live count at +0x6fc; grInitYakumono stores the
// index array at +0x710. Both are reached via *(r13 + 0x5ec) = *stc_grobj.

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

// === Ground scene-instance pool (placed-prop records) ===
// The ground runtime object holds a CONTIGUOUS array of 0x98-byte placed-
// instance records - the stage's positioned mesh/collision instances, parsed
// from grdata->pos_data. This is the array grScene_FindInstanceByKey searches
// (array base at GrObj+0x64, live count at GrObj+0x68).
//
// The multi-instance break families bind each visible prop to one of these
// records and stamp the owning parent yakumono GObj into record+0x90, so this
// pool is the family-agnostic way to reach EVERY breakable prop (the parent's
// own +0x130 layout differs per family - an array for trees/houses, a single
// pointer for the star pole). Each record:
//   +0x00  JObj *  - the sub-instance JObj; its world matrix (JObj+0x44) is the
//                    prop's real transform (what renders and what the hurtbox
//                    tracks). Move a prop by rewriting this matrix's translation.
//   +0x2c  Mtx     - cached 3x4 row-major copy of that world matrix (load-time).
//   +0x90  GOBJ *  - owning parent yakumono GObj (set by the break creators;
//                    0 for non-break scene instances).
#define YAKU_INSTANCE_SIZE 0x98 // stride of one placed-instance record
#define YAKU_INST_JOBJ     0x00 // record -> sub-instance JObj (world mtx at +0x44)
#define YAKU_INST_COLL_ARR 0x0c // record -> mpColl region array (0x40-byte stride)
#define YAKU_INST_COLL_NUM 0x10 // record -> mpColl region count
#define YAKU_INST_MATRIX   0x2c // record -> cached 3x4 world matrix copy
#define YAKU_INST_PARENT   0x90 // record -> owning yakumono GObj

// Scene-instance pool base; writes the live record count through out_count.
// Returns NULL (count 0) between scenes. Raw byte arithmetic to avoid stage.h.
static inline void *Yaku_GetInstancePool(int *out_count)
{
    void *grobj = *(void **)(0x805dd0e0 + 0x5ec); // *stc_grobj
    if (grobj == (void *)0)
    {
        if (out_count) *out_count = 0;
        return (void *)0;
    }
    if (out_count) *out_count = *(int *)((char *)grobj + 0x68);
    return *(void **)((char *)grobj + 0x64);
}

// i-th placed-instance record in the pool (no bounds check).
static inline void *Yaku_GetInstance(void *pool, int i)
{
    return (char *)pool + i * YAKU_INSTANCE_SIZE;
}

// The yakumono GObj that owns this placed-instance record (NULL for non-break
// scene instances). Compare against known parents by pointer - do not deref.
static inline GOBJ *Yaku_InstanceParent(void *record)
{
    return *(GOBJ **)((char *)record + YAKU_INST_PARENT);
}

// The sub-instance JObj carrying the prop's world matrix (NULL if unset).
static inline void *Yaku_InstanceJObj(void *record)
{
    return *(void **)((char *)record + YAKU_INST_JOBJ);
}

// World translation from the record's cached 3x4 row-major matrix
// (YAKU_INST_MATRIX): translation = float indices 3/7/11 (record +0x38/+0x48/+0x58).
static inline void Yaku_InstanceCachedPos(void *record, Vec3 *out)
{
    float *m = (float *)((char *)record + YAKU_INST_MATRIX);
    out->X = m[3];
    out->Y = m[7];
    out->Z = m[11];
}

// === mpColl region (one entry of the global region array) ===
// Each scene-instance record owns its regions inline (array at record+0x0c,
// count at record+0x10), and that array is a contiguous YAKU_REGION_SIZE-strided
// slice of the global region array based at Yaku_GetRegionArray(). The break path
// (and a synthesized break, collideWithObject) reads:
typedef struct YakuCollRegion
{
    u8   _pad00[0x0c];
    Vec3 normal;        // 0x0c outward normal; grScene_GetImpactSpeed projects the
                        //      impact delta onto it (must be non-degenerate)
    u8   _pad18[0x34 - 0x18];
    u32  refine_flags;  // 0x34 bit YAKU_REGION_REFINE selects the geometry-refined
                        //      impact path (can rewrite a synthetic delta from the
                        //      prop matrices)
    void *record;       // 0x38 back-pointer to the owning scene-instance record
    u32  coll_flags;    // 0x3c bit YAKU_REGION_COLLIDABLE = collidable/intact
                        //      (grScene_SetInstanceColl toggles it;
                        //      mpResponse_DispatchSceneObjColl drops a clear contact)
} YakuCollRegion;
#define YAKU_REGION_SIZE       0x40 // stride of one region entry
#define YAKU_REGION_REFINE     0x20 // refine_flags bit: geometry-refined impact path
#define YAKU_REGION_COLLIDABLE 0x40 // coll_flags bit: collidable / intact
_Static_assert(sizeof(YakuCollRegion) == YAKU_REGION_SIZE, "YakuCollRegion must be 0x40 bytes");

// === Ground scene-collision holder (GrObj+0x54) ===
// The holder the engine's break dispatcher (collideWithObject) indexes regions
// from - passed straight through as its collB, and the pool_base
// grScene_FindInstanceByKey takes. Sub-fields:
//   +0x08  base of the global YakuCollRegion array.
//   +0x10  the placed-instance record pool base (== Yaku_GetInstancePool's base).
//   +0x14  the record count.
// NULL between scenes.
#define YAKU_COLL_HOLDER_OFF 0x54

static inline void *Yaku_GetCollHolder(void)
{
    void *grobj = *(void **)(0x805dd0e0 + 0x5ec); // *stc_grobj
    return grobj ? (void *)((char *)grobj + YAKU_COLL_HOLDER_OFF) : (void *)0;
}

// Base of the global YakuCollRegion array (holder+0x08); NULL between scenes.
static inline YakuCollRegion *Yaku_GetRegionArray(void)
{
    void *holder = Yaku_GetCollHolder();
    return holder ? *(YakuCollRegion **)((char *)holder + 8) : (YakuCollRegion *)0;
}

// A record's own mpColl region slice (record+0x0c) and region count (record+0x10).
// The slice is a contiguous run within Yaku_GetRegionArray().
static inline YakuCollRegion *Yaku_InstanceRegions(void *record)
{
    return *(YakuCollRegion **)((char *)record + YAKU_INST_COLL_ARR);
}
static inline int Yaku_InstanceRegionCount(void *record)
{
    return *(int *)((char *)record + YAKU_INST_COLL_NUM);
}

// The grobj node/JObj registry (GrObj+0x104) - the per-stage node table
// Gr_GetNodeWorldPos resolves a node id through. Returns the JObj for `node_id`,
// or NULL between scenes / for a negative id. The caller must validate the
// returned pointer before writing through it.
static inline void *Yaku_GetSceneNodeJObj(int node_id)
{
    void *grobj = *(void **)(0x805dd0e0 + 0x5ec); // *stc_grobj
    if (grobj == (void *)0 || node_id < 0)
        return (void *)0;
    void *reg = *(void **)((char *)grobj + 0x104);
    return reg ? *(void **)((char *)reg + node_id * 8) : (void *)0;
}

// Read-only descriptor table - 70 pointers, indexed by desc_id. Two sections:
//   indices 0..15  - paired generic descriptors (8 unique 40-byte blocks,
//                    each used by two consecutive desc_ids; DescFunc ptr at
//                    +0x1c). Used by the entries[] walker via grYakuFuncTable.
//   indices 16..69 - per-instance descriptors (variable-size blocks; a kind
//                    init/check fn ptr at +0x08 for some kinds; embedded
//                    source-file + assertion strings, typically starting near
//                    +0x14). Hardcoded by per-grkind hooks like
//                    grDataCity1_CreateYakumono. The source string names the
//                    YakuKind (e.g. desc 48 = "gryakucannon.c").
// The descriptor's +0x00 field (its "back-pointer") points to the per-kind
// STATE TABLE that immediately precedes the descriptor - NOT uniform 16-byte
// padding: the gap is 0x10 for passive zone kinds and 0x20 for the cannon, and
// the bytes are real state-handler pointers (all-zero only for passive kinds).
// GrYaku_InitData copies it into yd->state_table (+0x84); Gr_StateChange
// indexes it by state (entry = base + state*16).
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

// A break family's coll_func: stc_yaku_descs[desc_id]->[+0x04] (the descriptor's
// break dispatcher, e.g. hitWeakObject / hitStrongObject). NULL if unset. Compare
// the result against a known coll_func to identify the family.
static inline void *Yaku_GetDescCollFunc(int desc_id)
{
    void *desc = stc_yaku_descs[desc_id];
    return desc ? *(void **)((char *)desc + 4) : (void *)0;
}

#endif // KAR_H_YAKUMONO
