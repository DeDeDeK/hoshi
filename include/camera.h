#ifndef KAR_H_CAM
#define KAR_H_CAM

#include "datatypes.h"

#define CM_CAMERA_MAX 32

typedef struct CamScissor
{
    u16 left;
    u16 right;
    u16 top;
    u16 bottom;
} CamScissor;

typedef struct CameraParam
{
    Vec3 interest;         // 0x00
    Vec3 eye;              // 0x0c
    Vec3 up;               // 0x18
    float fov;             // 0x24
} CameraParam;

typedef struct cmMainParamCommon
{
    u8 x0[0x14];                // 
    float aspect_mult_1p;       // 0x14
    float aspect_mult_2p;       // 0x18
    float aspect_mult_4p;       // 0x1c
    u8 x20[0x68];               // 0x20
    float fov_1p;               // 0x88
    float fov_2p;               // 0x8c
    float fov_4p;               // 0x90
    u8 x94[0x1D8];              // 0x94
    float fov_speed_max;        // 0x26c, maximum amount of added fov based on speed
    u8 x270[0xbc];              // 0x270
    float zoom_speed;           // 0x32c, zoom_amt gained per frame at full C-Stick Y
    float x330;                 // 0x330
    float x334;                 // 0x334
    float x338;                 // 0x338
    float x33c;                 // 0x33c
    float x340;                 // 0x340
    float x344;                 // 0x344
    float zoom_dist_min;        // 0x348, most negative zoom_amt (camera pulled in)
    float zoom_dist_max;        // 0x34c, largest zoom_amt (camera pushed out)
    float zoom_interest_raise;  // 0x350, interest raise at full zoom_amt
} cmMainParamCommon;

typedef struct CamInterest
{
    int x0;             // 0x00
    int x4;             // 0x04
    Vec3 pos;           // 0x08
    Vec3 pos_high;      // 0x14, this position is a little higher than the previous lol, its referenced when adjusting the camera position based on the c stick zoom value
    u8 x20[0x86];       // 0x20
    u16 ply;            // 0xa6
    u8 xa8_c0 : 2;      // 0xa8
    u8 is_grounded : 1; // 0xa8, 0x20, target machine or rider is on the ground
} CamInterest;

typedef struct CamData
{
    int kind;               // 0x0, index into the kind-descriptor table at 0x804a1f0c. 1 = normal, 6 = on foot, 9 = rail
    int x4;                 // 0x4
    int x8;                 // 0x8
    int xc;                 // 0xc
    int x10;                // 0x10
    CameraParam view;       // 0x14, from x138; what PlyCamGObj_Think hands the COBJ
    u8 x3c[0x38];           // 0x3c
    u8 x74[0x10];           // 0x74
    u8 zoom_enabled : 1;    // 0x84, 0x80, enables PlyCam_MachineZoomAdjust; raised while a kind feeds rotation_amt/zoom_amt
    float rotation_amt;     // 0x88, C-Stick X orbit angle, radians
    float zoom_amt;         // 0x8c, C-Stick Y eye distance offset, clamped to cmMainParamCommon zoom_dist_min..max
    float interest_raise;   // 0x90, interest raise along up, scaled from zoom_amt by cmMainParamCommon.zoom_interest_raise
    CamInterest *target;    // 0x94, camera target. is 0x450 of riderdata?
    u8 x98[0x28];           // 0x98
    CameraParam xc0;        // 0xc0, solved this frame by the kind's per-frame callback
    CameraParam xe8;        // 0xe8, xc0 smoothed toward over time; gets copied directly from the cobj eye position @ 800b783c
    CameraParam x110;       // 0x110, xe8 after PlyCam_MachineZoomAdjust applies rotation_amt/zoom_amt
    CameraParam x138;       // 0x138,
    Vec3 x160;              // 0x160, position referenced when updating audio sources
    u8 x16c[0x10];          // 0x16c
    PlayerCamData *player_cam_data; // 0x17c
    Vec3 eye_pos;           // 0x180
    Vec3 interest_pos;      // 0x18c
    u8 x198[0x28];          // 0x198
    float x1c0;             // 0x1c0, on-foot C-Stick X orbit budget, drained a fraction per frame
    u8 x1c4[0x104];         // 0x1c4
    float x2c8;             // 0x2c8,
} CamData;

