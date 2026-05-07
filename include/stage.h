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

typedef enum GroundKind
{
    GRKIND_CITY1 = 9,
    GRKIND_DRAG1,
    GRKIND_DRAG2,
    GRKIND_DRAG3,
    GRKIND_DRAG4,
    GRKIND_AIRGLIDER,
    GRKIND_TARGETFLIGHT,
    GRKIND_HIGHJUMP,
    GRKIND_KIRBYMELEE1,
    GRKIND_KIRBYMELEE2,
    GRKIND_DESTRUCTIONDERBY1,
    GRKIND_DESTRUCTIONDERBY2,
    GRKIND_DESTRUCTIONDERBY3,
    GRKIND_DESTRUCTIONDERBY4,
    GRKIND_DESTRUCTIONDERBY5,
    GRKIND_SINGLERACE1,
    GRKIND_SINGLERACE2,
    GRKIND_SINGLERACE3,
    GRKIND_SINGLERACE4,
    GRKIND_SINGLERACE5,
    GRKIND_SINGLERACE6,
    GRKIND_SINGLERACE7,
    GRKIND_SINGLERACE8,
    GRKIND_SINGLERACE9,
    GRKIND_VSKINGDEDEDE,
    GRKIND_NUM,
} GroundKind;

typedef enum StageKind
{
    STAGEKIND_NUM = 60,
} StageKind;

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

// YakumonoParam and YakumonoData are defined in yakumono.h.
// The minimal forward-style fields used to live here; the comprehensive
// definitions (with full proc-callback offsets, HurtData pointer, audio,
// etc.) now live alongside the rest of the yakumono framework. See
// docs/yakumono-system.md for the architecture overview.

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

// ModelSection — 4 dwords pointed at by GrData.model_section. Slots 0
// and 1 are JOBJDesc** (a pointer to a slot containing the JOBJDesc *
// instantiated by 3D_CreateStageModel), so a stage can ship a
// terrain mesh, a separate skybox/backdrop mesh, both, or neither.
// Confirmed by the loader at 0x800dcc28 reading +0x04 and attaching
// the result to GrObj+0xF4. See docs/sky-backdrop-system.md.
typedef struct ModelSection
{
    JOBJDesc **terrain;  // 0x00 — main playable geometry
    JOBJDesc **backdrop; // 0x04 — secondary skybox/horizon mesh
    void *unk_8;         // 0x08
    void *unk_c;         // 0x0C
} ModelSection;

typedef struct GrData // exists in the stage file
{                     //
    int flags;        // 0x0
    struct
    {
        int x0;
        float machine_accel;
        float scale;
        float gravity_unk; // flight dropoff?
        Vec3 gravity_force; // actually gravity direction?
        int fog_flags;
    } *stage_node;                  // 0x4
    int x8;                         // 0x8
    ModelSection *model_section;    // 0xc — terrain + backdrop JObj descs
    GrModelMotion *motion;          // 0x10, pointer placed at runtime
    void *spline;          // 0x14
    void *pos_data;        // 0x18
    int x1c;               // 0x1c
    int x20;               // 0x1c
    int x24;               // 0x1c
    int x28;               // 0x1c
    int x2c;               // 0x1c
    EventConfigData *event_config; // 0x30 — set by fn_grSetupCityEventData (0x8010f7c4) when entering City Trial. Loaded regardless of the events on/off setting; same pointer is also stored as EventCheckData.data when events are enabled.
    int x34;               // 0x1c
    int x38;               // 0x1c
    int x3c;               // 0x1c
    YakumonoTable *yakumono; // 0x40 — per-stage yakumono manifest (see yakumono.h)
} GrData;        //

typedef struct GrObj
{
    GOBJ *gobj;         // 0x0
    GroundKind gr_kind; // 0x4
    GrData *gr_data;    // 0x8
} GrObj;

static GrData **stc_grdatalookup = (GrData **)(0x80557638); // indexed by gr_kind
static GrObj **stc_grobj = (GrObj **)(0x805dd0e0 + 0x5ec);

StageKind Gm_GetCurrentStageKind();
GroundKind Gm_GetCurrentGrKind();
GroundKind Gm_GetGrKindFromStageKind(StageKind stage_kind);

// Gr_StateChange and other yakumono framework APIs are declared in yakumono.h.

// Sky/lighting system — operates on GrObj, presets loaded from stage file.
// See docs/sky-lighting-system.md for the full architecture.

// 0x48-byte preset entry stored in the stage file's sky-block array
// (gr_data+0x34 -> [4] -> [0]). Each entry interpolates fog, screen tint,
// sky ambient color, and AreaLight params over `transition_frames`.
// Color fields are packed RGBA8888 u32 (high byte = R) — that's how
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
    u8 light_vis_flag;         // 0x44 bit 0 → AreaLight registry +0x38 bit 0x80
    u8 _pad[3];                // 0x45
} SkyPresetEntry;
_Static_assert(sizeof(SkyPresetEntry) == 0x48, "SkyPresetEntry must be 0x48 bytes");

