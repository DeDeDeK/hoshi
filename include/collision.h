#ifndef KAR_H_COLL
#define KAR_H_COLL

#include "datatypes.h"
#include "obj.h"

typedef enum CollShapeKind
{
    Mp_CollShapeKind_Sphere,
} CollShapeKind;

// One contact the pushback resolved against a map triangle.
typedef struct mpCollContact
{
    int   tri_id;       // 0x00, index into GrCollParam.tri. Not range checked on this path
    float t;            // 0x04, sweep sort key, ascending; smaller is the earlier impact
    Vec3  pos;          // 0x08, world contact point
    Vec3  from_center;  // 0x14, pos - the swept sphere's centre at t
    int   x20;          // 0x20
    float x24;          // 0x24
    int   x28;          // 0x28, caller tag; the machine's wall picker takes a wall when this is 0
} mpCollContact;        // 0x2c

// One pushback substep's result. Which list a contact lands in is decided by
// GrCollTri.kind bits 0..2, baked per triangle, not by a runtime normal test.
typedef struct mpCollRec
{
    u8 best_kind;               // 0x00, 0 none, 1 GrCFK_Under, 2 GrCFK_Wall, 4 GrCFK_Top
    u8 hit_mask;                // 0x01, those bits OR'd for everything recorded this substep
    u8 x2[2];
    mpCollContact *under;       // 0x04
    int under_num;              // 0x08
    mpCollContact *wall;        // 0x0c
    int wall_num;               // 0x10
    mpCollContact *top;         // 0x14
    int top_num;                // 0x18
} mpCollRec;                    // 0x1c

// mpCollInfo - floor/wall/ceiling collision results sub-struct. Allocated
// internally by mpColl_Create via mpColl_AllocCollInfo (0x802416cc), pointed to
// by CollData+0x44. mpColl_SetDefaultParams (0x802460d4) clears it and then runs
// mpColl_UpdateCollision (0x802485e0) for up to 10 substeps, so the counts below
// describe the frame's own pushback and survive until the next step clears them.
// A body is touching a wall this frame exactly when wall_rec_num is non-zero;
// wall_recs[i]->wall names the triangle it was stopped by and where.
typedef struct mpCollInfo
{
    mpCollRec  rec[10];         // 0x000, one per substep
    int        cur_step;        // 0x118
    mpCollRec *under_recs[10];  // 0x11c
    int        under_rec_num;   // 0x144
    mpCollRec *wall_recs[10];   // 0x148
    int        wall_rec_num;    // 0x170
    mpCollRec *top_recs[10];    // 0x174
    int        top_rec_num;     // 0x19c
    mpCollRec  prev_pass_rec;   // 0x1a0, the previous pass's last substep
    int        capacity;        // 0x1bc, 1 - one contact per kind per substep
    u8         x1c0[0x1d0 - 0x1c0];
    int        contact_tri_id;  // 0x1d0, cached triangle id into GrCollParam.tri, -1 = none.
                                //        destroyBigStar (0x800d7b8c) is the only confirmed
                                //        reader: it takes tri[id].record as the instance the
                                //        body is in contact with.
} mpCollInfo;                   // >= 0x1d4

// Map collision. One vertex pool and one triangle array hold both baked terrain
// and every placed prop's triangles, each prop owning a contiguous slice through
// its instance record. Allocated exactly-sized by grColl_Alloc (0x800d6dcc) into
// the HSD heap and freed by grColl_Free (0x800d7060), field by field - so
// swapping in a mod-owned buffer leaks the original and hands a foreign pointer
// to OSFreeToHeap. Raise a coll_max count before allocation to get slack instead.
typedef struct GrCollRecord GrCollRecord;

typedef struct GrCollVtx
{
    Vec3 pos;   // 0x00 world space, baked from the model at load
    Vec3 prev;  // 0x0C previous frame, for moving-platform delta math
} GrCollVtx;    // 0x18

