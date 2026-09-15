#ifndef MEX_H_OBJ
#define MEX_H_OBJ

#include "structs.h"
#include "datatypes.h"
#include "gx.h"

// JObj Flags
#define JOBJ_SKELETON (1 << 0)             // 0x00000001
#define JOBJ_SKELETON_ROOT (1 << 1)        // 0x00000002
#define JOBJ_ENVELOPE_MODEL (1 << 2)       // 0x00000004
#define JOBJ_CLASSICAL_SCALING (1 << 3)    // 0x00000008
#define JOBJ_HIDDEN (1 << 4)               // 0x00000010
#define JOBJ_PTCL (1 << 5)                 // 0x00000020
#define JOBJ_MTX_DIRTY (1 << 6)            // 0x00000040
#define JOBJ_LIGHTING (1 << 7)             // 0x00000080
#define JOBJ_TEXGEN (1 << 8)               // 0x00000100
#define JOBJ_BILLBOARD (1 << 9)            // 0x00000200
#define JOBJ_VBILLBOARD (2 << 9)           // 0x00000400
#define JOBJ_HBILLBOARD (3 << 9)           // 0x00000600
#define JOBJ_RBILLBOARD (4 << 9)           // 0x00000800
#define JOBJ_INSTANCE (1 << 12)            // 0x00001000
#define JOBJ_PBILLBOARD (1 << 13)          // 0x00002000
#define JOBJ_SPLINE (1 << 14)              // 0x00004000
#define JOBJ_FLIP_IK (1 << 15)             // 0x00008000
#define JOBJ_SPECULAR (1 << 16)            // 0x00010000
#define JOBJ_USE_QUATERNION (1 << 17)      // 0x00020000
#define JOBJ_OPA (1 << 18)                 // 0x00040000 only rendered with gx pass 3
#define JOBJ_XLU (1 << 19)                 // 0x00080000
#define JOBJ_TEXEDGE (1 << 20)             // 0x00100000
#define JOBJ_NULL (0 << 21)                // 0x00000000
#define JOBJ_JOINT1 (1 << 21)              // 0x00100000
#define JOBJ_JOINT2 (2 << 21)              // 0x00200000
#define JOBJ_EFFECTOR (3 << 21)            // 0x00300000
#define JOBJ_USER_DEFINED_MTX (1 << 23)    // 0x00800000
#define JOBJ_MTX_INDEPEND_PARENT (1 << 24) // 0x01000000
#define JOBJ_MTS_INDEPEND_SRT (1 << 25)    // 0x02000000
#define JOBJ_GENERALFLAG (1 << 26)         // 0x04000000
#define JOBJ_GENERALFLAG2 (1 << 27)        // 0x08000000
#define JOBJ_ROOT_OPA (1 << 28)            // 0x10000000 only rendered with gx pass 3
#define JOBJ_ROOT_XLU (1 << 29)            // 0x20000000
#define JOBJ_ROOT_TEXEDGE (1 << 30)        // 0x40000000
#define JOBJ_31 (1 << 31)                  // 0x80000000

#define JOBJ_ROOT_MASK (JOBJ_ROOT_OPA | JOBJ_ROOT_TEXEDGE | JOBJ_ROOT_XLU)

// MObj Flags
#define HSD_A_M_AMBIENT_R 1
#define HSD_A_M_AMBIENT_G 2
#define HSD_A_M_AMBIENT_B 3
#define HSD_A_M_DIFFUSE_R 4
#define HSD_A_M_DIFFUSE_G 5
#define HSD_A_M_DIFFUSE_B 6
#define HSD_A_M_SPECULAR_R 7
#define HSD_A_M_SPECULAR_G 8
#define HSD_A_M_SPECULAR_B 9
#define HSD_A_M_ALPHA 10
#define HSD_A_M_PE_REF0 11
#define HSD_A_M_PE_REF1 12
#define HSD_A_M_PE_DSTALPHA 13
#define RENDER_DIFFUSE_SHIFT 0
#define RENDER_DIFFUSE_BITS (3 << RENDER_DIFFUSE_SHIFT)
#define RENDER_DIFFUSE_MAT0 (0 << RENDER_DIFFUSE_SHIFT)
#define RENDER_DIFFUSE_MAT (1 << RENDER_DIFFUSE_SHIFT)
#define RENDER_DIFFUSE_VTX (2 << RENDER_DIFFUSE_SHIFT)
#define RENDER_DIFFUSE_BOTH (3 << RENDER_DIFFUSE_SHIFT)
#define RENDER_CONSTANT (1 << 0) // 0x00000001
#define RENDER_VERTEX (1 << 1)   // 0x00000002
#define RENDER_DIFFUSE (1 << 2)  // 0x00000004
#define RENDER_SPECULAR (1 << 3) // 0x00000008
#define CHANNEL_FIELD (RENDER_CONSTANT | RENDER_VERTEX | RENDER_DIFFUSE | RENDER_SPECULAR)
#define RENDER_TEX0 (1 << 4)  // 0x00000010
#define RENDER_TEX1 (1 << 5)  // 0x00000020
#define RENDER_TEX2 (1 << 6)  // 0x00000040
#define RENDER_TEX3 (1 << 7)  // 0x00000080
#define RENDER_TEX4 (1 << 8)  // 0x00000100
#define RENDER_TEX5 (1 << 9)  // 0x00000200
#define RENDER_TEX6 (1 << 10) // 0x00000400
#define RENDER_TEX7 (1 << 11) // 0x00000800
#define RENDER_TEXTURES (RENDER_TEX0 | RENDER_TEX1 | RENDER_TEX2 | RENDER_TEX3 | RENDER_TEX4 | RENDER_TEX5 | RENDER_TEX6 | RENDER_TEX7)
#define RENDER_TOON (1 << 12)                         // 0x00001000
#define RENDER_ALPHA_SHIFT 13                         //
#define RENDER_ALPHA_BITS (3 << RENDER_ALPHA_SHIFT)   // 0x00006000
#define RENDER_ALPHA_COMPAT (0 << RENDER_ALPHA_SHIFT) //
#define RENDER_ALPHA_MAT (1 << RENDER_ALPHA_SHIFT)    // 0x00002000
#define RENDER_ALPHA_VTX (2 << RENDER_ALPHA_SHIFT)    // 0x00004000
#define RENDER_ALPHA_BOTH (3 << RENDER_ALPHA_SHIFT)   // 0x00006000
#define RENDER_SHADOW (1 << 26)                       // 0x04000000
#define RENDER_ZMODE_ALWAYS (1 << 27)                 // 0x08000000
#define RENDER_NO_ZUPDATE (1 << 29)                   // 0x20000000
#define RENDER_XLU (1 << 30)                          // 0x40000000

