#ifndef KAR_H_STAGE
#define KAR_H_STAGE

#include "datatypes.h"
#include "obj.h"
#include "hurt.h"
#include "collision.h"
#include "yakumono.h"

// Fully defined by the event system.
typedef struct EventConfigData EventConfigData;

#define GRSTATECHANGE_NOANIM (1 << 2)

// The 0..59 stage selection index - a stage's menu/mode-level identity, stored
// in GameData.stage_kind (+0xA97) and returned by Gm_GetCurrentStageKind. For Air
// Ride it equals AirRideCourse; City Trial stadiums occupy 9..33.
// Gm_GetGrKindFromStageKind maps one to its physical GroundKind. The two index
// spaces coincide only at 0/1/2 and City Trial (9), because menu order is not
// file order - Machine Passage is StageKind 6 but GroundKind 5.
typedef enum StageKind
{
    STAGEKIND_CITY1 = 9,
    STAGEKIND_DRAG1,
    STAGEKIND_DRAG2,
    STAGEKIND_DRAG3,
    STAGEKIND_DRAG4,
    STAGEKIND_AIRGLIDER,
    STAGEKIND_TARGETFLIGHT,
    STAGEKIND_HIGHJUMP,
    STAGEKIND_KIRBYMELEE1,
    STAGEKIND_KIRBYMELEE2,
    STAGEKIND_DESTRUCTIONDERBY1,
    STAGEKIND_DESTRUCTIONDERBY2,
    STAGEKIND_DESTRUCTIONDERBY3,
    STAGEKIND_DESTRUCTIONDERBY4,
    STAGEKIND_DESTRUCTIONDERBY5,
    STAGEKIND_SINGLERACE1,
    STAGEKIND_SINGLERACE2,
    STAGEKIND_SINGLERACE3,
    STAGEKIND_SINGLERACE4,
    STAGEKIND_SINGLERACE5,
    STAGEKIND_SINGLERACE6,
    STAGEKIND_SINGLERACE7,
    STAGEKIND_SINGLERACE8,
    STAGEKIND_SINGLERACE9,
    STAGEKIND_VSKINGDEDEDE, // = 33
    STAGEKIND_NUM = 60,
} StageKind;

// The physical ground-file index - which terrain geometry is loaded - into the
// stage-file table at 0x804A2FFC. Stored in GrObj.gr_kind and returned by
// Gr_GetCurrentGrKind; stc_grdatalookup is indexed by it. Many StageKinds can
// share one GroundKind. The enum is intentionally sparse: only members present
// in the file table are named.
//
// The 9 Air Ride grounds occupy 0..8 in file order, not menu order - AirRide
// course 1 (Magma Flows) is GroundKind 1 but course 3 (Frozen Hillside) is
// GroundKind 8.
typedef enum GroundKind
{
    GR_PLANTS1    = 0,  // Air Ride: Fantasy Meadows
    GR_HEAT2      = 1,  // Air Ride: Magma Flows
    GR_DESERT1    = 2,  // Air Ride: Sky Sands
    GR_CHECK2     = 3,  // Air Ride: Checker Knights
    GR_VALLEY2    = 4,  // Air Ride: Celestial Valley
    GR_MACHINE2   = 5,  // Air Ride: Machine Passage
    GR_SPACE2     = 6,  // Air Ride: Nebula Belt
    GR_SKY2       = 7,  // Air Ride: Beanstalk Park
    GR_ICE1       = 8,  // Air Ride: Frozen Hillside (last AR course ground)
    GR_CITY1      = 9,  // City Trial
    GR_ZEROYON1   = 10, // drag-race ground (GrZeroyon1)
    GR_PASTURE1   = 14, // Kirby Melee 1
    GR_COLOSSEUM1 = 15,
    GR_COLOSSEUM3 = 16,
    GR_COLOSSEUM5 = 17, // Kirby Melee 2
    GR_JUMP1      = 18, // High Jump ground
    GR_DEDEDE1    = 21, // Vs. King Dedede ground
} GroundKind;