typedef struct GrCollTri
{
    Vec3 *v0;              // 0x00 into GrCollParam.vtx
    Vec3 *v1;              // 0x04
    Vec3 *v2;              // 0x08
    Vec3 normal;           // 0x0C outward unit normal
    Vec3 aabb_center;      // 0x18
    Vec3 aabb_half;        // 0x24
    u32 kind;              // 0x30 bits 0..2 response category, bits 4..11 ground type
    u32 flags;             // 0x34 gameplay flags
    GrCollRecord *record;  // 0x38 owning instance record, never NULL
    u8 state;              // 0x3C
    u8 pad3d[3];
} GrCollTri;               // 0x40

#define GRCOLL_KIND_GROUNDTYPE_SHIFT 4
#define GRCOLL_KIND_GROUNDTYPE_MASK  0xFF0
// Bits 0..2 are the baked surface category. Every query ANDs its own mask
// against them first, so these decide whether a triangle can be stood on,
// walled off, or hit at all - not a runtime normal test. A runtime-built
// surface must set them to match its own facing.
#define GRCOLL_KIND_CATEGORY         0x7
#define GRCOLL_KIND_UNDER            0x1 // ground: what a rider stands and lands on
#define GRCOLL_KIND_WALL             0x2
#define GRCOLL_KIND_TOP              0x4 // ceiling
#define GRCOLL_FLAG_ROUGH            0x00000003 // gets an extruded prism entry
#define GRCOLL_FLAG_MOVING           0x00000020 // re-baked per frame by grColl_RebakeMovingRecord, and
                                                // selects the impact path that runs the collider through
                                                // the record's prev_inv/world - which rewrites a
                                                // synthetic frame delta, so clear it to force a
                                                // fabricated collider through unchanged
#define GRCOLL_STATE_SURFACE_PARAM   0x20 // set on every triangle the builder emits
#define GRCOLL_STATE_COLLIDABLE      0x40 // cleared by grScene_SetInstanceColl on a break
#define GRCOLL_STATE_DEGENERATE      0x80 // normal could not be computed; never collides
// Clearing GRCOLL_STATE_COLLIDABLE hides a triangle from every query, but not
// from entities holding a cached triangle id - those keep reading its fields
// until their next query. A live City Trial triangle reads state 0x60.

struct GrCollRecord
{
    JOBJ *jobj;           // 0x00 joint the moving rebake re-transforms from
    GrCollVtx *vtx;       // 0x04 this instance's slice of the vertex pool
    int vtx_num;          // 0x08
    GrCollTri *tri_begin; // 0x0C first triangle of this instance's slice
    int tri_num;          // 0x10 how many, NOT an end pointer
    Vec3 aabb_center;     // 0x14 broadphase box for the whole slice
    Vec3 aabb_half;       // 0x20
    Mtx world;            // 0x2C jobj's world matrix as of the last bake
    Mtx prev_inv;         // 0x5C inverse of the previous frame's world matrix;
                          //      the swept-sphere query multiplies the collider
                          //      through prev_inv then world, so both must be
                          //      valid (identity for a synthetic record)
    int desc_kind;        // 0x8C the yakumono break dispatch fires on == 3
    GOBJ *yaku_gobj;      // 0x90 owning prop, NULL for terrain
    u8 flags;             // 0x94 bit 7 = moved during the last rebake
    u8 x95[3];
};                        // 0x98

typedef struct GrCollParam
{
    GrCollVtx *vtx;               // 0x54
    int vtx_num;                  // 0x58
    GrCollTri *tri;               // 0x5C
    int tri_num;                  // 0x60
    GrCollRecord *record;         // 0x64
    int record_num;               // 0x68
    Vec3 *aux;                    // 0x6C zone vertex pool, from GrCollisionNode.zone_vtx
    int aux_num;                  // 0x70
    void *zone;                   // 0x74 0x140 stride
    int zone_num;                 // 0x78
    void *rough;                  // 0x7C 0x1C stride: {GrCollTri*, Vec3 center, Vec3 half}
    int rough_num;                // 0x80 one entry per triangle with flags & 3
    GrCollRecord **moving_record; // 0x84 brute-forced by every query
    int moving_record_num;        // 0x88
    void **moving_zone;           // 0x8C
    int moving_zone_num;          // 0x90
    void **moving_rough;          // 0x94
    int moving_rough_num;         // 0x98
} GrCollParam;                    // 0x48

