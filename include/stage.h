#ifndef KAR_H_STAGE
#define KAR_H_STAGE

#include "datatypes.h"
#include "obj.h"
#include "hurt.h"
#include "collision.h"
#include "yakumono.h"
#include "audio.h"

typedef struct EventConfigData EventConfigData;
typedef struct GrCollisionNode GrCollisionNode;
typedef struct GrObj GrObj;

#define GRSTATECHANGE_NOANIM (1 << 2)

// The 0..58 stage selection index - a stage's menu/mode-level identity, stored
// in GameData.stage_kind (+0xA97) and returned by Gm_GetCurrentStageKind. For Air
// Ride it equals AirRideCourse; City Trial is 9 and its stadiums occupy 10..33.
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
    STAGEKIND_NUM = 59, // St_Kind_Terminate
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

// Top Ride course indices (0-6), stored in GameData.topride_selected_course
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
    void *shapeanim_joint;
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
    JOBJDesc **backdrop; // 0x04 - secondary skybox/horizon mesh: [0] root JOBJDesc, [1] its GrModelMotionAnim array
    void *unk_8;         // 0x08
    void *unk_c;         // 0x0C
} ModelSection;

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

// One gravity zone's falloff, indexed like its point or spline. Inside radius the pull
// is lerp(strength_center, strength_edge, dist / radius) along (pos - zone point); a
// negative result flips the direction, so negative strengths attract.
typedef struct GrGravityZoneParam
{
    float radius;          // 0x00
    float strength_center; // 0x04
    float strength_edge;   // 0x08
} GrGravityZoneParam;

// The GrData+0x04 sub-block (HSDLib KAR_grStageNode): the stage's physics
// constants, global gravity, and the axis-aligned out-of-bounds death box that
// calcDistanceFromOOB reads every frame.
//
// Gravity splits into a magnitude (gravity_strength) and a unit down direction
// (gravity_dir), both returned by Gr_GetDownVector. To change how strong gravity
// feels, scale the strength and leave the direction unit-length - consumers
// derive an up vector from it.
typedef struct StageNode
{
    int x0;                 // 0x00
    float machine_accel;    // 0x04 - base machine acceleration scalar
    float scale;            // 0x08 - stage model scale (applied to stage JObjs)
    float gravity_strength; // 0x0C - global gravity magnitude / fall-accel scalar
                            //        (City Trial = 0.025); returned by Gr_GetDownVector
    Vec3 gravity_dir;       // 0x10 - global down DIRECTION, unit (0,-1,0); written
                            //        into Gr_GetDownVector's out-param. Keep unit.
    int fog_flags;          // 0x1C
    u8 x20[0x60 - 0x20];
    float minimap_scale;    // 0x60
    u8 x64[0x70 - 0x64];
    GrGravityZoneParam *gravity_point_params;  // 0x70 - per Gravitypos point; NULL on every shipped stage
    GrGravityZoneParam *gravity_spline_params; // 0x74 - per gravity spline; NULL on every shipped stage
    u8 x78[0x80 - 0x78];
    int node_flags;         // 0x80
    u8 x84[0xCC - 0x84];
    Vec3 oob_min;           // 0xCC - out-of-bounds box minimum corner (X,Y,Z)
    Vec3 oob_max;           // 0xD8 - out-of-bounds box maximum corner (X,Y,Z)
} StageNode;

// One row of a stadium item pool. The chance column is picked once per round by
// HSD_Randi(3) in CityItemSpawn_Init, so a stage can offer three alternative
// weightings of the same kinds - identical columns mean the pool never varies.
typedef struct GrItemPoolEntry
{
    int it_kind;    // 0x00, ItemKind
    int chance[3];  // 0x04, one column per round
} GrItemPoolEntry;

typedef struct GrItemPool
{
    void *box_spawn_chances;   // 0x00
    GrItemPoolEntry *entries;  // 0x04
    int entry_num;             // 0x08
} GrItemPool;

