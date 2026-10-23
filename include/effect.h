#ifndef KAR_H_EFFECT
#define KAR_H_EFFECT

#include "structs.h"
#include "datatypes.h"
#include "obj.h"

// Model-effect layer: standalone GObjs (entity_class 25, p_link 16) each carrying an
// HSD JObj model tree (inhale whirlwind, hit sparks, charge flashes, etc.), spawned by
// Effect_SpawnSync (0x80236c40) and built by EffectModel_CreateGObj (0x8023ccb4).
// Distinct from the point-particle pool (Particle/ptclGen). Effect IDs are
// decimal-packed: id = group*10000 + entry (group = id/10000).

// Per-kind model descriptor (0x14 bytes), looked up by Effect_GetModelData from an
// effect id (group in [24,37)). EffectModel_CreateGObj reads only +0x00 (the joint
// template). In the resident banks only group 24 is populated, from EfCommon.dat's
// efModelData symbol.
struct EffectModelDesc
{
    JOBJDesc *jointdesc;    // 0x00 model joint template (-> JObj_LoadJoint)
    void    **anim_set0;    // 0x04 NULL-terminated anim-tree array, or NULL
    void    **anim_set1;    // 0x08 second anim-tree array, or NULL
    void     *anim_set2;    // 0x0c shape-anim set (NULL in EfCommon)
    void     *list_head[2]; // 0x10 self-referencing list-head sentinel (next/prev)
};

// A loaded effect bank's <name>_ref symbol: the bank's effect-ID range key plus
// the manifest of IDs it provides. Not a {count, entries[]} table - the first
// word is the base id, not a count.
typedef struct EffectBankRef
{
    u32 base_id; // 0x00 group*10000 (range key)
    u32 ids[];   // 0x04 decimal effect-ID manifest
} EffectBankRef;

// GObj-userdata effect-instance state (GObj+0x2c), written by the instance init at
// 0x80233e24 as Effect_Init(effect, kind, gobj). Only the named fields are known; the
// full extent past +0x90 is unknown.
struct Effect
{
    GOBJ *gobj;       // 0x00 owning GObj
    int   kind;       // 0x04 effect kind/ID (group*10000+entry)
    void *list_node;  // 0x08 per-group active-list node
    s32   life;       // 0x0c lifetime counter (init -1 = unset/infinite). It also drives the
                      //      animation, so pinning it freezes the anim.
    u8    _pad10[8];  // 0x10
    u8    flags;      // 0x18 state bits (a bit is set when scene mode in [7,11))
    u8    _pad19[15]; // 0x19
    Vec3  pos;        // 0x28 position/velocity offset (init {0,0,0})
    f32   scale;      // 0x34 scale/rate (init 1.0)
    u8    _pad38[88]; // 0x38
    void *aux;        // 0x90 optional heap block freed by the destructor (0x80233ddc)
};

// id (group*10000+entry) -> per-kind model descriptor.
EffectModelDesc *Effect_GetModelData(int id); // 0x80235190

// Effect ids, group*10000 + entry. Groups below 24 are particles.
typedef enum EffectID
{
    EFFECT_ID_DUST_PUFF = 20067,          // ground dust event actors kick up as they run
    EFFECT_ID_WATER_SPLASH = 20118,
    EFFECT_ID_GROUND_BURST = 50007,       // the pillar event spawns both at a pillar's base, scale 3
    EFFECT_ID_GROUND_BURST_ALT = 50012,
    EFFECT_ID_INHALE_WHIRLWIND = 240002,  // Rider_StartInhale's whirlwind
    EFFECT_ID_QUAKE_MODEL = 240018,       // the shake model every quake id builds
    // Camera quakes: a QUAKE_KIND_JOBJ source removed with the effect. The PLY ones shake
    // only the given player's view. Small/medium/large are priorities 1-3, the rest 1.
    EFFECT_ID_QUAKE_PLY_SMALL = 370074,   // EFFECT_ANCHOR_QUAKE_PLY
    EFFECT_ID_QUAKE_PLY_MEDIUM = 370075,
    EFFECT_ID_QUAKE_PLY_LARGE = 370076,
    EFFECT_ID_QUAKE_PLY_LOOP = 370077,    // until the effect is retired
    EFFECT_ID_QUAKE_PLY_TIMED = 370078,   // EFFECT_ANCHOR_QUAKE_PLY_TIMED
    EFFECT_ID_QUAKE_SMALL = 370079,       // EFFECT_ANCHOR_QUAKE
    EFFECT_ID_QUAKE_MEDIUM = 370080,
    EFFECT_ID_QUAKE_LARGE = 370081,
    EFFECT_ID_QUAKE_LOOP = 370082,        // until the effect is retired
    EFFECT_ID_QUAKE_TIMED = 370083,       // EFFECT_ANCHOR_QUAKE_TIMED
} EffectID;