// One entry of GrObj.joint_table, the stage model's flat joint registry.
typedef struct GrJoint
{
    JOBJ *jobj;       // 0x00
    JOBJDesc *desc;   // 0x04
} GrJoint;            // 0x08

typedef struct CollData
{
    struct CollData *next;         // 0x0, linked list (head at r13+0x7E4 = 0x805DD8C4)
    GOBJ *g;                       // 0x4, owner GObj
    Vec3 pos;                      // 0x8, current world position
    Vec3 pos_delta;                // 0x14, pos - prev_pos, computed by mpColl_Update
    Vec3 prev_pos;                 // 0x20, previous frame position
    int x2c;                       // 0x2c
    int x30;                       // 0x30
    int x34;                       // 0x34
    int x38;                       // 0x38
    int x3c;                       // 0x3c
    int x40;                       // 0x40
    mpCollInfo *coll_info;         // 0x44, floor/wall/ceiling collision results
    int zone_hit[20];              // 0x48, GrCollParam.zone indices this body is inside
    int zone_hit_num;              // 0x98
    u8 moving_zone_hit[20][0x20];  // 0x9c, {zone_idx, sub_idx, ...} per moving zone hit
    int moving_zone_hit_num;       // 0x31c
    int x320;                      // 0x320
    int x324;                      // 0x324
    int x328;                      // 0x328
    int x32c;                      // 0x32c
    int x330;                      // 0x330
    int x334;                      // 0x334
    int x338;                      // 0x338
    CollShapeKind coll_shape_kind; // 0x33c
    struct CollShapeData           // 0x340
    {
        u8 x0[0xC];
        Vec3 direction;  // 0x0C
        u8 x18[0x18];
        float radius;    // 0x30, sphere radius (lerp endpoint)
        float radius2;   // 0x34, second sphere radius; mpColl_GetSphereRadius lerps +0x30 <-> +0x34
        Vec3 scale;      // 0x38
    } *shape_data;
    float radius;                  // 0x344, collision sphere radius
    int param;                     // 0x348, mode/flag parameter from mpColl_Init
    u8 flags;                      // 0x34c, bit 7 set by mpColl_SetFlag
    u8 x34d;                       // 0x34d
    u8 x34e;                       // 0x34e
    u8 x34f;                       // 0x34f
    int x350;                      // 0x350
    int x354;                      // 0x354
    int x358;                      // 0x358
    int x35c;                      // 0x35c
    int x360;                      // 0x360
    int x364;                      // 0x364
    int x368;                      // 0x368
    int x36c;                      // 0x36c
    int x370;                      // 0x370
    int x374;                      // 0x374
    int x378;                      // 0x378
    int x37c;                      // 0x37c
    int x380;                      // 0x380
    int x384;                      // 0x384
    int x388;                      // 0x388
    int x38c;                      // 0x38c
    int x390;                      // 0x390
    int x394;                      // 0x394
    int x398;                      // 0x398
    int x39c;                      // 0x39c
    int x3a0;                      // 0x3a0
    int x3a4;                      // 0x3a4
    int x3a8;                      // 0x3a8
    int x3ac;                      // 0x3ac
    int x3b0;                      // 0x3b0
    int x3b4;                      // 0x3b4
    int x3b8;                      // 0x3b8
    int x3bc;                      // 0x3bc
    int x3c0;                      // 0x3c0
    int x3c4;                      // 0x3c4
    int x3c8;                      // 0x3c8
    int x3cc;                      // 0x3cc
    int x3d0;                      // 0x3d0
    int x3d4;                      // 0x3d4
    int x3d8;                      // 0x3d8
    int x3dc;                      // 0x3dc
    int x3e0;                      // 0x3e0
    int x3e4;                      // 0x3e4
    int x3e8;                      // 0x3e8
    int x3ec;                      // 0x3ec
    int x3f0;                      // 0x3f0
    int x3f4;                      // 0x3f4
    int x3f8;                      // 0x3f8
    int x3fc;                      // 0x3fc
} CollData;