// GrData.item is an array of these indexed by the stage's ItemposId
// (stGetCurrentStageKind_ItemposId, Stage.dat row +0x24), so two StageKinds
// sharing a ground file get separate pools: GrCity1 carries the open city at 0,
// Destruction Derby 4 at 1 and Derby 5 at 2, and GrColosseum1 carries Derby 1 at
// 0 and Vs. King Dedede at 1. Only GrCity1, GrColosseum1, GrColosseum3 and
// GrDedede1 have one at all - every other ground, Air Ride courses included,
// leaves GrData.item NULL and runs no item spawner.
//
// CityItemSpawn_Init takes the first non-NULL of item_desc, pool_a, pool_b.
// item_desc is the full City Trial table (box chances, event drops, legendary
// pieces) and appears only on GrCity1 entry 0; the derbies use pool_a and
// Vs. King Dedede pool_b, and the two pool slots are read by identical code.
typedef struct GrItemNode
{
    int kind;            // 0x00, 0 city / 1 pool_a / 2 pool_b
    void *fall_timer;    // 0x04, {void *, ItemFallDesc *, int} spawn-timing table
    void *item_desc;     // 0x08, grBoxGeneInfo.item_desc
    GrItemPool *pool_a;  // 0x0c
    GrItemPool *pool_b;  // 0x10
} GrItemNode;

typedef struct GrSplineList
{
    HSD_Spline **splines; // 0x00
    int num;              // 0x04
} GrSplineList;

typedef struct GrCourseSetup
{
    GrSplineList *pieces; // 0x00, linear and one-way; City Trial's is one closed loop
    void *route;          // 0x04, {int *piece_idx, int num}: the pieces one lap runs
    void *branch;         // 0x08, each piece's branch chain
    void *branch_main;    // 0x0c, the main-route pieces a branch stands in for
    u8 loop;              // 0x10
} GrCourseSetup;

// GrData+0x1C. Every spline in it is scaled by the stage scale at load, so its points
// and lengths are world units.
typedef struct GrSplineNode
{
    GrCourseSetup *course;   // 0x00
    void *range;             // 0x04, {pairs, count, city_param}: the CPU road edges
    void *x08;               // 0x08
    void *x0c;               // 0x0c
    GrSplineList *conveyor;  // 0x10
    GrSplineList *rail;      // 0x14
    GrSplineList *heavy;     // 0x18
} GrSplineNode;

// A stage's AnimJoints of one sub-animation family.
typedef struct GrSubAnim
{
    AnimJointDesc **anims; // 0x00
    int anim_num;          // 0x04
} GrSubAnim;

// GrData+0x24. A slot is NULL on a stage without that family.
typedef struct GrSubAnimNode
{
    GrSubAnim *super_jump; // 0x00
    GrSubAnim *leap;       // 0x04
    GrSubAnim *rail;       // 0x08
    GrSubAnim *cannon;     // 0x0c - flight paths, by YakuCannonDesc.path_idx
    GrSubAnim *x10;        // 0x10
    GrSubAnim *event;      // 0x14
} GrSubAnimNode;

typedef struct GrData // exists in the stage file
{                     //
    int flags;                      // 0x00
    StageNode *stage_node;          // 0x04 - physics + gravity + OoB death box
    int x8;                         // 0x08
    ModelSection *model_section;    // 0x0c - terrain + backdrop JObj descs
    GrModelMotion *motion;          // 0x10, pointer placed at runtime
    void *lights;          // 0x14 - three NULL-terminated LightGroup* chains: +0x00 stage, +0x04 rider, +0x08 secondary
    GrCollisionNode *coll_node; // 0x18 - baked map collision, sized into GrObj.coll_max by grColl_CountArrays
    GrSplineNode *spline_node; // 0x1c
    void *pos_node;        // 0x20 - placed positions: +0x08 enemy positions by EnemyposId (0xC stride),
                           //        +0x2c yakumono position records (count at [+0x2c]->[+0x8])
    GrSubAnimNode *subanim; // 0x24
    int x28;               // 0x28
    GrItemNode *item;      // 0x2c - item-spawn tables, one entry per ItemposId
    EventConfigData *event_config; // 0x30 - set on entering City Trial, whatever the events on/off
                           //        setting. Also stored as EventCheckData.data when events are on.
    SkyBlock *sky_block;   // 0x34 - sky/fog descriptor pair: [0] HSD_FogDesc, [1] preset sub-header (array base + count)
    int x38;               // 0x38
    int x3c;               // 0x3c
    YakumonoTable *yakumono; // 0x40 - per-stage yakumono manifest
    int x44;               // 0x44
    void *coll_tree_node;  // 0x48 - [0] is the static KD-tree grColl_TreeInit installs as GrObj.coll_tree
} GrData;        //

