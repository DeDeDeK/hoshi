#ifndef KAR_H_STAGE
#define KAR_H_STAGE

#include "datatypes.h"
#include "obj.h"
#include "hurt.h"
#include "collision.h"
#include "yakumono.h"

// Forward declaration; full definition in event.h.
typedef struct EventConfigData EventConfigData;

#define GRSTATECHANGE_NOANIM (1 << 2)

// StageKind - the 0..59 stage *selection* index (the menu/mode-level identity of
// a stage). This is the value stored in GameData.stage_kind (+0xA97) and the
// r13[0x7F8] cache; it is what Gm_GetCurrentStageKind() and stGetCurrentStageKind()
// return. For Air Ride it equals AirRideCourse (menu order, 0..8); the City Trial
// stadium stages occupy 9..33 below. A StageKind maps to a physical GroundKind (the
// actual ground geometry file loaded, in FILE order) via Gm_GetGrKindFromStageKind().
// The two index spaces happen to coincide only at 0/1/2 and City Trial (9); they
// diverge everywhere else because menu order != file order - e.g. Machine Passage is
// StageKind 6 but GroundKind 5 (GrMachine2), and Kirby Melee 1 is StageKind 17 but
// GroundKind 14 (GrPasture1).
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

// GroundKind - the physical ground-file index (which terrain geometry is loaded),
// i.e. an index into the stage-file table in main.dol at 0x804A2FFC. This is the
// value stored in GrObj.gr_kind (+0x04) and returned by Gr_GetCurrentGrKind();
// stc_grdatalookup is indexed by it. DISTINCT from StageKind (see above) - many
// StageKinds can share one GroundKind. Only members present in the file table
// are named (the enum is intentionally sparse). Obtain one from a StageKind
// with Gm_GetGrKindFromStageKind().
typedef enum GroundKind
{
    GR_PLANTS1    = 0,  // Air Ride: Fantasy Meadows (first of the 9 AR course grounds)
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

// ModelSection - 4 dwords pointed at by GrData.model_section. Slots 0
// and 1 are JOBJDesc** (a pointer to a slot containing the JOBJDesc *
// instantiated by 3D_CreateStageModel), so a stage can ship a
// terrain mesh, a separate skybox/backdrop mesh, both, or neither.
// The loader at 0x800dcc28 reads +0x04 and attaches the result to GrObj+0xF4.
typedef struct ModelSection
{
    JOBJDesc **terrain;  // 0x00 - main playable geometry
    JOBJDesc **backdrop; // 0x04 - secondary skybox/horizon mesh
    void *unk_8;         // 0x08
    void *unk_c;         // 0x0C
} ModelSection;

// Forward declaration for the sky-block pointer chain (full SkyPresetEntry
// definition is further down this file). HSD_FogDesc comes from obj.h.
struct SkyPresetEntry;

// SkyPresetSubHeader - the {array, count} pair pointed at by SkyBlock+0x04.
// Sky_GetPresetCount (0x800d5414) returns preset_count; Sky_LoadPreset reads
// preset_array[index] (stride 0x48). Repointing both fields swaps in an
// extended preset array.
typedef struct SkyPresetSubHeader
{
    struct SkyPresetEntry *preset_array; // 0x00 - base of the 0x48-byte preset entries
    s32 preset_count;                    // 0x04 - number of presets in the array
} SkyPresetSubHeader;

// SkyBlock - the GrData+0x34 sub-block from the stage file. Two pointers:
// the initial fog descriptor and the preset sub-header. Reachable as
// grobj->gr_data->sky_block.
typedef struct SkyBlock
{
    HSD_FogDesc *fog_desc;             // 0x00 - initial fog parameters
    SkyPresetSubHeader *preset_header; // 0x04 - sky preset array + count
} SkyBlock;

// StageNode - the GrData+0x04 sub-block (HSDLib KAR_grStageNode). Holds the
// stage's physics constants, the global gravity (strength + direction), and the
// axis-aligned out-of-bounds death box. The OoB box at +0xCC/+0xD8 is read by
// calcDistanceFromOOB (0x800d4f20) every frame to compute how far a position
// is from leaving the playfield.
//
// Gravity splits across two fields: gravity_strength (+0x0C) is the magnitude
// (fall-acceleration scalar) and gravity_dir (+0x10) is the unit down direction.
// Gm_GetDownVector reads gravity_dir into its out-param and returns
// gravity_strength as the scalar - so to change how strong gravity feels, scale
// gravity_strength and leave gravity_dir unit-length (consumers derive an up
// vector from it).
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
    EventConfigData *event_config; // 0x30 - set by fn_grSetupCityEventData (0x8010f7c4) when entering City Trial. Loaded regardless of the events on/off setting; same pointer is also stored as EventCheckData.data when events are enabled.
    SkyBlock *sky_block;   // 0x34 - sky/fog descriptor pair: [0] HSD_FogDesc, [1] preset sub-header (array base + count)
    int x38;               // 0x38
    int x3c;               // 0x3c
    YakumonoTable *yakumono; // 0x40 - per-stage yakumono manifest (see yakumono.h)
} GrData;        //