typedef struct QuakeSource QuakeSource;

// Per-view screen-shake record from Quake_CreateView. Quake_Think (priority 11) rewrites
// the offsets and gate every frame, and PlyCamGObj_Think (priority 13) shifts the eye
// and interest along the camera's right/up by them while gate > 0, so a direct write
// must land between the two.
typedef struct CamShakeRec
{
    struct CamShakeRec *next; // 0x00
    int id;                   // 0x04
    Vec3 view_eye;            // 0x08, CamData.view.eye
    QuakeSource *sources;     // 0x14, sources owned by this view
    int x18;                  // 0x18
    int enabled;              // 0x1c, init 1; 0 zeroes the offsets
    float scale_right;        // 0x20, init 1.0
    float scale_up;           // 0x24, init 1.0
    float eye_right;          // 0x28, eye offset along the camera's right axis
    float eye_up;             // 0x2c, eye offset along the camera's up axis
    float mag;                // 0x30, the winning source's magnitude
    float interest_right;     // 0x34, interest offset along right
    float interest_up;        // 0x38, interest offset along up
    float interest_mag;       // 0x3c
    int source_num;           // 0x40, sources seen this frame
    float gate;               // 0x44, > 0 enables the offsets
} CamShakeRec;

typedef enum QuakeKind
{
    QUAKE_KIND_NONE,     // only the callback writes the output
    QUAKE_KIND_OSC,      // oscillator; nothing creates one
    QUAKE_KIND_CONSTANT, // output set by Quake_SetConstantOffset
    QUAKE_KIND_JOBJ,     // output from the GObj root's trans.xy * scale.xy
} QuakeKind;

typedef enum QuakeEndMode
{
    QUAKE_END_DURATION = 1, // once frame reaches duration
    QUAKE_END_SETTLED,      // once out_mag drops below 0.0001
    QUAKE_END_NEVER,        // as is any other value
    QUAKE_END_GOBJ,         // once the QUAKE_KIND_JOBJ GObj is gone
} QuakeEndMode;

// Nonzero ends the source.
typedef int (*QuakeSourceCB)(QuakeSource *src);

// One shake source (pool of 256). Each view applies only its highest-priority source,
// its own or global. A positioned source fades out between 40 and 500 units from the eye.
struct QuakeSource
{
    QuakeSource *next;       // 0x00, every live source
    QuakeSource *next_view;  // 0x04, the owning view's list, or the global list
    u32 id;                  // 0x08, owning view id << 16 | serial
    Vec3 pos;                // 0x0c, falloff origin unless no_pos
    u8 kind;                 // 0x18, QuakeKind
    u8 end_mode;             // 0x19, QuakeEndMode
    u16 priority;            // 0x1a
    u16 frame;               // 0x1c, frames lived
    u16 duration;            // 0x1e
    union
    {
        struct
        {
            float right;
            float up;
            float mag;
        } constant;          // QUAKE_KIND_CONSTANT
        GOBJ *gobj;          // QUAKE_KIND_JOBJ
        u8 osc[0x38];        // QUAKE_KIND_OSC
    } u;                     // 0x20
    QuakeSourceCB cb;        // 0x58, runs every frame after the kind's own step
    float amp_right;         // 0x5c
    float amp_up;            // 0x60
    float out_right;         // 0x64
    float out_up;            // 0x68
    float out_mag;           // 0x6c, the view's gate is out_mag * falloff * max(amp)
    int eye_only;            // 0x70, leaves the interest unshaken
    int no_pos;              // 0x74, no distance falloff
};