// The "CZK" tag carried by every collision-zone box face, packed into
// GrCollFace.kind_word. Dash*, Warp*, SuperJump, Jump and Spin are the names
// grlib.c asserts on; the rest are named after the Japanese texture label
// GrSimple paints on the matching zone box. Every consumer is an explicit
// compare, so a kind no compare mentions can never fire.
typedef enum GrCollZoneKind
{
    GrCZK_DashA       = 0,  // dash panel; grGetDashZoneParam
    GrCZK_DashB       = 1,
    GrCZK_DashGateA   = 2,  // grGetDashGateZoneParam, GrDashGate_Num = 2
    GrCZK_DashGateB   = 3,
    GrCZK_DashRing    = 4,  // grGetDashGateZoneParam, GrDashRing_Num = 2
    GrCZK_WarpIn      = 5,  // unimplemented
    GrCZK_WarpOut     = 6,  // unimplemented
    GrCZK_SuperJump   = 7,  // grGetSuperJumpZoneParam
    GrCZK_SuperJumpApproach = 8,
    GrCZK_Jump        = 9,  // grGetJumpZoneParam
    GrCZK_Spin        = 10, // grGetSpinZoneParam, GrSpinZone_Num = 2
    GrCZK_RandomAbility = 15, // ground copy panel; zz_80246f40_ -> RiderGObj_GiveRandomAbility
    GrCZK_FreeMove    = 16,
    GrCZK_Catch       = 18, // owned by a catch zone yakumono; mpColl_GetCatchZoneIndex
    GrCZK_Cannon      = 22, // camera hint only; mpColl_GetCannonZoneIndex
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

// GrData.coll_node (+0x18) points here - a paired {pointer, count} mirror of the
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
    Vec3 *zone_vtx;      // 0x18 - feeds GrCollParam.zone_vtx; always zone_num * 8
    int zone_vtx_num;    // 0x1c
    GrCollFace *faces;   // 0x20 - 0x18 stride; always zone_num * 12
    int face_num;        // 0x24
    GrCollZone *zones;   // 0x28 - 0x4C stride
    int zone_num;        // 0x2c - asserted < Gr_CollZone_NumMax (500)
} GrCollisionNode;

// Per-kind zone-parameter getters, taking an index into GrObj.coll.zone. Each
// asserts the zone it is handed carries its kind, then fills the caller's
// out-params from the per-kind param block.
void grGetDashZoneParam(int zone, float *out1, void *out2, void *out3, void *out4);          // 0x800d1ff0
// Kinds 2, 3 and 4: the zone's unit +Z (zero for kind 2: the machine's forward), then the
// stage-scaled GrCommon dash row (City Trial's ring: accel 0.12, impulse 0.05, 40 frames).
void grGetDashGateZoneParam(int zone, Vec3 *dir, float *accel, float *impulse, int *frames); // 0x800d21f8
int Gr_IsDashRingZone(int zone);                                                             // 0x800d21d0 - kind 4
int Gr_IsDashZone(int zone);                                                                 // 0x800d1f64 - kind 0 or 1
int Gr_IsDashGateZone(int zone);                                                             // 0x800d2190 - kind 2 or 3
int Gr_IsSuperJumpZone(int zone);                                                            // 0x800d1fa0 - kind 7
int Gr_IsJumpZone(int zone);                                                                 // 0x800d1fc8 - kind 9
int Gr_IsRandomAbilityZone(int zone);                                                        // 0x800d24d4 - kind 15
// Kinds 12 and 13 belong to a yakumono: its YakuKind (-1 with none), and a per-kind flag
// only a few kinds answer (0 for the rest). Both return 0 for any other zone kind.
int Gr_GetYakuZoneKind(int zone, int *yaku_kind);                                            // 0x800d2a1c
int Gr_GetYakuZoneFlag(int zone, int *flag);                                                 // 0x800d2afc
int Gr_GetZoneNum(void);                                                                     // 0x800d55e4 - GrObj.coll.zone_num
void Gr_GetZonePos(int zone, Vec3 *out);                                                     // 0x800d518c - the zone record's matrix translation
// GrObj.coll.moving_zone, as zone indices.
int Gr_GetMovingZoneNum(void);                                                               // 0x800d51bc
int Gr_GetMovingZone(int i);                                                                 // 0x800d51c8
// Zone walk over GrObj.coll_tree with an LbKdColl query: each NextZone returns a new zone
// index, -1 when done.
void grColl_TreeQueryReset(void *query);                                                     // 0x800de2d4
int  grColl_TreeQueryNextZone(void *query);                                                  // 0x800de2fc