// Air Ride course indices (0-8), a subset of StageKind used for Air Ride mode
typedef enum AirRideCourse
{
    AIRRIDE_FANTASY_MEADOWS,
    AIRRIDE_MAGMA_FLOWS,
    AIRRIDE_SKY_SANDS,
    AIRRIDE_FROZEN_HILLSIDE,
    AIRRIDE_BEANSTALK_PARK,
    AIRRIDE_CELESTIAL_VALLEY,
    AIRRIDE_MACHINE_PASSAGE,
    AIRRIDE_CHECKER_KNIGHTS,
    AIRRIDE_NEBULA_BELT,
    AIRRIDE_NUM,
} AirRideCourse;

static const char *const AirRideCourse_Names[AIRRIDE_NUM] = {
    [AIRRIDE_FANTASY_MEADOWS]  = "Fantasy Meadows",
    [AIRRIDE_MAGMA_FLOWS]      = "Magma Flows",
    [AIRRIDE_SKY_SANDS]        = "Sky Sands",
    [AIRRIDE_FROZEN_HILLSIDE]  = "Frozen Hillside",
    [AIRRIDE_BEANSTALK_PARK]   = "Beanstalk Park",
    [AIRRIDE_CELESTIAL_VALLEY] = "Celestial Valley",
    [AIRRIDE_MACHINE_PASSAGE]  = "Machine Passage",
    [AIRRIDE_CHECKER_KNIGHTS]  = "Checker Knights",
    [AIRRIDE_NEBULA_BELT]      = "Nebula Belt",
};

// Top Ride course indices (0-6), stored in GameData[0x374]
typedef enum TopRideCourse
{
    TOPRIDE_GRASS,
    TOPRIDE_SAND,
    TOPRIDE_SKY,
    TOPRIDE_FIRE,
    TOPRIDE_LIGHT,
    TOPRIDE_WATER,
    TOPRIDE_METAL,
    TOPRIDE_NUM,
} TopRideCourse;

static const char *const TopRideCourse_Names[TOPRIDE_NUM] = {
    [TOPRIDE_GRASS] = "Grass",
    [TOPRIDE_SAND]  = "Sand",
    [TOPRIDE_SKY]   = "Sky",
    [TOPRIDE_FIRE]  = "Fire",
    [TOPRIDE_LIGHT] = "Light",
    [TOPRIDE_WATER] = "Water",
    [TOPRIDE_METAL] = "Metal",
};

typedef struct GrModelMotionAnim
{
    AnimJointDesc *anim_joint;
    MatAnimJointDesc *matanim_joint;
    int x8;
    int xc;
    int x10;
} GrModelMotionAnim;

typedef struct GrModelMotion  // exists in the stage file
{                             //
    GrModelMotionAnim anim[]; // variable amount of these depending on the stage
} GrModelMotion;              //

// 4 dwords pointed at by GrData.model_section. Slots 0 and 1 are JOBJDesc**, so
// a stage can ship a terrain mesh, a separate skybox/backdrop mesh, both, or
// neither. The loader reads +0x04 and attaches the result to GrObj+0xF4.
typedef struct ModelSection
{
    JOBJDesc **terrain;  // 0x00 - main playable geometry
    JOBJDesc **backdrop; // 0x04 - secondary skybox/horizon mesh
    void *unk_8;         // 0x08
    void *unk_c;         // 0x0C
} ModelSection;

// Defined further down; HSD_FogDesc comes from obj.h.
struct SkyPresetEntry;

// The {array, count} pair at SkyBlock+0x04. Repointing both fields swaps in an
// extended preset array.
typedef struct SkyPresetSubHeader
{
    struct SkyPresetEntry *preset_array; // 0x00 - base of the 0x48-byte preset entries
    s32 preset_count;                    // 0x04 - number of presets in the array
} SkyPresetSubHeader;

