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

// Universal effect spawn. Returns the low word of an {r3, r4} handle, 0 on failure,
// while effects are suppressed (*(u32*)0x805dd8b8 != 0) or when the create gate
// rejects the scene. parent may be NULL. efgroup asserts on -1; a rider's
// RiderData.efgroup works. anchor_mode picks which varargs place it: mode 1 takes a
// `void (*)(void *node)` post-spawn callback and installs no follow or anim-loop proc,
// so the caller writes the root SRT and arms its own loop
// (JObj_SetAllAOBJLoopByFlags(root, 0xffff) + Effect.life = 1). Modes 200..220 follow
// a joint (218 is the mouth anchor Rider_StartInhale uses).
u32 Effect_SpawnSync(GOBJ *parent, int id, int efgroup, int anchor_mode, ...); // 0x80236c40

// Spawn-node fields the anchor-mode-1 callback needs.
#define EFFECT_NODE_GOBJ 0x5c  // node -> the effect GObj

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