// Target Flight's targets by index; assert on any other stage.
int Gr_GetTargetCount(void);              // 0x800d57e8
int Gr_GetTargetScore(int id);            // 0x800d5840
void Gr_GetTargetPos(int id, Vec3 *out);  // 0x800d58f4
void grGetSuperJumpZoneParam(int zone, int *out1, int *out2, int *out3);                    // 0x800d24fc - kind 7; out3 may be NULL
void grGetJumpZoneParam(int zone, int *out1, int *out2, int *out3);                         // 0x800d25a8 - kind 9; out3 may be NULL
int grGetSpinZoneParam(int zone);                                                            // 0x800d2654 - kind 10; returns param_index != 0
void grGetLocalDeadZoneParam(int zone, void *out1, void *out2, void *out3);                 // 0x800d50f8 - kind 25, via grGetLocalDeadPosition
void grZone_BuildRecord(int arg0, int arg1, GrCollParam *gcp, GrCollisionNode *node, GrJoint **joints, int zone); // 0x800dcf08 - GrCollZone -> 0x140 runtime record

// Collision build: a node's triangles become records in a window carved from coll at its
// current counts, transformed by the joint world matrices. grColl_Alloc builds the terrain
// into coll_terrain (call 0x800d7044); GrYaku_AttachModel builds each prop, mid-round too.
void grColl_Build(GrCollParam *coll, GrCollParam *max, GrCollParam *window, GrCollisionNode *node,
                  GrJoint **joints);                 // 0x800d6d88 - SetupNode then BakeRecords
void grColl_SetupNode(GrCollParam *coll, GrCollParam *max, GrCollParam *window, GrCollisionNode *node,
                      GrJoint **joints);             // 0x800d6774
// Sets state COLLIDABLE | SURFACE_PARAM, and lists the record in window->moving_record
// when its first triangle carries GRCOLL_FLAG_MOVING.
void grColl_BuildRecord(GrCollParam *coll, GrCollParam *max, GrCollParam *window, GrCollisionNode *node,
                        GrJoint **joints, int idx);  // 0x800d64b8
// Per record: world, vertices, normals and boxes, with prev = pos and prev_inv = inverse(world).
void grColl_BakeRecords(GrCollParam *window, GrCollisionNode *node); // 0x800d6a70
void grColl_ClearWindow(GrCollParam *window); // 0x800d6724 - a prop without collision
void grRough_AddTri(GrCollParam *coll, GrCollParam *max, GrCollParam *window, GrCollisionNode *node,
                    int tri);                        // 0x800dc7e4
// Per-frame rebake of window->moving_record only. An unchanged record gets prev = pos and
// prev_inv = inverse(world) once, so carry stops.
void grColl_RebakeMovingRecord(GrCollParam *window, GrCollisionNode *node, int idx); // 0x800d7268
void grColl_RebakeWindow(GrCollParam *window, GrCollisionNode *node); // 0x800d7648 - records, zones, rough
void grColl_RebakeTerrain(GrObj *gr); // 0x800d777c - coll_terrain, from Gr_PhysicsThink
void grColl_SegmentBounds(Vec3 *a, Vec3 *b, float *out_center_half); // 0x8024ac68
void grJObj_GetScaledTranslate(JOBJ *jobj, Vec3 *out); // 0x800d2fb4 - translate * grGetStageScale()
void grJointTable_Init(GrObj *gr);           // 0x800d8b98 - builds GrObj.joint_table