// The GrData+0x34 sub-block: the initial fog descriptor and the preset sub-header.
typedef struct SkyBlock
{
    HSD_FogDesc *fog_desc;             // 0x00 - initial fog parameters
    SkyPresetSubHeader *preset_header; // 0x04 - sky preset array + count
} SkyBlock;

// The GrData+0x04 sub-block (HSDLib KAR_grStageNode): the stage's physics
// constants, global gravity, and the axis-aligned out-of-bounds death box that
// calcDistanceFromOOB reads every frame.
//
// Gravity splits into a magnitude (gravity_strength) and a unit down direction
// (gravity_dir), both returned by Gm_GetDownVector. To change how strong gravity
// feels, scale the strength and leave the direction unit-length - consumers
// derive an up vector from it.
typedef struct StageNode
{
    int x0;                 // 0x00
    float machine_accel;    // 0x04 - base machine acceleration scalar
    float scale;            // 0x08 - stage model scale (applied to stage JObjs)
    float gravity_strength; // 0x0C - global gravity magnitude / fall-accel scalar
                            //        (City Trial = 0.025); returned by Gm_GetDownVector
    Vec3 gravity_dir;       // 0x10 - global down DIRECTION, unit (0,-1,0); written
                            //        into Gm_GetDownVector's out-param. Keep unit.
    int fog_flags;          // 0x1C
    u8 x20[0x60 - 0x20];
    float minimap_scale;    // 0x60
    u8 x64[0x80 - 0x64];
    int node_flags;         // 0x80
    u8 x84[0xCC - 0x84];
    Vec3 oob_min;           // 0xCC - out-of-bounds box minimum corner (X,Y,Z)
    Vec3 oob_max;           // 0xD8 - out-of-bounds box maximum corner (X,Y,Z)
} StageNode;

typedef struct GrData // exists in the stage file
{                     //
    int flags;                      // 0x00
    StageNode *stage_node;          // 0x04 - physics + gravity + OoB death box
    int x8;                         // 0x08
    ModelSection *model_section;    // 0x0c - terrain + backdrop JObj descs
    GrModelMotion *motion;          // 0x10, pointer placed at runtime
    void *spline;          // 0x14
    void *pos_data;        // 0x18
    int x1c;               // 0x1c
    void *yakumono_pos;    // 0x20 - yakumono position-record block; grGetYakumonoposNum reads [+0x2c]->[+0x8] as the record count (0x800d1434)
    int x24;               // 0x24
    int x28;               // 0x28
    int x2c;               // 0x2c
    EventConfigData *event_config; // 0x30 - set on entering City Trial, whatever the events on/off
                           //        setting. Also stored as EventCheckData.data when events are on.
    SkyBlock *sky_block;   // 0x34 - sky/fog descriptor pair: [0] HSD_FogDesc, [1] preset sub-header (array base + count)
    int x38;               // 0x38
    int x3c;               // 0x3c
    YakumonoTable *yakumono; // 0x40 - per-stage yakumono manifest
} GrData;        //

// The "CZK" tag carried by every collision-zone box face, packed into
// GrCollFace.kind_word. Dash*, Warp*, SuperJump, Jump and Spin are the names
// grlib.c asserts on; the rest are named after the Japanese texture label
// GrSimple paints on the matching zone box. Every consumer is an explicit
// compare, so a kind no compare mentions can never fire.
typedef enum GrCollZoneKind
{
    GrCZK_DashGateA   = 2,  // grGetDashGateZoneParam, GrDashGate_Num = 2
    GrCZK_DashGateB   = 3,
    GrCZK_DashRing    = 4,  // grGetDashGateZoneParam, GrDashRing_Num = 2
    GrCZK_WarpIn      = 5,  // UNIMPLEMENTED - see the warp note below
    GrCZK_WarpOut     = 6,  // UNIMPLEMENTED - "
    GrCZK_SuperJump   = 7,  // grGetSuperJumpZoneParam
    GrCZK_SuperJumpApproach = 8,
    GrCZK_Jump        = 9,  // grGetJumpZoneParam
    GrCZK_Spin        = 10, // grGetSpinZoneParam, GrSpinZone_Num = 2
    GrCZK_FreeMove    = 16,
    GrCZK_LocalDead   = 25, // grGetLocalDeadZoneParam
    GrCZK_Occlusion   = 26,
    GrCZK_BoxArea     = 30,
    GrCZK_AreaLight   = 32, // most common kind; every stage carries some
    GrCZK_Reverb      = 34,
    GrCZK_GateOpen    = 35,
} GrCollZoneKind;

