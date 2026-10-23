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
// by CollData+0x44. mpColl_ProcessMapColl (0x802460d4) clears it and then runs
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
                                //        destroyBigStar (0x800d7b8c) takes tri[id].record as
                                //        the instance the body is in contact with
    u8         x1d4[0x1e0 - 0x1d4];
} mpCollInfo;                   // 0x1e0


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
    Vec3 prev;  // 0x0C pos before the last rebake; triangle boxes cover both
} GrCollVtx;    // 0x18

// Queries read the stored normal and box and never recompute them; only the builder
// and the moving rebake derive the normal (grColl_ComputeTriNormal). Both narrowphase
// tests are one-sided, so the normal must face where bodies come from.
typedef struct GrCollTri
{
    Vec3 *v0;              // 0x00 into GrCollParam.vtx
    Vec3 *v1;              // 0x04
    Vec3 *v2;              // 0x08
    Vec3 normal;           // 0x0C unit, normalize((v2 - v0) x (v1 - v0))
    Vec3 aabb_center;      // 0x18 tight box over v0..v2; every test pads it by 0.01
    Vec3 aabb_half;        // 0x24
    u32 kind;              // 0x30 bits 0..2 response category, bits 4..11 ground type
    u32 flags;             // 0x34 gameplay flags
    GrCollRecord *record;  // 0x38 owning instance record, never NULL
    u8 state;              // 0x3C
    u8 pad3d[3];
} GrCollTri;               // 0x40

// Bits 0..2 are the baked surface category. Every query ANDs its own mask against them
// first; nothing tests the normal against world up, so an UNDER triangle facing sideways
// or down is still floor. The one world-Y rule: mpColl_ComputeSlideDelta flattens a slide
// along a WALL with |normal.y| <= 0.9. A runtime-built surface must set them itself.
#define GRCOLL_KIND_CATEGORY         0x7
#define GRCOLL_KIND_UNDER            0x1 // ground: what a rider stands and lands on
#define GRCOLL_KIND_WALL             0x2
#define GRCOLL_KIND_TOP              0x4 // ceiling

// GrCollTri.kind bits 4..11.
#define GRCOLL_KIND_GROUNDTYPE_SHIFT 4
#define GRCOLL_KIND_GROUNDTYPE_MASK  0xFF0
typedef enum GrGroundType
{
    GRCOLL_GROUNDTYPE_ASPHALT    = 5,
    GRCOLL_GROUNDTYPE_GRIND_RAIL = 26,
    GRCOLL_GROUNDTYPE_RAIL_DASH  = 27,
    GRCOLL_GROUNDTYPE_WATER_RAIL = 28,
    GRCOLL_GROUNDTYPE_SEA        = 29, // City Trial's sea
    GRCOLL_GROUNDTYPE_BARRIER    = 30, // City Trial's two invisible barriers ringing the city
} GrGroundType;

#define GRCOLL_FLAG_ROUGH            0x00000003 // gets an extruded prism entry
#define GRCOLL_FLAG_REFLECT_MASK     0x0000001C // index into StageNode+0x20, grGetUnkFromTriangleID (0x800cecd4)
// The record is rebaked per frame and carries bodies standing on it. It also selects
// the impact path that rewrites the collider's frame delta from the record's
// matrices, so clear it to force a fabricated collider through unchanged.
#define GRCOLL_FLAG_MOVING           0x00000020
#define GRCOLL_FLAG_FORCE_MASK       0x00001C00 // index into StageNode+0x40, grGetForceFromTriangleID
#define GRCOLL_FLAG_TAGGED           0x00008000 // the only non-floor triangles Raycast_AnyTagged sees

// Clearing GRCOLL_STATE_COLLIDABLE hides a triangle from every query, but not
// from entities holding a cached triangle id - those keep reading its fields
// until their next query. A live City Trial triangle reads state 0x60.
#define GRCOLL_STATE_SURFACE_PARAM   0x20 // set on every triangle the builder emits
#define GRCOLL_STATE_COLLIDABLE      0x40 // cleared by grScene_SetInstanceColl on a break
#define GRCOLL_STATE_DEGENERATE      0x80 // normal could not be computed; never collides