// Narrowphase primitives, both taking a triangle index into GrCollParam.tri and
// applying the kind_mask and state gates. Both are one-sided and return 1 on a hit.
int grColl_RayVsTri(Vec3 *start, Vec3 *dir, void *bounds, GrCollParam *gcp, int tri, int kind_mask, int filter,
                    float *out_t, Vec3 *out_pos, float max_t); // 0x800d95dc
int grColl_SweptSphereVsTri(Vec3 *start, Vec3 *delta, Vec3 *dir, void *bounds, GrCollParam *gcp, int tri,
                            int kind_mask, float *out_t, Vec3 *out_hit, Vec3 *out_sphere_pt, int *out_edge,
                            float *out_dist, float radius, float radius_end, float len); // 0x802448b0
int grColl_SweptSphereQuery(GrCollParam *gcp, Vec3 *start, Vec3 *end, int kind_mask, int moving, Vec3 *out_pos, int arg6,
                            float f1, float f2); // 0x800d9e34 - moving sweep + tree walk, moving pass only when moving != 0. Returns the hit tri or -1

// Rider and machine wall/floor pushback, under mpColl_UpdateCollision
// (0x802485e0). The sweep is what a machine is actually stopped by.
void mpColl_SweptSphereMapColl(void *coll, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6); // 0x802454f8 - swept sphere vs map tris, moving pass always
void mpColl_InsertContact(void *contacts, int slot, GrCollParam *gcp, int tri, Vec3 *arg4, Vec3 *arg5,
                          int arg6, int arg7, float f1, float f2); // 0x80241ca8 - caches the winning tri id in the floor/wall/ceiling slot
void grColl_TreeInit(GrObj *gr); // 0x800de0e8 - fills GrObj.coll_tree from GrData.coll_tree_node; adds objKinds 3-5
void grColl_TreeFree(GrObj *gr); // 0x800de1a8

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
                                //         by CreateStageModel_3D. NULL if the
                                //         stage's ModelSection.backdrop is NULL.
    u8 x0f8[0x104 - 0xF8];
    GrJoint *joint_table;       // 0x104 - per-joint {JOBJ*, JOBJDesc*} array built from
                                //         ModelSection.terrain by grLoadStage; entry 0 =
                                //         terrain root joint. Indexed by Sky_SetupLights;
                                //         walk the tree from [0] to reach every terrain MObj.
    u8 x108[0x110 - 0x108];
    GrCourseSetup *course;      // 0x110 - the course pieces Gr_GetCourseSpline indexes
    u8 x114[0x11C - 0x114];
    void *spline;               // 0x11C - -> {entries, count} of CPU range spline pairs
    void *range_state;          // 0x120 - 0x1c bytes per range; +0x18 bit 7 = enabled
    void *junctions;            // 0x124 - 0x2c bytes per junction: up to 10 {range, end} pointers, count at +0x28
    int junction_num;           // 0x128
    u8 x12c[0x13C - 0x12C];
    void *gravity_pos;          // 0x13C - 0x24 bytes per Gravitypos point, position first
    u8 x140[0x168 - 0x140];
    GOBJ *sky_gobj;             // 0x168 - fog/sky GObj built by Sky_InitFog.
                                //         hsd_object (+0x28) = HSD_Fog *,
                                //         userdata (+0x2C) = SkyState *.
    u8 x16c[0x6FC - 0x16C];
    int yaku_num;               // 0x6FC - live yakumono GObjs, incremented per GrYaku_Create
    void *coll_tree;            // 0x700 - static KD-tree, = *(GrData+0x48); built by grColl_TreeInit
    u8 x704[0x710 - 0x704];
    YakumonoData **yaku;        // 0x710 - one per YakumonoTable.entries, not yaku_num; NULL in City Trial
    u32 fade_slot_id;           // 0x714 - lbfade slot ID owned by the sky
                                //         system (incremented per stage entry,
                                //         so it doubles as a freshness signal).
    AreaLight *area_light;      // 0x718 - KAR-proprietary directional light.
    u8 x71c[0x870 - 0x71C];
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