// DOBJ flags
#define DOBJ_HIDDEN (1 << 0)           // 0x00000001
#define DOBJ_RENDER_ORDER_UNK (1 << 2) // 0x00000004

// POBJ flags. HSD_PObjDisp (0x80407988) maps the two cull bits to GX cull modes
// and skips the POBJ entirely when both are set. An outward-facing surface is
// wound clockwise, so CULLBACK is ordinary backface culling and CULLFRONT leaves
// only the far side of a closed shape.
#define POBJ_ANIM (1 << 3)
#define POBJ_SKIN (0 << 12)
#define POBJ_SHAPEANIM (1 << 12)
#define POBJ_ENVELOPE (2 << 12)
#define POBJ_CULLFRONT (1 << 14)
#define POBJ_CULLBACK (1 << 15)

// AOBJ flags
#define AOBJ_REWINDED (1 << 26)   // 0x04000000
#define AOBJ_FIRST_PLAY (1 << 27) // 0x08000000
#define AOBJ_NO_UPDATE (1 << 28)  // 0x10000000
#define AOBJ_LOOP (1 << 29)       // 0x20000000
#define AOBJ_NO_ANIM (1 << 30)    // 0x40000000

// LOBJ flags
#define LOBJ_AMBIENT (0 << 0)
#define LOBJ_INFINITE (1 << 0)
#define LOBJ_POINT (2 << 0)
#define LOBJ_SPOT (3 << 0)
#define LOBJ_DIFFUSE (1 << 2)
#define LOBJ_SPECULAR (1 << 3)
#define LOBJ_ALPHA (1 << 4)
#define LOBJ_HIDDEN (1 << 5)
#define LOBJ_RAW_PARAM (1 << 6)
#define LOBJ_DIFF_DIRTY (1 << 7)
#define LOBJ_SPEC_DIRTY (1 << 8)

// COBJ flags
#define COBJ_UP_VECTOR_UNK (1 << 0) // 0x00000001, related to initing the up vector on cobj load (8036a440)
#define COBJ_MTX_DIRTY (1 << 1)     // 0x00000002, updates the view mtx when this flag is lowered during COBJSetCurrent @ 80368564
#define COBJ_40000000 (1 << 30)     // 0x40000000, is checked during CObjMtxIsDirty (8036959c)
#define COBJ_80000000 (1 << 31)     // 0x80000000, is raised when the COBJSetCurrent returns

#define PROJ_PERSPECTIVE 1
#define PROJ_FRUSTRUM 2
#define PROJ_ORTHO 3

// Anim flags (used for JObj_XByFlags)
#define JObj_ANIM 0x1
#define MOBJ_ANIM 0x4
#define TOBJ_ANIM 0x10
#define ALL_ANIM 0x7FF

// Macro
#define JObj_PauseOnFrame(jobj, child_index, flags, frame)                    \
    {                                                                         \
        JOBJ *this_jobj;                                                      \
        if (child_index != 0)                                                 \
            this_jobj = JObj_GetIndex(jobj, child_index);                     \
        else                                                                  \
            this_jobj = jobj;                                                 \
        JObj_ForEachAnim(this_jobj, 6, flags, AOBJ_ReqAnim, 1, (float)frame); \
        JObj_AnimAll(this_jobj);                                              \
        JObj_ForEachAnim(this_jobj, 6, flags, AOBJ_StopAnim, 6, 0, 0);        \
    }
#define JObj_PlayOnFrame(jobj, child_index, flags, frame)                     \
    {                                                                         \
        JOBJ *this_jobj;                                                      \
        if (child_index != 0)                                                 \
            this_jobj = JObj_GetIndex(jobj, child_index);                     \
        else                                                                  \
            this_jobj = jobj;                                                 \
        JObj_ForEachAnim(this_jobj, 6, flags, AOBJ_ReqAnim, 1, (float)frame); \
        JObj_AnimAll(this_jobj);                                              \
    }
#define JObj_GetChildPosition(jobj, child_index, pos)      \
    {                                                      \
        JOBJ *_this_jobj = jobj;                           \
        if (child_index != 0)                              \
            _this_jobj = JObj_GetIndex(jobj, child_index); \
        if (_this_jobj)                                    \
            JObj_GetWorldPosition(_this_jobj, 0, pos);     \
    }

typedef enum ForEachAnimFlag //  (used for JObj_ForEachAnim)
{
    AOBJFLAG_JOBJ = 0x1,
    AOBJFLAG_MOBJ = 0x80,
    AOBJFLAG_TOBJ = 0x400,
    AOBJFLAG_ALL = 0x7FF,
} ForEachAnimFlag;

typedef enum HSD_ObjKind
{
    HSD_OBJKIND_NONE = 0,
    HSD_OBJKIND_COBJ,
    HSD_OBJKIND_LOBJ,
    HSD_OBJKIND_JOBJ,
    HSD_OBJKIND_FOG
} HSD_ObjKind;

/*** Structs ***/

struct HSD_Obj
{
    HSD_ClassInfo *parent;    // 0x0
    s16 ref_count;            // 0x4
    s16 ref_count_individual; // 0x6
};

struct GOBJ
{
    short entity_class;                          // 0x0
    char p_link;                                 // 0x2, used to classify similar gobjs together.
    char gx_link;                                // 0x3. 0-63 are gx. 64+ are reserved for camera objects
    char p_priority;                             // 0x4
    char gx_pri;                                 // 0x5
    char obj_kind;                               // 0x6
    char data_kind;                              // 0x7
    GOBJ *next;                                  // 0x8
    GOBJ *previous;                              // 0xC
    GOBJ *nextOrdered;                           // 0x10
    GOBJ *previousOrdered;                       // 0x14
    GOBJProc *proc;                              // 0x18
    void (*gx_cb)(GOBJ *g, int pass);            // 0x1C
    u64 cobj_links;                              // 0x20. this is used to know which gobjs to render
    void *hsd_object;                            // 0x28
    void *userdata;                              // 0x2C
    void (*destructor_function)(void *userdata); // 0x30
    int unk_linked_list;                         // 0x34
};