// Allocates CollData from the pool, links it into the global list and creates
// its coll_info and shape_data.
CollData *mpColl_Create(void);                    // 0x80245b4c
// Sets position, direction and scale, then inits the subsystems.
void mpColl_Init(CollData *cd, int type, Vec3 *pos, Vec3 *dir, Vec3 *extents, int param, float radius, float f2); // 0x80245c10
void mpColl_Reinit(CollData *cd, Vec3 *pos, Vec3 *dir); // 0x80245db0. Re-initializes with new position/direction
void mpColl_Destroy(CollData *cd);                // 0x80245ed0. Frees shape_data and its two sub-allocations (+0x44, +0x48), unlinks from global list
// Per-frame: updates position, computes the delta, updates the shape.
void mpColl_Update(CollData *cd, Vec3 *pos, Vec3 *dir, Vec3 *extents, int r7); // 0x80245f70
void mpColl_SetDefaultParams(CollData *cd);       // 0x802460d4. Sets default collision check parameters
void mpColl_UpdateShapeExtents(CollData *cd, Vec3 *pos); // 0x8024625c. Updates shape extents from scale
void mpColl_SetFlag(CollData *cd, int value);     // 0x80247e2c. Sets/clears bit 7 of flags byte at +0x34C
CollData *mpColl_GetFirstCollObj(void);           // 0x802414d4. Returns head of global CollData linked list
int mpColl_UpdateCollision(void);                 // 0x802485e0. Recursive pushback substep; records its contacts into coll_info

// Four byte-identical wrappers over Raycast_Do(&GrObj.coll, start, end, kind_mask,
// filter, out_pos), differing only in the two constants they pass. Each returns the
// nearest triangle id along the segment or -1, and writes the hit point to out_pos.
//
// kind_mask is ANDed with GrCollTri.kind in grColl_RayVsTri (0x800d95dc+0x118)
// before the state bits are looked at, and kind bits 0..2 are the baked surface
// category - 1 GrCFK_Under, 2 GrCFK_Wall, 4 GrCFK_Top. So a wrapper's mask decides
// which surfaces exist as far as its caller is concerned: Raycast_Ground cannot
// see a wall, and Raycast_Wall cannot see the floor in front of it.
//
// The filter argument, when non-zero, additionally drops any triangle that is
// neither ground (kind & 1) nor tagged with flags bit 0x8000.
int Raycast_Any(Vec3 *start, Vec3 *end, Vec3 *out_pos);       // 0x800d1a54. mask 7, filter 0 - every surface
int Raycast_Ground(Vec3 *start, Vec3 *end, Vec3 *out_pos);    // 0x800d1ac4. mask 1, filter 0 - GrCFK_Under only
int Raycast_Wall(Vec3 *start, Vec3 *end, Vec3 *out_pos);      // 0x800d1b34. mask 2, filter 0 - GrCFK_Wall only
int Raycast_AnyTagged(Vec3 *start, Vec3 *end, Vec3 *out_pos); // 0x800d1ba4. mask 7, filter 1
// The only range check on a triangle id anywhere in the collision system, and
// every ground / landing / shadow consumer runs it. The wall sweep and the
// surface-property lookups do not, and index GrCollParam.tri by the raw id.
int PointCollision_EnsureIDValid(int triangle_id); // 0x800d1838. Returns 0 if valid, 1 if outside [0, tri_num)
void PointCollision_GetNormalByID(int triangle_id, Vec3 *out_normal); // 0x800d1860. Looks up triangle normal (stride 0x40)
int grGetGroundTypeFromTriangleID(int triangle_id); // 0x800cec28. Returns ground type from triangle ID

// Two hemisphere caps and a cylinder spanning p0..p1; a bare sphere if p0 == p1.
void Debug_DrawCapsule(Vec3 *p0, Vec3 *p1, float radius, void *mat_color, void *amb_color); // 0x8007d988
// One lit GX_QUADS quad from 4 corners and a shared normal.
void Debug_DrawQuad(Vec3 corners[4], Vec3 *normal, void *mat_color, void *amb_color);       // 0x8007e61c
// Higher-level callers (all debug): Trigger_DrawCollision (0x800826a4), Hit_DrawCollision
// (0x80082838), Hurt_DrawHurtbox (0x8008252c), Map_Debug_DrawCollisionMode (0x800a3ab0).

#endif