// Runs from Effect_Init at every 3D load, resetting both pools; a held source id is
// stale after it.
void Quake_Init(void);                    // 0x8006c798
void Quake_UpdateView(CamShakeRec *rec);  // 0x8006d690, one record's offsets and gate
void Quake_Think(GOBJ *gobj);             // 0x8006d95c
CamShakeRec *Quake_CreateView(void);      // 0x8006dc10, NULL when the pool is full
void Quake_DestroyView(int view_id);      // 0x8006ca5c, with every source it owns
// Both write to address 0 when no source has the id; remove only a live id.
void Quake_FreeSource(CamShakeRec *view, u32 id); // 0x8006c8e0
void Quake_RemoveSource(u32 id);                  // 0x8006ca00
int Quake_StepOscillator(void *osc, float *out);  // 0x8006cb58, out = {right, up, mag}
// view NULL shakes every view; pos NULL means no falloff. NULL when the pool is full.
QuakeSource *Quake_AllocSource(CamShakeRec *view, int priority, QuakeEndMode end_mode, int duration, Vec3 *pos,
                               QuakeSourceCB cb, float amp_right, float amp_up); // 0x8006dcc0
// The ones below return the source id, 0 on failure.
u32 Quake_AddJObjSource(CamShakeRec *view, int priority, QuakeEndMode end_mode, int duration, GOBJ *gobj, Vec3 *pos,
                        QuakeSourceCB cb, float amp_right, float amp_up);        // 0x8006de2c
// QUAKE_END_NEVER, no position, amplitude 1.
u32 Quake_AddConstantSource(CamShakeRec *view, int priority, float right, float up); // 0x8006de8c
int Quake_SetConstantOffset(u32 id, float right, float up); // 0x8006df74, 0 unless id is a live constant source
void Quake_SetEyeOnly(u32 id, int eye_only);                // 0x8006e03c
void Quake_SetAmplitude(u32 id, float amp_right, float amp_up); // 0x8006e07c
void Quake_SetPosition(u32 id, Vec3 *pos);                  // 0x8006e0c0, NULL = no falloff

typedef struct PlayerCamData
{
    CamData *cam_data;      // 0x0
    COBJ *cobj;             // 0x4
    int x8;                 // 0x8
    u8 xc_80 : 1;           // 0xc, 0x80
    u8 xc_40 : 1;           // 0xc, 0x40
    u8 xc_20 : 1;           // 0xc, 0x20
    u8 xc_10 : 1;           // 0xc, 0x10
    u8 xd;                  // 0xd
    u8 xe;                  // 0xe
    u8 xf;                  // 0xf
    u8 x10[0x58];           // 0x10
    u16 ply;                // 0x68
    u16 view_index;         // 0x6a, for example if p1 and p4 are playing with 2 views, p4 would have 1 for this variable
    int x6c;                // 0x6c
    u8 controller_idx;      // 0x70
    u8 lod;                 // 0x71, level of detail this camera uses (1 = highest poly, 2 is 2p, 3 is 4p?), 0 = none? also dictates fov @ 800c062c
    u8 x72;                 // 0x72
    u8 x73;                 // 0x73
    CamShakeRec *shake;     // 0x74
} PlayerCamData;

typedef struct PlayerCamLookup
{
    u8 x0[0xac];                        // 0x0
    void *unk_data;                     // 0xac
    GOBJ *cam_gobjs[CM_CAMERA_MAX];     // 0xb0, has data PlayerCamData
    COBJ *cam_cobjs[CM_CAMERA_MAX];     // 0x130
    int x1b0[32];                       // 0x1b0
    int x230;                           // 0x230
    cmMainParamCommon *param;           // 0x234, 
    int x238;                           // 0x238
    u8 x23c[4];                         // 0x23c
    struct                              // 0x240
    {
        float normal;                   // 0x240
        float rail;                     // 0x244
    } ply_distance[4];
} PlayerCamLookup;