struct GOBJProc
{
    GOBJ *parent;
    GOBJProc *next;
    GOBJProc *prev;
    char s_link;     // 0xC
    char x0d_80 : 1; // 0xD
    char x0d_40 : 1;
    char update_idx : 2;
    char x0d_08 : 1;
    char x0d_04 : 1;
    char x0d_02 : 1;
    char x0d_01 : 1;
    GOBJ *parentGOBJ;       // 0x10
    void (*cb)(GOBJ *gobj); // function callback
};

struct GXList
{
    // pointed to @ -0x3e80(r13)
    GOBJ *gx_render[63]; // pointer to 63 gobjs
    GOBJ *gx_camera;     // pointer to the highest priority cobj gobj. they are linked together via the next member.
};

struct TOBJ
{
    HSD_Obj parent;
    TOBJ *next;
    u32 id;                           // GXTexMapID
    u32 src;                          // GXTexGenSrc 0x10
    u32 mtxid;                        // 0x14
    Vec4 rotate;                      // 0x18
    Vec3 scale;                       // 0x28
    Vec3 translate;                   // 0x34
    u32 wrap_s;                       // 0x40 GXTexWrapMode
    u32 wrap_t;                       // 0x44 GXTexWrapMode
    u8 repeat_s;                      // 0x48
    u8 repeat_t;                      // 0x49
    u16 anim_id;                      // 0x4A
    u32 flags;                        // 0x4C
    f32 blending;                     // 0x50
    u32 magFilt;                      // 0x54 GXTexFilter
    struct _HSD_ImageDesc *imagedesc; // 0x58
    struct _HSD_Tlut *tlut;           // 0x5C
    struct _HSD_TexLODDesc *lod;      // 0x60
    AOBJ *aobj;                       // 0x64
    struct _HSD_ImageDesc **imagetbl; // 0x68
    struct _HSD_Tlut **tluttbl;       // 0x6C
    int tlut_no;                      // 0x70
    Mtx mtx;                          // 0x74
    u32 coord;                        // GXTexCoordID
    struct _HSD_TObjTev *tev;
};

typedef struct _HSD_AObjDesc
{
    u32 flags;                      // 0x00
    f32 end_frame;                  // 0x04
    struct _HSD_FObjDesc *fobjdesc; // 0x08
    u32 obj_id;                     // 0x0C
} HSD_AObjDesc;

// One animation track. `track` is the HSD_A_T_* channel it drives - for a joint,
// 1-3 rotation, 5-7 translation, 8-10 scale, 12 branch.
//
// `buffer` is a keyframe stream of exactly `length` bytes, walked by 0x80403858
// under HSD_FObjInterpretAnim (0x80404928): a run header byte carrying the
// interpolation op in its low nibble and (count - 1) in bits 4-6, extended by
// 7-bit groups while bit 7 is set, then per key the value, the slope for ops that
// carry one, and the frame delta as a 7-bit varint. Values and slopes are
// little-endian; flag 0 is a raw float, otherwise bits 5-7 pick s16/u16/s8/u8
// (0x20/0x40/0x60/0x80) and bits 0-4 the power of two to divide by.
typedef struct _HSD_FObjDesc
{
    struct _HSD_FObjDesc *next; // 0x00
    u32 length;                 // 0x04
    f32 start_frame;            // 0x08
    u8 track;                   // 0x0C
    u8 value_flag;              // 0x0D
    u8 tan_flag;                // 0x0E
    u8 pad;                     // 0x0F
    void *buffer;               // 0x10
} HSD_FObjDesc;

struct AOBJ
{
    u32 flags;                // 0x0
    f32 curr_frame;           // 0x4
    f32 rewind_frame;         // 0x8
    f32 end_frame;            // 0xC
    f32 framerate;            // 0x10
    struct _HSD_FObj *fobj;   // 0x14
    struct _HSD_Obj *hsd_obj; // 0x18, points to spline data
};

// 0x20 bytes (MObjInfoInit 0x803fb08c).
struct MOBJ
{
    int *parent;
    u32 rendermode;
    TOBJ *tobj;             // 0x8
    HSD_Material *mat;      // 0xC
    struct _HSD_PEDesc *pe; // 0x10
    AOBJ *aobj;             // 0x14
    struct _HSD_TExpTevDesc *tevdesc; // 0x18
    union _HSD_TExp *texp;            // 0x1C
};

struct JOBJDesc
{
    char *class_name;       // 0x00
    u32 flags;              // 0x04
    struct JOBJDesc *child; // 0x08
    struct JOBJDesc *next;  // 0x0C
    union
    {
        struct _HSD_DObjDesc *dobjdesc;
        struct _HSD_Spline *spline;
        struct _HSD_SList *ptcl;
    } u;                            // 0x10
    Vec3 rotation;                  // 0x14 - 0x1C
    Vec3 scale;                     // 0x20 - 0x28
    Vec3 position;                  // 0x2C - 0x34
    Mtx mtx;                        // 0x38
    struct _HSD_RObjDesc *robjdesc; // 0x3C
};

typedef struct _HSD_TlutDesc
{
    void *lut;
    u32 fmt;
    u32 tlut_name;
    u16 n_entries;
} HSD_TlutDesc;

typedef struct _HSD_TexAnim
{
    struct _HSD_TexAnim *next;
    u32 id; // GXTexMapID
    HSD_AObjDesc *aobjdesc;
    _HSD_ImageDesc **imagetbl;
    struct _HSD_TlutDesc **tluttbl;
    u16 n_imagetbl;
    u16 n_tluttbl;
} HSD_TexAnim;

struct MatAnimDesc
{
    MatAnimDesc *next;
    HSD_AObjDesc *material_aobj;
    HSD_TexAnim *texture_anim;
    int is_render_anim;
};

struct MatAnimJointDesc
{
    MatAnimJointDesc *child;
    MatAnimJointDesc *next;
    MatAnimDesc *matanim;
};

// Binds by tree position: JObj_AddAnimAll (0x80409480) walks this tree in
// lockstep with a JObj tree, so a node drives whichever joint sits where it does.
struct AnimJointDesc
{
    AnimJointDesc *child;
    AnimJointDesc *next;
    void *aobj;      // 0x08, HSD_AObjDesc*
    void *robj_anim; // 0x0c, RObj animation for the same joint
    int flags;       // 0x10, bit 0 is the joint's classical scaling
};

struct WOBJDesc
{
    char *class_name;
    Vec3 pos;                       // 0x04
    struct _HSD_RObjDesc *robjdesc; // 0x10
    WOBJDesc *next;
};