// Base of the global triangle array, which collideWithObject's tri_idx indexes.
static inline GrCollTri *Gr_GetCollTris(void)
{
    GrObj *gr = *stc_grobj;
    return gr ? gr->coll.tri : (GrCollTri *)0;
}

// Global index of a record's first triangle; its triangles are a contiguous slice
// of the array. -1 for a record with no slice.
static inline int Gr_GetRecordTriIndex(GrCollRecord *record)
{
    GrCollTri *base = Gr_GetCollTris();
    if (base == (GrCollTri *)0 || record->tri_begin < base)
        return -1;
    return record->tri_begin - base;
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

// A terrain joint's world translation and Y and Z axes, refreshing a dirty matrix
// first. Assert on a joint the stage never built.
void Gr_GetNodeWorldPos(int joint, Vec3 *out);   // 0x800d4bf4
void Gr_GetNodeWorldAxisY(int joint, Vec3 *out); // 0x800d4d58 - not normalized
void Gr_GetNodeWorldAxisZ(int joint, Vec3 *out); // 0x800d4ca0 - not normalized
// Machines, riders and items are drawn at this scale; stage geometry is modelled in
// units of 1 / it, and a stage path moves a machine by its translate times it.
float grGetStageScale(void); // 0x800d3058

// grlib wrappers over the adhering yakumono (gondolas and cannons).
GOBJ *Gr_GetCatchZoneTarget(int zone, GOBJ *gobj); // 0x800d2790 - GrYakuCatchZone_GetTarget
// 1 for a cannon; a gondola asks the gondola whether gobj may board.
int Gr_CanAdhereYaku(GOBJ *yaku_gobj, GOBJ *gobj);  // 0x800d27b0
// Pose a machine snaps to; asserts on any kind but a gondola or cannon.
void Gr_GetAdhereYakuPose(GOBJ *yaku_gobj, Vec3 *pos, Vec3 *fwd, Vec3 *up); // 0x800d2828
void Gr_GetCannonPath(GOBJ *yaku_gobj, void **out); // 0x800d297c - asserts YAKUKIND_CANNON

// Moving sweep + tree walk over a segment, moving pass always.
int Raycast_Do(GrCollParam *gcp, Vec3 *start, Vec3 *end, int kind_mask, int filter, Vec3 *out_pos); // 0x800d9958

// Sizes every array from coll_max. The terrain is counted by the call at 0x800d6e4c;
// 0x800d6f4c is the last point to raise a count before the allocations.
void grColl_Alloc(GrObj *gr);                                 // 0x800d6dcc
// Accumulates one node's counts into coll_max, including one moving_record slot per
// GRCOLL_FLAG_MOVING record. City Trial totals 8 moving_record slots (6 terrain, 2 event
// props), none spare.
void grColl_CountArrays(GrCollParam *max, GrCollisionNode *node); // 0x800d6290
void grColl_Free(GrObj *gr);                                  // 0x800d7060

StageKind Gm_GetCurrentStageKind();  // 0x800092d8 - reads GameData.stage_kind (+0xA97)
StageKind stGetCurrentStageKind();   // 0x80261ECC - reads the r13[0x7F8] StageKind cache (same value)
GroundKind Gr_GetCurrentGrKind();    // 0x800d1d3c - reads (*stc_grobj)->gr_kind (+0x04): the physical ground
GroundKind Gm_GetGrKindFromStageKind(StageKind stage_kind); // 0x80261ce8 - StageKind -> physical GroundKind
// Magma Flows' 12 grounded Boost Panels as a zone-index bitset laid out like
// PlayerStats.zone_bits, which Ply_GetAllBoostPanelsUsedFlag ANDs against it, or
// NULL off GR_HEAT2.
u8 *GrMagma_GetBoostPanelRefMask(void); // 0x800d5740
int stGetCurrentStageKind_ItemposId(); // 0x802623e8 - Stage.dat row +0x24, the GrData.item index
// Returns GrData.item[ItemposId], or NULL when the stage has no item node or
// Gm_IsMapDebug() holds.
GrItemNode *fn_grGetItemData(GrObj *gr); // 0x800da518

// Minimum signed distance from a world position to any of the six
// StageNode.oob_min/oob_max planes; positive = inside, negative = past a wall.
float calcDistanceFromOOB(Vec3 *pos);          // 0x800d4f20

// Props the current stage places for a yakumono descriptor id (the id that also
// indexes PlayerStats.yakumono_break). Only meaningful while a stage is loaded;
// returns 0 for grounds with no spawn-count table (only GR_CITY1 and GR_DESERT1
// have one). City Trial: desc 33 = 10, 34 = 53, 35 = 41, 38 = 30.
int Gr_GetYakumonoSpawnTotal(YakuKind kind);   // 0x800f7db0

// City Trial's sky presets, by index into GrCity1.dat's preset array. Sky_Init rolls
// among Day, Foggy, Dusk and Night; Gray Sky and Dark Purple are the Dragoon and Hydra
// skies, and the last two belong to the Meteor and Lighthouse events.
typedef enum CitySkyPreset
{
    CITYSKY_DAY,
    CITYSKY_MIDNIGHT,
    CITYSKY_LIGHT_FOG,
    CITYSKY_DUSK_2,
    CITYSKY_DUSKY_CLOUDS,
    CITYSKY_DARK_VIGNETTE,
    CITYSKY_DAY_2,
    CITYSKY_BLUE_SKY,
    CITYSKY_PINK_SKY,
    CITYSKY_DENSE_FOG,
    CITYSKY_FOGGY,
    CITYSKY_DUSK,
    CITYSKY_NIGHT,
    CITYSKY_GRAY_SKY,
    CITYSKY_DARK_PURPLE,
    CITYSKY_RED_VIGNETTE,
    CITYSKY_DARK_LOW_VIS,
    CITYSKY_METEOR,
    CITYSKY_LIGHTHOUSE,
    CITYSKY_NUM,
} CitySkyPreset;

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
    u32 current_output_sky_color;   // 0x18 RGBA written every frame; Sky_DrawTintQuad's (0x800d7e78) tint
    s32 current_preset_index;       // 0x1C
    AreaLightData start_area_light; // 0x20 lerp start for AreaLight_Lerp (0x2C bytes)
} SkyState;

