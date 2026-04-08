#ifndef KAR_H_STAGE
#define KAR_H_STAGE

#include "datatypes.h"
#include "obj.h"
#include "hurt.h"
#include "collision.h"

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

typedef struct YakumonoParam
{
    union
    {
        struct
        {
            int joint_idx;
            int x4;
            int x8;
            int xc;
            int x10;
            int start_anim_idx;    // 0x14
            int active_anim_idx;   // 0x18
            int end_anim_idx;      // 0x1c
            int inactive_anim_idx; // 0x20
        } *lighthouse;
        struct
        {
            int x0;
            int x4;
        } *other;
    };
} YakumonoParam;

typedef struct YakumonoData
{
    GOBJ *gobj;           // 0x0
    int kind;             // 0x4
    YakumonoParam *param; // 0x8
    u8 xc[0x68];          // 0xc
    int state;            // 0x74
} YakumonoData;

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
    } *stage_node;         // 0x4
    int x8;                // 0x8
    void *lights;          // 0xc
    GrModelMotion *motion; // 0x10, pointer placed at runtime
    void *spline;          // 0x14
    void *pos_data;        // 0x18
    int x1c;               // 0x1c
    int x20;               // 0x1c
    int x24;               // 0x1c
    int x28;               // 0x1c
    int x2c;               // 0x1c
    int x30;               // 0x1c
    int x34;               // 0x1c
    int x38;               // 0x1c
    int x3c;               // 0x1c
    struct
    {
        YakumonoParam *data;
        int num;
    } *yakumono; // 0x40
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

void Gr_StateChange(YakumonoData *yp, int state_idx, int anim_idx, int joint_idx, int flags, float start_frame, float anim_rate, float blend_rate);

// Sky/lighting system — operates on GrObj, presets loaded from stage file
void Sky_Init(GrObj *grobj);                            // 0x8010f114 — initial sky setup per stage
void Sky_SetPresetIndex(GrObj *grobj, int preset_index); // 0x800dc630 — store preset index in sky state +0x1C
void Sky_LoadPreset(GrObj *grobj);                       // 0x800dc1b4 — load preset immediately (no transition)
void Sky_BeginTransition(GrObj *grobj, int preset_index); // 0x800dc354 — smooth transition to preset
void Sky_ApplyStoredIndex(GrObj *grobj);                 // 0x800dc4c0 — transition to stored index (+0x1C)
void Sky_Update(GrObj *grobj);                           // 0x800dc640 — per-frame interpolation
int Sky_GetPresetCount();                                // 0x800d5414 — total preset count from stage data
void Sky_TransitionGlobal(int preset_index);             // 0x800d5444 — transition via stc_grobj_ptr
void Sky_RestoreGlobal(void);                            // 0x800d546c — restore default sky preset
int Gm_Roll(int *weights, int count);                    // 0x800db2b8 — weighted random selection

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