struct COBJDesc
{
    char *class_name;        // 0x00
    u16 flags;               // 0x04
    u16 projection_type;     // 0x06
    u16 viewport_left;       // 0x08
    u16 viewport_right;      // 0x0A
    u16 viewport_top;        // 0x0C
    u16 viewport_bottom;     // 0x0E
    u32 scissor_lr;          // 0x10
    u32 scissor_tb;          // 0x14
    WOBJDesc *eye_desc;      // 0x18
    WOBJDesc *interest_desc; // 0x1C
    f32 roll;                // 0x20
    Vec3 *vector;            // 0x24
    f32 near;                // 0x28
    f32 far;                 // 0x2C
    union
    {
        struct
        {
            f32 fov;
            f32 aspect;
        } perspective;

        struct
        {
            f32 top;
            f32 bottom;
            f32 left;
            f32 right;
        } frustrum;

        struct
        {
            f32 top;
            f32 bottom;
            f32 left;
            f32 right;
        } ortho;
    } projection_param;
};

struct HSD_VtxDescList
{
    GXAttribute attr;             // 0x0
    GXAttributeType attr_type;    // 0x4
    GXComponentContents comp_cnt; // 0x8
    GXComponentType comp_type;    // 0xc
    u8 frac;                      // 0x10
    u16 stride;                   // 0x12
    void *vertex;                 // 0x14
};

struct POBJDesc
{
    char *class_name;
    struct POBJDesc *next;
    struct _HSD_VtxDescList *verts;
    u16 flags;
    u16 n_display;
    u8 *display;
    union
    {
        JOBJDesc *joint;
        void *shape_set;
        void **envelope_p;
    } u;
};
struct POBJ
{
    int parent;                    // 0x0
    POBJ *next;                    // 0x4
    struct HSD_VtxDescList *verts; // 0x8
    u16 flags;                     // 0xC
    u16 n_display;                 // 0xE
    u8 *display;                   // 0x10, u8 primitive, u8 vtxcnt, u16* indices
    union                          // 0x14
    {
        JOBJ *jobj;
        void *shape_set;
        void *envelope_list;
    } u;
    AOBJ *aobj; // 0x18
};

// 0x18 bytes (DObjInfoInit 0x803f4afc). flags bit 0 is DOBJ_HIDDEN; bits 1/2/3
// are the render bucket (OPA/XLU/TEXEDGE), derived from mobj->rendermode by
// DObjLoad (0x803f465c) and tested by HSD_JObjDispSub_ (0x8040f218).
struct DOBJ
{
    int parent;
    DOBJ *next; // 0x04
    MOBJ *mobj; // 0x08
    POBJ *pobj; // 0x0C
    AOBJ *aobj; // 0x10
    u32 flags;  // 0x14
};

struct JOBJ
{
    HSD_Obj object; // 0x0
    JOBJ *sibling;  // 0x08 (this is actually called next)
    JOBJ *parent;   // 0x0C (this is actually called prev)
    JOBJ *child;    // 0x10
    int flags;      // 0x14
    DOBJ *dobj;     // 0x18
    //  union {        // 0x18
    //     void* ptcl;
    //     DOBJ* dobj;
    //     HSD_Spline* spline;
    // } u;
    Vec4 rot;       // 0x1C 0x20 0x24 0x28
    Vec3 scale;     // 0x2C
    Vec3 trans;     // 0x38
    Mtx rotMtx;     // 0x44
    Vec3 *VEC;      // 0x74
    Mtx *MTX;       // 0x78
    AOBJ *aobj;     // 0x7C
    void *robj;     // 0x80
    JOBJDesc *desc; // 0x84
};

struct WOBJ
{
    HSD_Obj parent;
    u32 flags;  // 0x08
    Vec3 pos;   // 0xC
    AOBJ *aobj; // 0x18
    void *robj; // 0x1C
};

struct COBJ
{
    HSD_Obj parent;      // 0x0
    u32 flags;           // 0x08
    f32 viewport_left;   // 0x0C
    f32 viewport_right;  // 0x10
    f32 viewport_top;    // 0x14
    f32 viewport_bottom; // 0x18
    u16 scissor_left;    // 0x1C
    u16 scissor_right;   // 0x1E
    u16 scissor_top;     // 0x20
    u16 scissor_bottom;  // 0x22
    WOBJ *eye;           // 0x24
    WOBJ *interest;      // 0x28
    union
    {
        f32 roll; // 0x28
        Vec3 up;  // 0x28 - 0x34
    } u;
    f32 near; // 0x38
    f32 far;  // 0x3C
    union
    {
        struct
        {
            f32 fov;
            f32 aspect;
        } perspective;

        struct
        {
            f32 top;
            f32 bottom;
            f32 left;
            f32 right;
        } frustrum;

        struct
        {
            f32 top;
            f32 bottom;
            f32 left;
            f32 right;
        } ortho;
    } projection_param;
    u8 projection_type; // 0x50
    Mtx view_mtx;       // 0x54
    AOBJ *aobj;         // 0x84
    Mtx *proj_mtx;      // 0x88
};

struct _HSD_ImageDesc
{
    void *img_ptr;
    u16 width;
    u16 height;
    u32 format;
    u32 mipmap;
    f32 minLOD;
    f32 maxLOD;
};

struct _HSD_Tlut
{
    void *tlut_ptr;
    u32 format;
    u32 gxtlut;
    u16 colorcount;
    u16 x0E;
};

// Per-TObj TEV color/alpha combiner state (TObj+0xA8). MakeColorGenTExp (0x803f5f98)
// compiles the color_a..d / alpha_a..d selectors into a TExp tree; MObjSetupTev
// (0x803faba0) re-materializes the constant/tev0/tev1 GXColors into GX registers each
// frame. Selector bytes are GX_CC_* (color) / GX_CA_* (alpha) verbatim for 0x00-0x0F,
// plus register codes:
//   color_a..d:  0x80=constant(RGB), 0x81/82/83=constant.r/g/b, 0x84=constant.a,
//                0x85=tev0(RGB), 0x86=tev0.a, 0x87=tev1(RGB), 0x88=tev1.a.
//   alpha_a..d:  0x40=constant, 0x43=constant.a, 0x44=tev0.a, 0x45=tev1.a.
// The constant/tev0/tev1 GXColors are plain value fields the texture-anim system never
// touches, so rewriting them recolors a model effect in place (preserve each register's alpha).
struct _HSD_TObjTev
{
    u8 color_op;     // 0x00 GXTevOp
    u8 alpha_op;     // 0x01
    u8 color_bias;   // 0x02 GXTevBias
    u8 alpha_bias;   // 0x03