// One triangle of a zone box, as authored in the stage file. A zone is always
// 8 vertices / 12 triangles, grouped into 6 box faces that each carry a kind.
typedef struct GrCollFace
{
    int group;      // 0x00 - which of the box's 6 faces (0..5)
    int vtx[3];     // 0x04 - indices into GrCollisionNode.zone_vtx
    u32 kind_word;  // 0x10 - kind | (param_index << 28); see GrCollZoneKind
    int x14;        // 0x14
} GrCollFace;

typedef struct GrCollZone
{
    int joint;      // 0x00 - index into GrObj.joint_table (8-byte stride)
    int vtx_base;   // 0x04 - first vertex in GrCollisionNode.zone_vtx
    int vtx_num;    // 0x08 - asserted == 8
    int face_base;  // 0x0c - first face in GrCollisionNode.faces
    int face_num;   // 0x10 - asserted == 12
    int x14;
    u8 x18[0x4C - 0x18];
} GrCollZone;

// GrData.pos_data (+0x18) points here - a paired {pointer, count} mirror of the
// runtime GrCollParam. grColl_Alloc sizes every runtime array straight from
// these counts with no headroom, so tri_num here is exactly GrCollParam.tri_num.
typedef struct GrCollisionNode
{
    Vec3 *mesh_vtx;      // 0x00 - baked world-space collision vertices
    int mesh_vtx_num;    // 0x04
    void *tris;          // 0x08 - 0x14 stride; kind at +0x0C, flags at +0x10
    int tri_num;         // 0x0c
    void *records;       // 0x10 - 0x1C stride; first triangle index at +0x0C
    int record_num;      // 0x14
    Vec3 *zone_vtx;      // 0x18 - feeds GrCollParam.aux; always zone_num * 8
    int zone_vtx_num;    // 0x1c
    GrCollFace *faces;   // 0x20 - 0x18 stride; always zone_num * 12
    int face_num;        // 0x24
    GrCollZone *zones;   // 0x28 - 0x4C stride
    int zone_num;        // 0x2c - asserted < Gr_CollZone_NumMax (500)
} GrCollisionNode;

// Per-kind zone-parameter getters. Each asserts the zone it is handed carries
// its kind, then fills the caller's out-params from the per-kind param block.
void grGetDashZoneParam(void);                 // 0x800d1ff0
void grGetDashGateZoneParam(void);             // 0x800d21f8 - kinds 2, 3 and 4
void grGetSuperJumpZoneParam(void);            // 0x800d24fc - kind 7
void grGetJumpZoneParam(void);                 // 0x800d25a8 - kind 9
void grGetSpinZoneParam(void);                 // 0x800d2654 - kind 10
void grGetLocalDeadZoneParam(void);            // 0x800d50f8 - kind 25
void grZone_BuildRecord(void);                 // 0x800dcf08 - GrCollZone -> 0x140 runtime record

// Narrowphase primitives, both taking a triangle index into GrCollParam.tri and
// applying the kind_mask and state gates.
void grColl_RayVsTri(void);            // 0x800d95dc - segment vs one triangle by index
void grColl_SweptSphereVsTri(void);    // 0x802448b0 - swept sphere vs one triangle by index
void grColl_SweptSphereQuery(void);    // 0x800d9e34 - moving sweep + tree walk; moving pass gated on arg 5