// Per-stage runtime object. The layout below names the runtime members the
// engine populates during stage init; the surrounding padding holds other
// stage state (notably the per-light helper records at +0x54 and the light
// JOBJ table at +0x104).
typedef struct GrObj
{
    GOBJ *gobj;                 // 0x000
    GroundKind gr_kind;         // 0x004 - physical ground (Gr_GetCurrentGrKind reads this)
    GrData *gr_data;            // 0x008
    u8 x00c[0xF4 - 0x0C];
    JOBJ *backdrop_jobj;        // 0x0F4 - distant skybox/horizon mesh attached
                                //         by 3D_CreateStageModel. NULL if the
                                //         stage's ModelSection.backdrop is NULL.
    u8 x0f8[0x104 - 0xF8];
    void *joint_table;          // 0x104 - per-joint {JOBJ*, JOBJDesc*} array (8-byte
                                //         stride) built from ModelSection.terrain by
                                //         grLoadStage; entry 0 = terrain root joint.
                                //         Indexed by Sky_SetupLights; walk the tree
                                //         from [0] to reach every terrain MObj.
    u8 x108[0x168 - 0x108];
    GOBJ *sky_gobj;             // 0x168 - fog/sky GObj built by Sky_InitFog.
                                //         hsd_object (+0x28) = HSD_Fog *,
                                //         userdata (+0x2C) = SkyState *.
    u8 x16c[0x714 - 0x16C];
    u32 fade_slot_id;           // 0x714 - lbfade slot ID owned by the sky
                                //         system (incremented per stage entry,
                                //         so it doubles as a freshness signal).
    AreaLight *area_light;      // 0x718 - KAR-proprietary directional light.
} GrObj;

static GrData **stc_grdatalookup = (GrData **)(0x80557638); // indexed by physical GroundKind
static GrObj **stc_grobj = (GrObj **)(0x805dd0e0 + 0x5ec);

StageKind Gm_GetCurrentStageKind();  // 0x800092d8 - reads GameData.stage_kind (+0xA97)
StageKind stGetCurrentStageKind();   // 0x80261ECC - reads the r13[0x7F8] StageKind cache (same value)
GroundKind Gr_GetCurrentGrKind();    // 0x800d1d3c - reads (*stc_grobj)->gr_kind (+0x04): the physical ground
GroundKind Gm_GetGrKindFromStageKind(StageKind stage_kind); // 0x80261ce8 - StageKind -> physical GroundKind

// Signed clearance from the stage out-of-bounds death box. pos is a world
// Vec3. Returns the minimum signed distance to any of the six
// StageNode.oob_min/oob_max planes (positive = inside the box, negative =
// already past a wall). Reads the box from (*stc_grobj)->gr_data->stage_node.
float calcDistanceFromOOB(Vec3 *pos);          // 0x800d4f20

// Total number of props the current stage places for a yakumono descriptor id
// (the same id that indexes PlayerStats.yakumono_break). Reads the stage's
// spawn-count table, so it is only meaningful while a stage is loaded and
// returns 0 for grounds with no table (only GR_CITY1 and GR_SANDS2 have one).
// City Trial's table: desc 33 = 10, desc 34 = 53, desc 35 = 41, desc 38 = 30.
int Gr_GetYakumonoSpawnTotal(int desc_id);     // 0x800f7db0

// 0x48-byte preset entry stored in the stage file's sky-block array
// (gr_data+0x34 -> [4] -> [0]). Each entry interpolates fog, screen tint,
// sky ambient color, and AreaLight params over `transition_frames`.
// Color fields are packed RGBA8888 u32 (high byte = R) - that's how
// GXColor_Lerp (0x80079c04) loads/stores them with single lwz/stw.
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

// Runtime sky state struct, owned by the fog/sky GObj at GrObj+0x168.
// Reachable via grobj->gobj[+0x168]+0x2C. Holds the current target preset,
// transition counter, and the lerp-start values for each interpolated field.
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

// Global EFB/erase color written each frame by Sky_Update step 4 and read
// by World_CObj+0x144 to drive HSD_SetEraseColor for the next CopyDisp.
// BSS, not heap. RGBA8888.
static u32 *stc_global_fog_color = (u32 *)0x80557484;

// First INFINITE non-HIDDEN LOBJ in the primary stage-light chain. Cached
// at stage init by Light_CreateForStage (via 0x80057598 walker). For City
// Trial this is the warm-white directional light at (-1000, 700, 1500) -
// the one that lights terrain. Write to its color (+0x10) / hw_color (+0x14)
// to re-tint terrain at runtime. NULL outside CT-style stages.
static struct LOBJ **stc_main_light = (struct LOBJ **)(0x805dd0e0 + 0x5fc);

// Spline path system - enemy walking paths embedded in stage data.
// Spline data is stored at GrObj+0x11C as a header {void *entries, int count}.
// Each entry (stride 0x18) contains forward/backward HSD spline pointers.
// Access via the functions below (they read from stc_grobj internally).
int Spline_GetCount(void);                               // 0x800cf38c - number of spline segments in current stage
void *Spline_GetForward(int segment);                    // 0x800cf3ac - forward HSD spline pointer for segment
void *Spline_GetBackward(int segment);                   // 0x800cf44c - backward HSD spline pointer for segment

// HSD spline evaluation. param is in [0.0, 1.0] range:
// 0.0 = first control point, 1.0 = last control point.
// Spline struct at spline_ptr has: u8 type (+0x0), s16 num_points (+0x2).
// Already linked: splGetSplinePoint, splArcLengthGetParameter, splArcLengthPoint (see enemy.h)

AudioEmitter Map_AllocAudioEmitter(int index);
#endif