    u8 color_scale;  // 0x04 GXTevScale
    u8 alpha_scale;  // 0x05
    u8 color_clamp;  // 0x06
    u8 alpha_clamp;  // 0x07

    u8 color_a;      // 0x08 color combiner input selectors (see header comment)
    u8 color_b;      // 0x09
    u8 color_c;      // 0x0A
    u8 color_d;      // 0x0B

    u8 alpha_a;      // 0x0C alpha combiner input selectors
    u8 alpha_b;      // 0x0D
    u8 alpha_c;      // 0x0E
    u8 alpha_d;      // 0x0F

    GXColor constant; // 0x10 primary KColor (recolor target; RGB only - opacity is MObj.mat->alpha)
    GXColor tev0;     // 0x14 secondary blend color (color_a)
    GXColor tev1;     // 0x18 tertiary register (unused by the inhale model)
    uint flags;       // 0x1C color/alpha-tree enable bits (inhale model = 0xC000007F)
};

struct _HSD_LightPoint
{
    f32 cutoff;
    u8 point_func;
    f32 ref_br;
    f32 ref_dist;
    u8 dist_func;
};

struct _HSD_LightPointDesc
{
    f32 cutoff;
    u8 point_func;
    f32 ref_br;
    f32 ref_dist;
    u8 dist_func;
};

struct _HSD_LightSpot
{
    f32 cutoff;
    u8 spot_func;
    f32 ref_br;
    f32 ref_dist;
    u8 dist_func;
};

struct _HSD_LightSpotDesc
{
    f32 cutoff;
    u8 spot_func;
    f32 ref_br;
    f32 ref_dist;
    u8 dist_func;
};

struct _HSD_LightAttn
{
    f32 a0;
    f32 a1;
    f32 a2;
    f32 k0;
    f32 k1;
    f32 k2;
};

struct LObjDesc
{
    char *class_name;               // 0x00
    LObjDesc *next;                 // 0x04
    u16 flags;                      // 0x08 -- low 2 bits = LOBJ_AMBIENT/INFINITE/POINT/SPOT (asserted by
                                    //         LObjLoad), bits 2-5 = DIFFUSE/SPECULAR/ALPHA/HIDDEN
    u16 attnflags;                  // 0x0A -- bit 0 LOBJ_RAW_PARAM: u.attn is a raw 6-float block,
                                    //         otherwise u.spot is a 3-word computed block
    GXColor color;                  // 0x0C
    struct _HSD_WObjDesc *position; // 0x10 -- loaded as WObj if flags & 3 != 0
    struct _HSD_WObjDesc *interest; // 0x14 -- loaded as WObj if flags & 3 == 3 (LOBJ_SPOT)
    union
    {
        void *p;
        f32 *shininess;
        void *point;
        void *spot;
        void *attn;
    } u;
};
struct LightAnim
{
    LightAnim *next;
    struct _HSD_AObjDesc *aobjdesc;
    struct _HSD_WObjAnim *position_anim;
    struct _HSD_WObjAnim *interest_anim;
};
struct LightGroup
{
    LObjDesc *lobj_desc;
    LightAnim *anim;
};

struct LOBJ
{
    HSD_Obj parent;    // 0x00
    u16 flags;         // 0x08
    u16 priority;      // 0x0A
    struct LOBJ *next; // 0x0C
    GXColor color;     // 0x10
    GXColor hw_color;  // 0x14
    WOBJ *position;    // 0x18
    WOBJ *interest;    // 0x1C
    union
    {
        _HSD_LightPoint point;
        _HSD_LightSpot spot;
        _HSD_LightAttn attn;
    } u;
    f32 shininess;
    Vec3 lvec;
    AOBJ *aobj; // 0x48
    u32 id;     // 0x4c, GXLightID
    // GXLightObj lightobj;      //0x50
    u32 spec_id; // 0x90 GXLightID
    // GXLightObj spec_lightobj; //0x94
};

// GXFogType values written to HSD_Fog.type / HSD_FogDesc.type and passed
// straight to GXSetFog. PERSP_* use perspective-projected Z; the suffix is the
// density curve. City Trial ships GX_FOG_PERSP_LIN.
typedef enum GXFogType
{
    GX_FOG_NONE          = 0,
    GX_FOG_PERSP_LIN     = 2,  // linear falloff (City Trial default)
    GX_FOG_PERSP_EXP     = 4,  // exponential
    GX_FOG_PERSP_EXP2    = 5,  // exponential-squared (thick wall)
    GX_FOG_PERSP_REVEXP  = 6,  // reverse exponential (dense near camera)
    GX_FOG_PERSP_REVEXP2 = 7,  // reverse exponential-squared
} GXFogType;

struct HSD_Fog
{
    HSD_Obj parent;
    u32 type;          // 0x08 GXFogType word (City Trial = 2, GX_FOG_PERSP_LIN)
    HSD_Fog *fog_adj;  // 0x0C range-adjust descriptor (NULL on City Trial)
    f32 start;         // 0x10
    f32 end;           // 0x14
    GXColor color;     // 0x18
    struct AOBJ *aobj; // 0x1C
    f32 scale;         // 0x20 HSD_FogSet emits GXSetFog with (end * scale); 1.0 on CT
};

// AreaLightData: 0x2C-byte directional-light record stored inside a stage's
// sky-preset entry (preset+0x18). Interpolated per-frame by AreaLight_Lerp
// (lbarealight.c) into the live AreaLight at GrObj+0x718.
//
// Color fields are packed RGBA8888 u32 (high byte = R) - that's how
// GXColor_Lerp (0x80079c04) loads/stores them with single lwz/stw.
typedef struct AreaLightData
{
    u32 header;          // 0x00 metadata (raw-copied)
    u8 unk_04;           // 0x04 raw-copied
    u8 flags;            // 0x05 bits 0+1: validity (asserted), bit 2: lerp enable
    u16 unk_06;          // 0x06 raw-copied
    u32 color;           // 0x08 RGBA diffuse light color (interpolated)
    u32 hw_color;        // 0x0C RGBA specular/hardware light color (interpolated)
    Vec3 direction;      // 0x10 light direction (Vec3 lerp, 12 bytes)
    u8 unk_1C[3];        // 0x1C raw-copied (HSD attn type/flags)
    u8 intensity;        // 0x1F byte interpolation if flags bit 2
    u32 attn_param_0;    // 0x20 HSD light attn/spot params
    u32 attn_param_1;    // 0x24
    u32 attn_param_2;    // 0x28
} AreaLightData;