// A query's moving pass tests aabb_center/aabb_half, then tri_begin[0..tri_num). A
// record listed only in GrCollParam.moving_record, in no window, is never rebaked: its
// owner keeps the triangles, boxes and matrices current itself.
struct GrCollRecord
{
    JOBJ *jobj;           // 0x00 joint the moving rebake re-transforms from; must be non-NULL
    GrCollVtx *vtx;       // 0x04 this instance's slice of the vertex pool
    int vtx_num;          // 0x08
    GrCollTri *tri_begin; // 0x0C first triangle of this instance's slice
    int tri_num;          // 0x10 how many, NOT an end pointer
    Vec3 aabb_center;     // 0x14 broadphase box for the whole slice
    Vec3 aabb_half;       // 0x20
    Mtx world;            // 0x2C jobj's world matrix as of the last bake
    Mtx prev_inv;         // 0x5C inverse of last frame's world; map sweeps push bodies by world * prev_inv
    int desc_kind;        // 0x8C the yakumono break dispatch fires on == 3
    GOBJ *yaku_gobj;      // 0x90 owning prop, NULL for terrain
    u8 flags;             // 0x94 bit 7 = moved during the last rebake
    u8 x95[3];
};                        // 0x98

// One layout for three roles in GrObj: the capacities coll_max, the live arrays coll,
// and per-node windows into them. coll's counts are the build's fill cursor, so anything
// appended at runtime must advance coll's count too, or the next prop attach writes
// over it.
typedef struct GrCollParam
{
    GrCollVtx *vtx;               // 0x54
    int vtx_num;                  // 0x58
    GrCollTri *tri;               // 0x5C
    int tri_num;                  // 0x60 the bound PointCollision_EnsureIDValid checks
    GrCollRecord *record;         // 0x64
    int record_num;               // 0x68
    Vec3 *zone_vtx;               // 0x6C zone vertex pool, from GrCollisionNode.zone_vtx
    int zone_vtx_num;             // 0x70
    void *zone;                   // 0x74 0x140 stride
    int zone_num;                 // 0x78
    void *rough;                  // 0x7C 0x1C stride: {GrCollTri*, Vec3 center, Vec3 half}
    int rough_num;                // 0x80 one entry per triangle with flags & 3
    GrCollRecord **moving_record; // 0x84 records with GRCOLL_FLAG_MOVING, brute-forced by every query
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

#define COLL_PROBE_NUM 3

// A point mpColl_RoughProbe (0x8024c5b4) sweeps from its previous to its current world
// position, given in the owner's body frame like CollShapeData.offset.
typedef struct CollProbe
{
    Vec3 offset; // 0x00
    int tri;     // 0x0C, -1 = none found yet
} CollProbe;     // 0x10

typedef struct CollData
{
    struct CollData *next;         // 0x0, linked list (head at r13+0x7E4 = 0x805DD8C4)
    GOBJ *g;                       // 0x4, owner GObj
    Vec3 pos;                      // 0x8, current world position
    Vec3 pos_delta;                // 0x14, pos - prev_pos, computed by mpColl_Update
    Vec3 prev_pos;                 // 0x20, previous frame position
    Vec3 sweep_center;             // 0x2c, box around this step's swept sphere, for the moving pass
    Vec3 sweep_half;               // 0x38
    mpCollInfo *coll_info;         // 0x44, floor/wall/ceiling collision results
    int zone_hit[20];              // 0x48, GrCollParam.zone indices this body is inside
    int zone_hit_num;              // 0x98
    u8 zone_cross[20][0x20];       // 0x9c, {int zone, int face group, Vec3 point} per zone face the sweep crossed
    int zone_cross_num;            // 0x31c
    int rail_idx;                  // 0x320, rail found by the rail sweep, -1 = none
    int x324;                      // 0x324, rail related
    float x328;                    // 0x328, rail related
    float x32c;                    // 0x32c, rail related
    float x330;                    // 0x330, rail related, copied from x32c
    float yaku_break_speed_cap;    // 0x334, speed cap after ramming a breakable prop
    u8 req_yaku_break_effect : 1;  // 0x338, 0x80, set by GrYaku_TestImpactBreak; consumed by Machine_CollideWithBreakable
    CollShapeKind coll_shape_kind; // 0x33c
    struct CollShapeData           // 0x340
    {
        u8 x0[0xC];
        Vec3 direction;  // 0x0C
        u8 x18[0x18];
        float radius;    // 0x30, sphere radius (lerp endpoint); written only by mpColl_Init
        float radius2;   // 0x34, second sphere radius; mpColl_GetSphereRadius lerps +0x30 <-> +0x34
        Vec3 offset;     // 0x38, sphere center in the owner's body frame; written only by mpColl_Init
        void *x44;       // 0x44, 0x16c-byte pool object, freed by mpColl_Destroy
        CollProbe *probes; // 0x48, COLL_PROBE_NUM of them, freed by mpColl_Destroy; set only on machines
    } *shape_data;
    float radius;                  // 0x344, collision sphere radius
    int param;                     // 0x348, mode/flag parameter from mpColl_Init
    u8 flags;                      // 0x34c, bit 2 = this collider may break scene objects
                                   //        (mpColl_SceneObjBreakGate, 0x80241574), bit 7 set by
                                   //        mpColl_SetFlag
    u8 x34d;                       // 0x34d
    u8 x34e;                       // 0x34e
    u8 x34f;                       // 0x34f
} CollData;                        // 0x350


// Allocates CollData from the pool, links it into the global list and creates
// its coll_info and shape_data.
CollData *mpColl_Create(GOBJ *owner);             // 0x80245b4c. Stores owner at +0x04; every
                                                  // break-credit lookup resolves a player from it
// Sets the body frame (pos, the z axis dir and the y axis up), the sphere's body-frame
// offset and both shape radii, then inits the subsystems. f2 goes to cd->radius.
void mpColl_Init(CollData *cd, Vec3 *pos, Vec3 *dir, Vec3 *up, Vec3 *offset, int param, float radius, float f2); // 0x80245c10
void mpColl_Reinit(CollData *cd, Vec3 *pos, Vec3 *dir); // 0x80245db0. Re-initializes with new position/direction
void mpColl_Destroy(CollData *cd);                // 0x80245ed0. Frees shape_data and its x44/x48, unlinks from global list
// Per-frame: places the sphere at shape_data->offset in the body frame and computes the
// delta. radius goes to cd->radius, radius2 to shape_data->radius2 unless it is -1.
void mpColl_Update(CollData *cd, Vec3 *pos, Vec3 *dir, Vec3 *up, int arg5, float radius, float radius2); // 0x80245f70
// The frame's map collision pass with the default kind mask and 10 substeps; runs
// mpColl_UpdateCollision and fills coll_info.
void mpColl_ProcessMapColl(CollData *cd);         // 0x802460d4
// Writes the owner position back from the resolved sphere center, minus shape_data->offset.
void mpColl_UpdateShapeExtents(CollData *cd, Vec3 *pos); // 0x8024625c
// Sets the shape_data->probes offsets; machines' collision init calls it after mpColl_Init.
void mpColl_SetProbes(CollData *cd, Vec3 *probe0, Vec3 *probe1, Vec3 *probe2); // 0x802463a8
void mpColl_SetFlag(CollData *cd, int value);     // 0x80247e2c. Sets/clears bit 7 of flags byte at +0x34C
// Sets/clears flags bit 2. With it set, the map sweep runs the yakumono break
// dispatch for this collider, so it can break City Trial props; force is
// CollData.radius (+0x344) x impactSpeed^2 against the prop's HP. Vanilla sets
// it on machines only - mpColl_Create clears it.
void mpColl_SetSceneObjBreak(CollData *cd, int enabled); // 0x80247e58
CollData *mpColl_GetFirstCollObj(void);           // 0x802414d4. Returns head of global CollData linked list
// Recursive pushback substep; records its contacts into coll_info.
int mpColl_UpdateCollision(CollData *cd, GrCollParam *coll, int step, int arg4, int arg5, int arg6,
                           int arg7, int arg8, int arg9, int arg10, int arg11, int arg12); // 0x802485e0
// The zone index of the first zone the body is in whose kind (+0x24, low 25 bits) is 25,
// moving zones included; -1 when none. Machine_CheckFallDeath hands it to Machine_SetFallDead.
int mpColl_GetDeadZoneIndex(CollData *cd);         // 0x80247fac
// The same first-zone-of-a-kind scan, for kinds 18 (catch) and 22 (cannon); -1 when none.
int mpColl_GetCatchZoneIndex(CollData *cd);        // 0x802471c0
int mpColl_GetCannonZoneIndex(CollData *cd);       // 0x80247348

// Four wrappers over Raycast_Do(&GrObj.coll, start, end, kind_mask, filter, out_pos).
// Each returns the nearest triangle id along the segment or -1 and writes the hit to
// out_pos. kind_mask is ANDed with the triangle's GRCOLL_KIND_CATEGORY first, so
// Raycast_Ground cannot see a wall and Raycast_Wall cannot see the floor in front of it.
// A non-zero filter also drops any non-ground triangle without GRCOLL_FLAG_TAGGED. A mask
// carrying ground type 30 in bits 4..11 drops GRCOLL_GROUNDTYPE_BARRIER; none of these do.
int Raycast_Any(Vec3 *start, Vec3 *end, Vec3 *out_pos);       // 0x800d1a54. mask 7, filter 0 - every surface
int Raycast_Ground(Vec3 *start, Vec3 *end, Vec3 *out_pos);    // 0x800d1ac4. mask 1, filter 0 - GrCFK_Under only
int Raycast_Wall(Vec3 *start, Vec3 *end, Vec3 *out_pos);      // 0x800d1b34. mask 2, filter 0 - GrCFK_Wall only
int Raycast_AnyTagged(Vec3 *start, Vec3 *end, Vec3 *out_pos); // 0x800d1ba4. mask 7, filter 1
// Sphere sweeps through grColl_SweptSphereQuery with its moving pass on. Each returns
// the hit triangle id or -1.
int SphereCast_Any(Vec3 *start, Vec3 *end, Vec3 *out_pos, Vec3 *out2, float radius);    // 0x800d1c14. mask 7
int SphereCast_Ground(Vec3 *start, Vec3 *end, Vec3 *out_pos, Vec3 *out2, float radius); // 0x800d1ca8. mask 1
// The only range check on a triangle id anywhere in the collision system, and
// every ground / landing / shadow consumer runs it. The wall sweep and the
// surface-property lookups do not, and index GrCollParam.tri by the raw id.
int PointCollision_EnsureIDValid(int triangle_id); // 0x800d1838. Returns 0 if valid, 1 if outside [0, tri_num)
void PointCollision_GetNormalByID(int triangle_id, Vec3 *out_normal); // 0x800d1860. Looks up triangle normal (stride 0x40)
int PointCollision_IsMovingTri(int triangle_id); // 0x800d18f0, GRCOLL_FLAG_MOVING, no range check
// Moving-platform carry for a body on triangle_id: the distance point moved, its direction
// in out_dir, forward and up turned alike. A static triangle returns 0 and copies them.
float grColl_GetPlatformCarry(GrCollParam *gcp, Vec3 *point, int triangle_id, Vec3 *out_dir, Vec3 *forward,
                              Vec3 *up, Vec3 *out_forward, Vec3 *out_up); // 0x800d8d44
float PointCollision_GetPlatformCarry(Vec3 *point, int triangle_id, Vec3 *out_dir, Vec3 *forward, Vec3 *up,
                                      Vec3 *out_forward, Vec3 *out_up); // 0x80247d70, on GrObj.coll
// out = world * (prev_inv * in): where a point riding the record last frame is now.
void grColl_CarryPoint(Mtx prev_inv, Mtx world, Vec3 *in, Vec3 *out); // 0x802440ac
int grGetGroundTypeFromTriangleID(int triangle_id); // 0x800cec28. Returns ground type from triangle ID
// Ground-type row +0x00 of the GrCommon.dat table at r13+0x5e8 (0x1c stride, 47 rows).
float grGetTractionFromTriangleID(int triangle_id); // 0x800cec80
int grGetDustFromGroundType(int ground_type);        // 0x800cec44, the ground table's dust effect index (+0x10)
// The wall push bonus from the triangle's GRCOLL_FLAG_FORCE_MASK index, or 0 without
// GRCOLL_STATE_SURFACE_PARAM.
float grGetForceFromTriangleID(int triangle_id); // 0x800ced6c
void grColl_GetTriVtx(GrCollParam *gcp, int tri, Vec3 *v0, Vec3 *v1, Vec3 *v2); // 0x800d7118
// out = normalize((v2 - v0) x (v1 - v0)); returns 0 and a zero normal when the
// cross product is shorter than 1e-5, which the builder marks GRCOLL_STATE_DEGENERATE.
int grColl_ComputeTriNormal(Vec3 *v0, Vec3 *v1, Vec3 *v2, Vec3 *out); // 0x8024347c
void grColl_ExpandMinMax(Vec3 *min, Vec3 *max, Vec3 *p); // 0x8024a608
// {center, half} from a min/max pair, the box layout every triangle and record uses.
void grColl_MinMaxToBounds(float *out_center_half, Vec3 *min, Vec3 *max); // 0x8024a704

// One lit GX_QUADS quad from 4 corners and a shared normal.
void Debug_DrawQuad(Vec3 corners[4], Vec3 *normal, void *mat_color, void *amb_color);       // 0x8007e61c
// Higher-level callers (all debug): Trigger_DrawCollision (0x800826a4), Hit_DrawCollision
// (0x80082838), Hurt_DrawHurtbox (0x8008252c), Map_Debug_DrawCollisionMode (0x800a3ab0).

#endif