// Rider and machine wall/floor pushback, under mpColl_UpdateCollision
// (0x802485e0). The sweep is what a machine is actually stopped by.
void mpColl_SweptSphereMapColl(void);  // 0x802454f8 - swept sphere vs map tris, moving pass always
void mpColl_InsertContact(void);       // 0x80241ca8 - caches the winning tri id in the floor/wall/ceiling slot
void grColl_TreeInit(void);            // 0x800de0e8 - fills GrObj.coll_tree; adds objKinds 3-5
void grColl_TreeFree(void);            // 0x800de1a8

typedef struct GrObj
{
    GOBJ *gobj;                 // 0x000
    GroundKind gr_kind;         // 0x004 - physical ground (Gr_GetCurrentGrKind reads this)
    GrData *gr_data;            // 0x008
    GrCollParam coll_max;       // 0x00C - capacity mirror sized by grColl_Alloc; the fill pass
                                //         grows `coll` up to it, so the difference is allocated
                                //         slack. Anything raising a count must raise this too.
    GrCollParam coll;           // 0x054 - map collision, passed to the query API as gcp
    GrCollParam coll_terrain;   // 0x09C - window into `coll` owned by the terrain model; each
                                //         prop gets its own at yaku_data+0x1C. The moving rebake
                                //         walks these windows while queries read `coll`.
    u8 xe4[0xF4 - 0xE4];
    JOBJ *backdrop_jobj;        // 0x0F4 - distant skybox/horizon mesh attached
                                //         by 3D_CreateStageModel. NULL if the
                                //         stage's ModelSection.backdrop is NULL.
    u8 x0f8[0x104 - 0xF8];
    GrJoint *joint_table;       // 0x104 - per-joint {JOBJ*, JOBJDesc*} array built from
                                //         ModelSection.terrain by grLoadStage; entry 0 =
                                //         terrain root joint. Indexed by Sky_SetupLights;
                                //         walk the tree from [0] to reach every terrain MObj.
    u8 x108[0x168 - 0x108];
    GOBJ *sky_gobj;             // 0x168 - fog/sky GObj built by Sky_InitFog.
                                //         hsd_object (+0x28) = HSD_Fog *,
                                //         userdata (+0x2C) = SkyState *.
    u8 x16c[0x6FC - 0x16C];
    int yaku_num;               // 0x6FC - live yakumono GObjs, incremented per GrYaku_Create
    void *coll_tree;            // 0x700 - static KD-tree, = *(GrData+0x48); built by grColl_TreeInit
    u8 x704[0x710 - 0x704];
    YakumonoData **yaku;        // 0x710 - yakumono in spawn order, yaku_num entries
    u32 fade_slot_id;           // 0x714 - lbfade slot ID owned by the sky
                                //         system (incremented per stage entry,
                                //         so it doubles as a freshness signal).
    AreaLight *area_light;      // 0x718 - KAR-proprietary directional light.
} GrObj;

static GrData **stc_grdatalookup = (GrData **)(0x80557638); // indexed by physical GroundKind
static GrObj **stc_grobj = (GrObj **)(0x805dd0e0 + 0x5ec);

// The accessors below all read *stc_grobj and return NULL/0 between scenes.

// The stage's placed-instance records. Every break-family prop binds one visible
// instance to one record, so this pool reaches every breakable prop regardless
// of family; terrain records have a NULL yaku_gobj.
static inline GrCollRecord *Gr_GetCollRecords(int *out_num)
{
    GrObj *gr = *stc_grobj;
    if (gr == (GrObj *)0)
    {
        if (out_num) *out_num = 0;
        return (GrCollRecord *)0;
    }
    if (out_num) *out_num = gr->coll.record_num;
    return gr->coll.record;
}