// Live AreaLight runtime object built by AreaLight_Create. One per stage,
// stored at GrObj+0x718. Per-frame lerp target for sky-preset color/direction.
// Bit 0x80 of byte +0x38 is the visibility bit toggled by the preset's
// light_vis_flag (preset+0x44 bit 0).
struct AreaLight
{
    HSD_ClassInfo *parent;  // 0x00 vtable, from class-table 1336(r13)
    void *class_ptr;        // 0x04 the class param passed to AreaLight_Create
    u32 header;             // 0x08 copied from src+0x00
    u8 unk_0C;              // 0x0C copied from src+0x04
    u8 flags;               // 0x0D copied from src+0x05
    u16 x0E;            // 0x0E
    u32 color;              // 0x10 RGBA from src+0x08 (lerp target each frame)
    u32 hw_color;           // 0x14 RGBA from src+0x0C
    Vec3 direction;         // 0x18 from src+0x10
    u8 unk_24[3];           // 0x24 from src+0x1C
    u8 intensity;           // 0x27 from src+0x1F
    u32 attn_param_0;       // 0x28 from src+0x20
    u32 attn_param_1;       // 0x2C from src+0x24
    u32 attn_param_2;       // 0x30 from src+0x28
    u32 extra;              // 0x34 r5 to AreaLight_Create (=0 from stage init)
    u32 x38;      // 0x38 bit 0x80 = visibility (set by light_vis_flag)
};
typedef struct AreaLight AreaLight;

struct HSD_FogDesc
{
    u32 type;                   // 0x00 GXFogType word (copied to HSD_Fog.type; CT = 2)
    HSD_FogDesc *fog_adj;       // 0x04
    f32 start;                  // 0x08
    f32 end;                    // 0x0C
    GXColor color;              // 0x10
    struct _HSD_AObjDesc *aobj; // 0x14
};

struct JOBJSet
{
    JOBJDesc *jobj;
    AnimJointDesc **animjoint;
    MatAnimJointDesc **matanimjoint;
    void **shapeaninjoint;
};

struct HSD_SObjDesc
{
    JOBJSet **jobjset;
    COBJDesc **cobjdesc;
    LightGroup *lights;
    HSD_FogDesc **fog;
};

typedef struct HSD_GObjInitData {
    u8 p_link_max;
    u8 gx_link_max;
    u8 proc_pri_max;
    void* funcs; 
    u64* unk;
} HSD_GObjInitData;

/*** Static Variables ***/
static GOBJ ***stc_gobj_lookup = (GOBJ ***)(0x805de334);                        //
static u8 *stc_gobj_proc_num = (u8 *)0x804ce382;                                // number of elements in the below array
static GOBJProc ***stc_gobjproc_lookup = (GOBJProc ***)0x804D7840;              // array of gobj procs ptrs
static GOBJProc **stc_gobjproc_cur = (GOBJProc **)0x804d7838;                   // current gobj proc being processed
static u32 *stc_gobjproc_updateidx_cur = (u32 *)0x804d783c;                     // update index of the current gobj proc being processed. this is compared to
static HSD_GObjInitData *stc_gobj_init_data = (HSD_GObjInitData *)0x8058c190;
static float *stc_cobj_aspect = (float *)0x805deb20;

// HSD hardware-light slot table - 9 LOBJ pointers, rebuilt every frame by
// HSD_LObjSetCurrentAll (0x803ff570). Slots 0..7 hold positional/directional
// LOBJs in active-list insertion order; slot 8 is the dedicated ambient slot.
// Reading is stale-by-one-frame for any per-frame think hook (the table is
// populated during GX rendering), but the LOBJ pointers themselves are stable
// across the lookup.
#define HSD_LOBJ_HW_SLOT_AMBIENT 8
static struct LOBJ **stc_lobj_hw_slot_table = (struct LOBJ **)0x805899B0;