void Sky_Init(GOBJ *stage_gobj);                         // 0x8010f114 - initial sky setup per stage
void Sky_SetPresetIndex(GrObj *grobj, int preset_index);  // 0x800dc630 - store preset index in sky state +0x1C
void Sky_LoadPreset(GrObj *grobj);                        // 0x800dc1b4 - load preset immediately (no transition)
void Sky_BeginTransition(GrObj *grobj, int preset_index); // 0x800dc354 - smooth transition to preset
void Sky_ApplyStoredIndex(GrObj *grobj);                  // 0x800dc4c0 - transition to stored index (+0x1C)
void Sky_Update(GrObj *grobj);                            // 0x800dc640 - per-frame interpolation
int Sky_GetPresetCount(void);                             // 0x800d5414 - total preset count from stage data
void Sky_TransitionGlobal(int preset_index);              // 0x800d5444 - transition via stc_grobj
void Sky_RestoreGlobal(void);                             // 0x800d546c - restore default sky preset
// Writes the sky fog's HSD_Fog.scale; World_CObj overwrites it every frame from the eye's
// angle to gravity.
void Sky_SetFogScale(float scale);                        // 0x800d1e20
void Sky_SetupLights(GrObj *grobj, int jobj_idx);         // 0x800db774 - toggle JOBJ visibility flags (no LOBJ creation)
void Sky_InitFog(GrObj *grobj);                           // 0x800dbfa8 - build the fog GObj at grobj+0x168
void Sky_AllocFade(GrObj *grobj);                         // 0x800eef04 - alloc lbfade slot 3 at grobj+0x714
void Sky_BeginFade(GrObj *grobj, u32 *color, int frames); // 0x800eef50 - fire screen tint via ScreenFade_Begin
void Sky_FreeFade(GrObj *grobj);                          // 0x800eefb0 - free lbfade slot at scene teardown
int Gm_Roll(int *weights, int count);                     // 0x800db2b8 - weighted random selection