// Base of the global triangle array. collideWithObject's tri_idx indexes it, so
// a record's own base index is (record->tri_begin - Gr_GetCollTris()).
static inline GrCollTri *Gr_GetCollTris(void)
{
    GrObj *gr = *stc_grobj;
    return gr ? gr->coll.tri : (GrCollTri *)0;
}

// The gcp the query API and collideWithObject take.
static inline GrCollParam *Gr_GetCollParam(void)
{
    GrObj *gr = *stc_grobj;
    return gr ? &gr->coll : (GrCollParam *)0;
}

// Terrain joint by registry index. The table carries no count, so an index past
// the stage's joints reads garbage - validate the result before writing to it.
static inline JOBJ *Gr_GetJoint(int idx)
{
    GrObj *gr = *stc_grobj;
    if (gr == (GrObj *)0 || gr->joint_table == (GrJoint *)0 || idx < 0)
        return (JOBJ *)0;
    return gr->joint_table[idx].jobj;
}

// Moving sweep + tree walk over a segment, moving pass always.
int Raycast_Do(GrCollParam *gcp, Vec3 *start, Vec3 *end, int kind_mask, int filter, Vec3 *out_pos); // 0x800d9958

void grColl_Alloc(GrObj *gr);                                 // 0x800d6dcc - sizes every array from coll_max
void grColl_CountArrays(GrCollParam *max, void *node);        // 0x800d6290 - accumulates one node's counts into coll_max
void grColl_Free(GrObj *gr);                                  // 0x800d7060

StageKind Gm_GetCurrentStageKind();  // 0x800092d8 - reads GameData.stage_kind (+0xA97)
StageKind stGetCurrentStageKind();   // 0x80261ECC - reads the r13[0x7F8] StageKind cache (same value)
GroundKind Gr_GetCurrentGrKind();    // 0x800d1d3c - reads (*stc_grobj)->gr_kind (+0x04): the physical ground
GroundKind Gm_GetGrKindFromStageKind(StageKind stage_kind); // 0x80261ce8 - StageKind -> physical GroundKind

// Minimum signed distance from a world position to any of the six
// StageNode.oob_min/oob_max planes; positive = inside, negative = past a wall.
float calcDistanceFromOOB(Vec3 *pos);          // 0x800d4f20

// Props the current stage places for a yakumono descriptor id (the id that also
// indexes PlayerStats.yakumono_break). Only meaningful while a stage is loaded;
// returns 0 for grounds with no spawn-count table (only GR_CITY1 and GR_SANDS2
// have one). City Trial: desc 33 = 10, 34 = 53, 35 = 41, 38 = 30.
int Gr_GetYakumonoSpawnTotal(int desc_id);     // 0x800f7db0

// 0x48-byte preset entry in the stage file's sky-block array, interpolating fog,
// screen tint, sky ambient color and AreaLight params over transition_frames.
// Color fields are packed RGBA8888 u32 with the high byte red.
typedef struct SkyPresetEntry
{
    s32 transition_frames;     // 0x00 blend duration in frames
    u32 fog_color;             // 0x04 RGBA fog/background color (interpolated)
    float fog_start;           // 0x08 fog near distance (interpolated)
    float fog_end;             // 0x0C fog far distance (interpolated)
    u32 fade_color;            // 0x10 RGBA lbfade screen tint (transitions only)
    u32 sky_ambient_color;     // 0x14 RGBA ambient sky color (interpolated)
    AreaLightData area_light;  // 0x18 directional light params (0x2C bytes)
    u8 light_vis_flag;         // 0x44 bit 0 -> AreaLight registry +0x38 bit 0x80
    u8 x45[3];                // 0x45
} SkyPresetEntry;
_Static_assert(sizeof(SkyPresetEntry) == 0x48, "SkyPresetEntry must be 0x48 bytes");