/*** Functions ***/
int JObj_GetWorldPosition(JOBJ *source, Vec3 *add, Vec3 *dest); // 0x80053f34
void JObj_SetMtxDirtySub(JOBJ *jobj); // 0x8040d92c
void JObj_SetupMtxSub(JOBJ *jobj); // 0x8040d6b4
void HSD_JObjMakeMatrix(JOBJ *jobj); // 0x80408884
JOBJ *JObj_Alloc(); // 0x8040c774
JOBJ *JObj_LoadJoint(JOBJDesc *joint); // 0x8040afe8
// The JObj class's load method: fills jobj from desc, kept at JOBJ.desc, and loads its children.
int JObjLoad(JOBJ *jobj, JOBJDesc *desc); // 0x8040add4
void JObj_RemoveAll(JOBJ *joint); // 0x8040b920
void JObj_Remove(JOBJ *joint); // 0x8040b718
void JObj_AddNext(JOBJ *parent, JOBJ *child); // 0x8040bac8 - appends child to the end of parent's child list
JOBJ *JObj_GetPrev(JOBJ* joint); // 0x8040bc14
void JObj_SetFlags(JOBJ *joint, int flags); // 0x8040bd64
void JObj_SetFlagsAll(JOBJ *joint, int flags); // 0x8040be00
void JObj_ClearFlags(JOBJ *joint, int flags); // 0x8040c0d8
void JObj_ClearFlagsAll(JOBJ *joint, int flags); // 0x8040c174
void HSD_JObjMakePositionMtx(JOBJ *jobj, Mtx *vmtx, Mtx *pmtx); // 0x8040f00c - vmtx x joint matrix into pmtx, billboarded per the joint's billboard flags
void JObj_SetFrameAndRate(JOBJ *j, int frame, float rate); // 0x80138ba4
void JObj_ForEachAnim(JOBJ *joint, int unk, ForEachAnimFlag flags, void *cb, int argkind, ...); // 0x803fcdb8 - argkind specifies how to pop args off the va_list
void JObj_Anim(JOBJ *joint); // 0x8040a1a0
void JObj_AnimAll(JOBJ *joint); // 0x8040a304
void JObj_AddAnim(JOBJ *joint, void *animjoint, void *matanimjoint, void *shapeanimjoint); // 0x80409340
void JObj_AddAnimAll(JOBJ *joint, void *animjoint, void *matanimjoint, void *shapeanimjoint); // 0x80409480
void JObj_RemoveAnim(JOBJ *joint); // 0x80408e74
void JObj_RemoveAnimAll(JOBJ *joint); // 0x80408edc
void JObj_ReqAnim(JOBJ *joint, float frame); // 0x80409250
void JObj_ReqAnimByFlags(JOBJ *joint, int flags, float frame); // 0x80408f00
void JObj_ReqAnimAll(JOBJ *joint, float unk); // 0x80409274
void JObj_ReqAnimAllByFlags(JOBJ *joint, int flags, float frame); // 0x80408f88
void JObj_SetAllMOBJFlags(JOBJ *joint, int flags); // 0x80052fb8
int JObj_CheckAObjPlaying(JOBJ *joint); // 0x800547e0
void JObj_SetAllAOBJRateByFlags(JOBJ *j, int flags, float rate); // 0x800550bc
// Binds an HSD_FigaTree to a JObj tree: walks it in preorder against the
// FigaTree's per-node track-count table (+0x0c, 0xff-terminated), building an
// AObj per node and one FObj per track out of the 0x0c-byte records at +0x10.
// The end frame at +0x08 becomes every AObj's.
void JObj_AddFigaTreeAnim(JOBJ *joint, void *figatree);  // 0x8006e2c0
float FigaTree_GetEndFrame(void *figatree);              // 0x8006e58c, +0x08, or 0 when NULL
// Despite the name, the engine's general "place a JObj in the world": builds a
// TRS matrix from an orthonormal basis (right = forward x up, normalized) scaled
// by scale, translates it to pos and stamps it on the joint.
void gmLanMenu_Scale3DObject(f32 scale, JOBJ *joint, Vec3 *forward, Vec3 *up, Vec3 *pos); // 0x80054414
void JObj_SetAllAOBJLoopByFlags(JOBJ *j, int flags); // 0x800550f4
void JObj_DispAll(JOBJ *joint, Mtx *vmtx, int rendermode, int mobj_flags); // 0x8040a7b8
void JObj_AttachPosition(JOBJ *to_attach, JOBJ *attach_to); // 0x80055c14
void JObj_AttachPositionRotation(JOBJ *to_attach, JOBJ *attach_to); // 0x80055c7c
void JObj_AddSetAnim(JOBJ *jobj, int anim_id, JOBJSet *set, float frame, float rate);                                        // 0x80138b10
// Drops every animation, adds the set's first and poses it on `frame`.
void JObj_AddSetAnim0_SetFrameAndRate(JOBJ *jobj, JOBJSet *set, float frame, float rate); // 0x80114d9c
void JObj_Detach(JOBJ *to_attach); // 0x80055d10 - removes every JObj constraint RObj, as added by JObj_AttachPosition*
void JObj_ResetFromDesc(JOBJ *jobj, JOBJDesc *desc); // 0x800551a8 - copies desc rotation, scale and position onto jobj; not recursive
void JObj_RemoveAnimByFlags(JOBJ *, int); // 0x80408b54
void JObj_RemoveAnimAllByFlags(JOBJ *, int); // 0x80408bd0
void JObj_SetAllMObjAnimFrameAndRate(JOBJ *j, int frame, float rate); // 0x80115174
void JObj_PauseAllTObjAnimFrame(JOBJ *j, int frame); // 0x80115114
void ROBJ_RemoveAll(void *robj); // 0x80419b2c
void AOBJ_ReqAnim(AOBJ *aobj, float unk); // 0x803fb280
void AOBJ_StopAnim(AOBJ *aobj); // 0x803fb2c0
void AOBJ_SetRate(AOBJ *aobj, float rate); // 0x803fda3c
void AOBJ_SetFlags(AOBJ *aobj, int flags); // 0x803fb190
void AOBJ_ClearFlags(AOBJ *aobj, int flags); // 0x803fb1ac
void DOBJ_SetFlags(DOBJ *dobj, int flags); // 0x803f43f8
void DOBJ_ClearFlags(DOBJ *dobj, int flags); // 0x803f4410
void DOBJ_AddAnimAll(DOBJ *dobj, void *matanim, void *textureanim); // 0x803f44c0
void MOBJ_AddAnim(MOBJ *mobj, MatAnimDesc *matanim); // 0x803f9954
void TOBJ_AddAnim(TOBJ *tobj, void *textureanim); // 0x803f4bdc
void TOBJ_RemoveAnimAll(TOBJ *tobj); // 0x803f4b7c
COBJ *COBJ_Alloc(); // 0x80402f64
COBJ *COBJ_LoadDesc(COBJDesc *cobj); // 0x804031ec
COBJ *COBJ_LoadDescSetAspect(COBJDesc *cobj); // 0x80066df8
void COBJ_Init(COBJ *cobj, COBJDesc *cobj_desc); // 0x80402fc0 - re-initializes a live cobj using its descriptor
void CObjThink_Common(GOBJ *gobj); // 0x8042a29c
int CObj_SetCurrent(COBJ *cobj); // 0x80401150
void CObj_SetEraseColor(int r, int g, int b, int a); // 0x8040f884
void CObj_EraseScreen(COBJ *cobj, GXBool color_update_enable, GXBool alpha_update_enable, GXBool depth_update_enable); // 0x804006d4
void CObj_RenderGXLinks(GOBJ *gobj, int render_mode); // 0x8042a0b4
void CObj_EndCurrent(); // 0x804016c4
void CObj_SetOrtho(COBJ *cobj, float top, float bottom, float left, float right); // 0x80402f08
void CObj_SetViewport(COBJ *cobj, float left, float right, float top, float bottom); // 0x80402de8
void CObj_SetScissor(COBJ *cobj, u16 left, u16 right, u16 top, u16 bottom); // 0x80402d3c
void CObj_SetEyePosition(COBJ *cobj, Vec3 *eye_pos); // 0x804018ac, writes the eye WObj (+0x24) position
void COBJ_GetEyePosition(COBJ *cobj, Vec3 *eye_pos); // 0x80401840
void CObj_SetInterest(COBJ *cobj, Vec3 *pos);        // 0x804017d4, writes the interest WObj (+0x28) position
void CObj_SetUp(COBJ *cobj, Vec3 *up);                // 0x80402138
void CObj_SetRoll(COBJ *cobj, float roll); // 0x804027cc
void CObj_SetAspect(COBJ *cobj, float aspect); // 0x80402c94
void CObj_Release(COBJ *cobj); // 0x80403324
void CObj_Destroy(COBJ *cobj); // 0x8042a2e4 - drops a reference; the last one releases and frees the cobj
COBJ *COBJ_GetCurrent(); // 0x80402f5c
void COBJ_GetEyeVector(COBJ *cobj, Vec3 *eye_vec); // 0x80401918
void COBJ_GetInterest(COBJ *cobj, Vec3 *interest); // 0x80401768
float COBJ_GetEyeDistance(COBJ *cobj); // 0x80401a9c
void CObj_SetMtxDirty(COBJ *cobj); // 0x804026a8
void COBJ_GetViewingMtx(COBJ *cobj, Mtx *out); // 0x80402704
Mtx *COBJ_SetupViewingMtx(COBJ *cobj); // 0x80401014
int COBJ_IsPositionVisible(COBJ *cobj, Vec3 *pos, Vec3 *out, int check_scissor); // 0x80067438
Vec3 *COBJ_ProjectPoint(COBJ *cobj, Vec3 *pos, Vec3 *out, int unk); // 0x800644ac - returns the out vector, weird
GOBJ *GObj_Create(int entity_class, int p_link, int p_priority); // 0x80428f28
void GObj_Destroy(GOBJ *gobj); // 0x80428f64
JOBJ *GObj_GetJObjIndex(GOBJ *gobj, int index); // 0x80055AF0, depth-first traversal to get Nth JOBJ from gobj->hsd_object
// Moves an existing GObj onto another p_link at the given priority.
// Relinking a HUD element to GAMEPLINK_PAUSEHUD is what keeps it drawn while paused.
void GObj_SetPLink(GOBJ *gobj, int p_link, u8 p_priority); // 0x804293f4
void GObj_AddGXLink(GOBJ *gobj, void *cb, int gx_link, int gx_pri); // 0x80429690
void GObj_DestroyGXLink(GOBJ *gobj); // 0x80429774 - unlinks from its gx list and resets gx_link to 0xff
GOBJProc *GObj_AddProc(GOBJ *gobj, void *callback, int priority); // 0x804288a4
void GObj_RemoveProc(GOBJ *gobj); // 0x80428ad8
void GObj_AddObject(GOBJ *gobj, u8 obj_kind, void *object); // 0x80429c14
void GObj_FreeObject(GOBJ *gobj); // 0x80429cb0 - destroys hsd_object by obj_kind and clears it
void GObj_AddUserData(GOBJ *gobj, int userDataKind, void *destructor, void *userData); // 0x80429d0c
void GOBJ_InitCamera(GOBJ *gobj, void *cb, int gx_pri); // 0x80429728
void GObj_UpdateAll();                                          // 0x80429ed4
void JObj_GX(GOBJ *gobj, int pass); // 0x8042a258
int HSD_GetRenderPass(int pass); // 0x8042a0a0 - maps a gx callback pass to the JObj_DispAll render mode
void LObj_GX(GOBJ *gobj, int pass); // 0x8042a22c
void Fog_GX(GOBJ *gobj, int pass); // 0x800dbf84
LOBJ *LObj_LoadDesc(void *lobjdesc); // 0x80400238
LOBJ *LObj_CreateAll(void **lobjdesc); // 0x80057468
int LObj_GetPosition(LOBJ *lobj, Vec3 *pos); // 0x803ffcec
void LObj_SetPosition(LOBJ *lobj, Vec3 *pos); // 0x803ffc64
int LObj_GetInterest(LOBJ *lobj, Vec3 *pos); // 0x803ffdb4
void LObj_SetInterest(LOBJ *lobj, Vec3 *pos); // 0x803ffd2c
void LObj_ReqAnimAll(LOBJ *lobj, float frame); // 0x803fdf28
void LObj_AnimAll(LOBJ *lobj); // 0x803fde54
void LObj_DeleteCurrentAll(int unk); // 0x803ff318
void LObj_RemoveAll(LOBJ *lobj); // 0x803ffa40
HSD_Fog *Fog_LoadDesc(HSD_FogDesc *fogdesc); // 0x8041b3c0
void FogRelease(HSD_Fog *fog); // 0x8041b850 - class release: drops fog_adj and the AObj, does not free fog