static PlayerCamLookup *stc_plycam_lookup = (PlayerCamLookup *)0x80557248; // array of 32, this is actually part of a larger struct

COBJ *PlyCam_GetCObj(int cam_index);                            // 0x800b8054
void PlyCamGObj_Think(GOBJ *cam_gobj);                          // 0x800b3540, per-frame drive for one player camera
void PlyCam_MachineZoomAdjust(CamData *cam);                    // 0x800b61f4, folds rotation_amt/zoom_amt into x110; no-op unless zoom_enabled
// C-Stick to rotation_amt / zoom_amt. p = {rot_scale, zoom_min, zoom_max, interest_raise}.
void cameraControlThink(CamData *cam, int pad_index, float *p); // 0x800b67cc
int PlyCam_OnMachineThink(CamData *cam, int pad_index);         // 0x800c04b0, kind 1 input think
int PlyCam_OnFootThink(CamData *cam, int pad_index);            // 0x800cb3b4, kind 6 input think; C-Stick X only, no zoom
void PlyCam_OnFootEnter(CamData *cam, int *unk);                // 0x800cc050, kind 6 on-enter
// Plays the machine's cannon_path while it is in a cannon, so the path is in absolute stage units.
void PlyCam_CannonThink(CamData *cam);                          // 0x800caee0
// Kind 1 per-frame solve into xc0. Its up comes from Gr_GetDownVector at target->pos_high,
// so the camera rolls with gravity; PlyCam_OnFootSolve does the same.
void PlyCam_MachineSolve(CamData *cam, int is_first_frame);     // 0x800c05ac
// Kind 1 smoothing of xc0 into xe8. The up is lerped without renormalizing, so it
// shrinks through a near-reversal of gravity.
void PlyCam_MachineBlend(CamData *cam);                         // 0x800c24e8
void PlyCam_OnFootSolve(CamData *cam, int is_first_frame);      // 0x800cb500, kind 6 per-frame solve into xc0
void PlyCam_OnFootBlend(CamData *cam);                          // 0x800cbec8, kind 6 smoothing of xc0 into xe8
void PlyCam_ReplayEnter(CamData *cam, int *args);               // 0x800c89f0, kind 3 on-enter
int PlyCam_ReplayThink(CamData *cam, int pad_index);            // 0x800c754c, kind 3 input think; ORs every human pad
void PlyCam_CollectEyeHitboxes(GOBJ *cam_gobj);                 // 0x800cd5e4, attack regions containing the eye
void PlyCam_GetFullscreenScissor(CamScissor *out);              // 0x80068574
void PlyCam_Get2PScissor(int screen, CamScissor *out);          // 0x80068588, GameData.ply_view_desc[].screen
void PlyCam_Get4PScissor(int screen, CamScissor *out);          // 0x800685ac

// Scripted cinematic camera, on a spare slot (5-31) of cam_gobjs. The anchor's
// world matrix is sampled once and every eye/interest key transformed through it,
// so the animation is authored in anchor-local space. Returns the slot index, the
// handle PlyCam_DestroySlot takes. desc is {HSD_CObjDesc*, HSD_CameraAnim**}, a
// NULL-terminated anim array.
int PlyCam_CreateAnimCamera(int ply, JOBJ *anchor, void *desc, int open_frames, int restore_frames); // 0x800b9c74
void PlyCam_DestroySlot(int slot);                              // 0x800b94c8, destroys the camera GObj and clears the slot
int PlyCam_FindFreeSlot(void);                                  // 0x800b9370, first free slot in 5-31
// Takes a free slot, letterboxes it over the cameras sharing `screen` and switches it to
// kind 3 (replay). Returns the slot.
int PlyCam_CreateReplayCamOnScreen(int ply, int controller_idx, int lod, int screen, int flag); // 0x800b94e8
#endif