// Runtime sky state owned by the fog/sky GObj at GrObj+0x168, reachable via
// grobj->gobj[+0x168]+0x2C: the target preset, transition counter, and the
// lerp-start value for each interpolated field.
typedef struct SkyState
{
    SkyPresetEntry *target_preset;  // 0x00 current target preset
    s32 transition_frame_counter;   // 0x04 0 -> target.transition_frames
    u32 start_fog_color;            // 0x08 RGBA lerp start; reused as Sky_Update output mirror
    float start_fog_start;          // 0x0C
    float start_fog_end;            // 0x10
    u32 start_sky_color;            // 0x14 RGBA
    u32 current_output_sky_color;   // 0x18 RGBA written every frame (consumer unknown)
    s32 current_preset_index;       // 0x1C
    AreaLightData start_area_light; // 0x20 lerp start for AreaLight_Lerp (0x2C bytes)
} SkyState;
_Static_assert(sizeof(SkyState) == 0x4C, "SkyState must be 0x4C bytes");

void Sky_Init(GrObj *grobj);                              // 0x8010f114 - initial sky setup per stage
void Sky_SetPresetIndex(GrObj *grobj, int preset_index);  // 0x800dc630 - store preset index in sky state +0x1C
void Sky_LoadPreset(GrObj *grobj);                        // 0x800dc1b4 - load preset immediately (no transition)
void Sky_BeginTransition(GrObj *grobj, int preset_index); // 0x800dc354 - smooth transition to preset
void Sky_ApplyStoredIndex(GrObj *grobj);                  // 0x800dc4c0 - transition to stored index (+0x1C)
void Sky_Update(GrObj *grobj);                            // 0x800dc640 - per-frame interpolation
int Sky_GetPresetCount(void);                             // 0x800d5414 - total preset count from stage data
void Sky_TransitionGlobal(int preset_index);              // 0x800d5444 - transition via stc_grobj
void Sky_RestoreGlobal(void);                             // 0x800d546c - restore default sky preset
void Sky_SetupLights(GrObj *grobj, int jobj_idx);         // 0x800db774 - toggle JOBJ visibility flags (no LOBJ creation)
void Sky_InitFog(GrObj *grobj);                           // 0x800dbfa8 - build the fog GObj at grobj+0x168
void Sky_AllocFade(GrObj *grobj);                         // 0x800eef04 - alloc lbfade slot 3 at grobj+0x714
void Sky_BeginFade(GrObj *grobj, u32 *color, int frames); // 0x800eef50 - fire screen tint via ScreenFade_Begin
void Sky_FreeFade(GrObj *grobj);                          // 0x800eefb0 - free lbfade slot at scene teardown
int Gm_Roll(int *weights, int count);                     // 0x800db2b8 - weighted random selection

// Global EFB/erase color (RGBA8888, in BSS), written each frame by Sky_Update
// and read by World_CObj+0x144 to drive HSD_SetEraseColor for the next CopyDisp.
static u32 *stc_global_fog_color = (u32 *)0x80557484;

// First INFINITE non-HIDDEN LOBJ in the primary stage-light chain, cached at
// stage init. For City Trial this is the warm-white directional light at
// (-1000, 700, 1500) that lights terrain; write its color (+0x10) / hw_color
// (+0x14) to re-tint terrain at runtime. NULL outside CT-style stages.
static struct LOBJ **stc_main_light = (struct LOBJ **)(0x805dd0e0 + 0x5fc);

// Spline path system - enemy walking paths embedded in stage data, headed at
// GrObj+0x11C as {void *entries, int count}. Each 0x18-stride entry holds
// forward/backward HSD spline pointers. The accessors below read stc_grobj.
int Spline_GetCount(void);                               // 0x800cf38c - number of spline segments in current stage
void *Spline_GetForward(int segment);                    // 0x800cf3ac - forward HSD spline pointer for segment
void *Spline_GetBackward(int segment);                   // 0x800cf44c - backward HSD spline pointer for segment

// HSD spline evaluation. param runs [0.0, 1.0] from first to last control
// point. The spline struct has u8 type at +0x0 and s16 num_points at +0x2.

AudioEmitter Map_AllocAudioEmitter(int index);
#endif