// Think proc of the stage GObj: steps stage anims, animates GrObj.backdrop_jobj,
// runs Sky_Update, then dispatches the per-GroundKind think through the vtable
// table at 0x804a322c. Takes the stage GObj, not the GrObj.
void Gr_Think(GOBJ *stage_gobj);                          // 0x800ce618

// Instantiates grdata->model_section: terrain onto the stage GObj, backdrop into
// GrObj.backdrop_jobj (+0xF4) with no GObj of its own. Stamps grGetStageScale()
// over all three scale components of each instantiated root joint, discarding
// whatever scale the loaded JOBJDesc carried.
void CreateStageModel_3D(GOBJ *stage_gobj);               // 0x800dcbf0
// Bind motion `index` to the terrain (GrData.motion) or the backdrop (ModelSection.backdrop[1]),
// resetting the joints and looping the anims. grLoadStage binds index 0 to both.
void Gr_SetTerrainAnim(GOBJ *stage_gobj, int index);     // 0x800db8d8
void Gr_SetBackdropAnim(GOBJ *stage_gobj, int index);    // 0x800dbc2c

// Global EFB/erase color (in BSS), written each frame by Sky_Update and read by
// World_CObj+0x144 to drive CObj_SetEraseColor for the next CopyDisp.
static GXColor *stc_global_fog_color = (GXColor *)0x80557484;

// First INFINITE non-HIDDEN LOBJ in the primary stage-light chain, cached at
// stage init. For City Trial this is the warm-white directional light at
// (-1000, 700, 1500) that lights terrain; write its color (+0x10) / hw_color
// (+0x14) to re-tint terrain at runtime. NULL outside CT-style stages.
static struct LOBJ **stc_main_light = (struct LOBJ **)(0x805dd0e0 + 0x5fc);

// CPU road ranges, reached through GrObj.spline as {void *entries, int count}. Each
// 0x18-stride entry pairs the left and right road edges CPUs steer between, about 2
// stage units above the floor. The accessors read stc_grobj.
int Spline_GetCount(void);                               // 0x800cf38c - number of range pairs
HSD_Spline *Spline_GetForward(int segment);              // 0x800cf3ac - the pair's left edge
HSD_Spline *Spline_GetBackward(int segment);             // 0x800cf44c - the pair's right edge
HSD_Spline *Spline_GetCenter(int segment);               // 0x800cf4f0 - the center line
#define SPLINE_JUNCTION_EXIT_MAX 10 // ranges a junction point holds
// The other enabled ranges meeting at the range's start point (end != 0: its end point),
// each with its direction flag, 1 = driven end to start. Returns the count.
int Spline_GetJunctionExits(int segment, int end, int *ranges, int *flags); // 0x800cf1c0
int grRangeSpline_IsEnabled(GrObj *gr, int range);      // 0x800e2c84 - GrObj.range_state bit
// The spline of course piece id, as RiderData.track_spline_id and MachineData.respawn_pos
// name it; asserts on an id out of range.
HSD_Spline *Gr_GetCourseSpline(int id);                 // 0x800cefb4
// Ranges from junction point from to point to, by an all-pairs next-hop table. Returns
// the count, 0 when from == to, -1 when unreachable.
int grRangeSpline_FindPath(int from, int to, int *ranges); // 0x800cfb34
// The u16 next-hop range (0xffff = none) for points a < b in that table.
u16 *grRangeSpline_GetNextHop(void *table, int a, int b); // 0x800e1940

AudioEmitter Map_AllocAudioEmitter(int index); // 0x8005ded0
#endif