// Effect_SpawnSync's anchor_mode: the varargs that place the effect. Modes 200..220
// follow a joint (218 is Rider_StartInhale's mouth anchor).
typedef enum EffectAnchorMode
{
    EFFECT_ANCHOR_DETACHED = 1,           // (void (*cb)(void *node)), run after the spawn
    EFFECT_ANCHOR_WORLDPOS = 100,         // particle ids: (Vec3 *pos)
    EFFECT_ANCHOR_WORLDPOS_SCALED = 103,  // particle ids: (Vec3 *pos, float *scale)
    // Quake ids. A NULL pos shakes with no distance falloff.
    EFFECT_ANCHOR_QUAKE_PLY = 506,        // (int ply, int unused, Vec3 *pos)
    EFFECT_ANCHOR_QUAKE_PLY_TIMED = 507,  // (int ply, int unused, Vec3 *pos, int frames)
    EFFECT_ANCHOR_QUAKE = 510,            // (int unused, Vec3 *pos)
    EFFECT_ANCHOR_QUAKE_TIMED = 511,      // (int unused, Vec3 *pos, int frames)
} EffectAnchorMode;

// Returns the spawn handle, 0 on failure. parent may be NULL; efgroup asserts on -1.
// EFFECT_ANCHOR_DETACHED installs no follow or anim procs: the caller places the root,
// arms its loop (JObj_SetAllAOBJLoopByFlags) and runs JObj_AnimAll on it every frame.
u64 Effect_SpawnSync(GOBJ *parent, int id, int efgroup, EffectAnchorMode anchor_mode, ...); // 0x80236c40
// The particle body of EFFECT_ANCHOR_WORLDPOS. Returns the spawn node or NULL.
void *Effect_SpawnPtclAtPos(int id, int efgroup, Vec3 *pos); // 0x8023bd90
// Handle -> spawn node, NULL once the effect is gone; prev may be NULL.
void *Effect_FindNode(u64 handle, void **prev);                         // 0x802342f8
// Retune or move a quake effect's source through its handle.
void Effect_SetQuakeAmplitude(u64 handle, float amp_right, float amp_up); // 0x8023687c
void Effect_SetQuakePosition(u64 handle, Vec3 *pos);                    // 0x802368d0

// Spawn-node fields the EFFECT_ANCHOR_DETACHED callback needs.
#define EFFECT_NODE_GOBJ     0x5c // node -> the effect GObj
#define EFFECT_NODE_QUAKE_ID 0x60 // node -> a quake effect's source id

// Never GObj_Destroy a spawned effect: the per-node kill destroys node+0x5c again
// when the group is retired. Hide its model tree (JObj_SetFlagsAll(root, JOBJ_HIDDEN))
// instead.

// EfGroup buckets. A particle generator created while a group is current (a
// weapon's state animation runs under weapon+0x114) belongs to that group.
int  Effect_AllocEfGroup(void);             // 0x802364e0
void Effect_SetCurrentEfGroup(int efgroup); // 0x802369e0
void Effect_ClearCurrentEfGroup(void);      // 0x802369f0

// Effect-instance manager. The per-group EffectModelDesc* table at +0x24 is the
// only source for model-effect descriptor lookup.
static void **const stc_effect_mgr = (void **)0x8055D7A0;

// Bank-install registry (4x u32[64] + generator-template count/array), written by
// psInitDataBanks. Consumed only by the point-particle path, independent of the
// stc_effect_mgr+0x24 model-descriptor table.
static void **const stc_ef_global = (void **)0x8058C208;

#endif // KAR_H_EFFECT