// Emits GXSetFog/GXSetFogColor from fog, reading the current COBJ near/far, or
// GX_FOG_NONE when fog is NULL. The current fog is left as it was.
void HSD_FogSet(HSD_Fog *fog); // 0x8041b0fc
void HSD_FogSetCurrent(HSD_Fog *fog); // 0x8041b0d0 - records fog as the current fog, then HSD_FogSet
HSD_Fog *HSD_FogGetCurrent(void); // 0x8041b0f4 - the fog last passed to HSD_FogSetCurrent

AreaLight *AreaLight_Create(void *class_ptr, AreaLightData *src, u32 extra);  // 0x80079428
AreaLight *AreaLight_Create_Default(AreaLightData *src);                       // 0x8007a4d0 - class=0, extra=0
void AreaLight_Lerp(AreaLight *start, AreaLightData *target, AreaLight *dest, float ratio); // 0x800797a8
void AreaLight_BroadcastVisFlag(void *class_ptr, u8 vis_flag);                 // 0x80079948 - preset+0x44 bit 0 -> +0x38 bit 0x80
void AreaLight_StageInit(GOBJ *grobj_or_gobj);                                 // 0x800ef618 - stack-builds default, stores at +0x718
void AreaLight_LerpToLive(GOBJ *grobj, AreaLight *start, AreaLightData *target, float ratio); // 0x800ef864 - Sky_Update adapter

// Stage HSD light chain - real GX hardware lights, separate from AreaLight.
// Loaded from gr_data->stage_resource[+0x14] (LObjDesc**, NULL-terminated).
void Light_GX(GOBJ *gobj, int pass);                  // 0x800d5fb0 - wraps LObj_GX
void Light_CreateForStage(void *grobj_or_ctx);        // 0x800d5fd0 - class=1 GObj at stage init
DOBJ *JObj_GetDObj(JOBJ *jobj); // 0x8040bca0 - NULL for particle and spline joints
void HSD_MObjSetAlpha(MOBJ *mobj, float alpha); // 0x803fad80
void HSD_MObjAnim(MOBJ *mobj); // 0x803f9ebc
void GObj_CopyGXPri(GOBJ *target, GOBJ *source); // 0x80429bac - moves target onto source's gx_link and gx_pri, drawn right after source
#endif