// Runtime sky state struct, owned by the fog/sky GObj at GrObj+0x168.
// Reachable via grobj->gobj[+0x168]+0x2C. Holds the current target preset,
// transition counter, and the lerp-start values for each interpolated field.
typedef struct SkyState
{
    SkyPresetEntry *target_preset;  // 0x00 current target preset
    s32 transition_frame_counter;   // 0x04 0 → target.transition_frames
    u32 start_fog_color;            // 0x08 RGBA lerp start; reused as Sky_Update output mirror
    float start_fog_start;          // 0x0C
    float start_fog_end;            // 0x10
    u32 start_sky_color;            // 0x14 RGBA
    u32 current_output_sky_color;   // 0x18 RGBA written every frame (consumer not yet identified)
    s32 current_preset_index;       // 0x1C
    AreaLightData start_area_light; // 0x20 lerp start for AreaLight_Lerp (0x2C bytes)
} SkyState;
_Static_assert(sizeof(SkyState) == 0x4C, "SkyState must be 0x4C bytes");

void Sky_Init(GrObj *grobj);                              // 0x8010f114 — initial sky setup per stage
void Sky_SetPresetIndex(GrObj *grobj, int preset_index);  // 0x800dc630 — store preset index in sky state +0x1C
void Sky_LoadPreset(GrObj *grobj);                        // 0x800dc1b4 — load preset immediately (no transition)
void Sky_BeginTransition(GrObj *grobj, int preset_index); // 0x800dc354 — smooth transition to preset
void Sky_ApplyStoredIndex(GrObj *grobj);                  // 0x800dc4c0 — transition to stored index (+0x1C)
void Sky_Update(GrObj *grobj);                            // 0x800dc640 — per-frame interpolation
int Sky_GetPresetCount(void);                             // 0x800d5414 — total preset count from stage data
void Sky_TransitionGlobal(int preset_index);              // 0x800d5444 — transition via stc_grobj
void Sky_RestoreGlobal(void);                             // 0x800d546c — restore default sky preset
void Sky_SetupLights(GrObj *grobj, int jobj_idx);         // 0x800db774 — toggle JOBJ visibility flags (no LOBJ creation)
void Sky_InitFog(GrObj *grobj);                           // 0x800dbfa8 — build the fog GObj at grobj+0x168
void Sky_AllocFade(GrObj *grobj);                         // 0x800eef04 — alloc lbfade slot 3 at grobj+0x714
void Sky_BeginFade(GrObj *grobj, u32 *color, int frames); // 0x800eef50 — fire screen tint via ScreenFade_Begin
void Sky_FreeFade(GrObj *grobj);                          // 0x800eefb0 — free lbfade slot at scene teardown
int Gm_Roll(int *weights, int count);                     // 0x800db2b8 — weighted random selection

// Global EFB/erase color written each frame by Sky_Update step 4 and read
// by World_CObj+0x144 to drive HSD_SetEraseColor for the next CopyDisp.
// BSS, not heap. RGBA8888.
static u32 *stc_global_fog_color = (u32 *)0x80557484;

// First INFINITE non-HIDDEN LOBJ in the primary stage-light chain. Cached
// at stage init by Light_CreateForStage (via 0x80057598 walker). For City
// Trial this is the warm-white directional light at (-1000, 700, 1500) —
// the one that lights terrain. Write to its color (+0x10) / hw_color (+0x14)
// to re-tint terrain at runtime. NULL outside CT-style stages.
static struct LOBJ **stc_main_light = (struct LOBJ **)(0x805dd0e0 + 0x5fc);

// Spline path system — enemy walking paths embedded in stage data.
// Spline data is stored at GrObj+0x11C as a header {void *entries, int count}.
// Each entry (stride 0x18) contains forward/backward HSD spline pointers.
// Access via the functions below (they read from stc_grobj internally).
int Spline_GetCount(void);                               // 0x800cf38c — number of spline segments in current stage
void *Spline_GetForward(int segment);                    // 0x800cf3ac — forward HSD spline pointer for segment
void *Spline_GetBackward(int segment);                   // 0x800cf44c — backward HSD spline pointer for segment

// HSD spline evaluation. param is in [0.0, 1.0] range:
// 0.0 = first control point, 1.0 = last control point.
// Spline struct at spline_ptr has: u8 type (+0x0), s16 num_points (+0x2).
// Already linked: splGetSplinePoint, splArcLengthGetParameter, splArcLengthPoint (see enemy.h)

AudioEmitter Map_AllocAudioEmitter(int index);
#endif