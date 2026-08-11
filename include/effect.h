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
    JOBJDesc *jointdesc;    // 0x00 model joint template (-> HSD_JObjLoadJoint)
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
    s32   life;       // 0x0c lifetime counter (init -1 = unset/infinite). NOTE this also drives
                      //      the effect's animation playback - pinning it every frame freezes the
                      //      anim, so it can't be used to extend a finite effect; re-spawn instead.
    u8    _pad10[8];  // 0x10
    u8    flags;      // 0x18 state bits (a bit is set when scene mode in [7,11))
    u8    _pad19[15]; // 0x19
    Vec3  pos;        // 0x28 position/velocity offset (init {0,0,0})
    f32   scale;      // 0x34 scale/rate (init 1.0)
    u8    _pad38[88]; // 0x38
    void *aux;        // 0x90 optional heap block freed by the destructor (0x80233ddc)
};

// id (group*10000+entry) -> per-kind model descriptor (0x80235190).
EffectModelDesc *Effect_GetModelData(int id);

// Universal effect spawn. Only the low word of the {r3, r4} handle is declared -
// it is enough to tell success (0 on failure) and every vanilla caller discards
// it. Returns 0 while effects are globally suppressed (*(u32*)0x805dd8b8 != 0)
// or the create gate rejects the scene.
//
//   parent      - owning GObj. May be NULL: the owner block is then skipped and
//                 the owner-player index preseeds to 5 ("none").
//   id          - group*10000 + entry.
//   efgroup     - EfGroup bucket. Asserts on -1, so pass a live one; a rider's
//                 RiderData.efgroup works.
//   anchor_mode - selects which varargs the placement resolver reads. It fills
//                 its descriptor purely from the varargs, never from `parent`.
//
// Mode 1 takes one vararg, a `void (*)(void *node)` post-spawn callback invoked
// with the spawn node. It skips the joint-attach path, so no follow proc is
// installed and the model root's SRT is the caller's to write. This is the
// world-anchored path; no variant takes a raw Vec3.
//
// Modes 200..220 are joint-follow. 218 is the mouth anchor Rider_StartInhale
// uses: Effect_SpawnSync(rd->gobj, 0x3a982, rd->efgroup, 218, jobj, jobj, ply).
//
// A joint-followed effect gets priority-11 procs that rewrite the root's SRT
// from the target joint each frame (defeat by zeroing the anchor flags at
// Effect+0x1e) and arm the anim loop after the intro. Mode 1 installs neither,
// so a mod-spawned effect must arm its own looping
// (JObj_SetAllAOBJLoopByFlags(root, 0xffff) + Effect.life = 1) or the animation
// stalls on the intro's last frame.
u32 Effect_SpawnSync(GOBJ *parent, int id, int efgroup, int anchor_mode, ...); // 0x80236c40

// Spawn-node fields the anchor-mode-1 callback needs.
#define EFFECT_NODE_GOBJ 0x5c  // node -> the effect GObj

// NEVER GObj_Destroy a spawned effect. The spawn node keeps pointing at the GObj
// and the per-node kill destroys node+0x5c unconditionally when the group is
// retired, double-freeing it - the GObj returns to the free list, gets handed
// out again, and trips an unrelated GObj_AddUserData assert. To retire a mode-1
// effect, hide its model tree (JObj_SetFlagsAll(root, JOBJ_HIDDEN)) and let the
// engine own the lifetime.

// Effect-instance manager. The per-group EffectModelDesc* table at +0x24 is the
// only source for model-effect descriptor lookup.
static void **const gEffectMgr = (void **)0x8055D7A0;

// Bank-install registry (4x u32[64] + generator-template count/array), written by
// psInitDataBanks. Consumed only by the point-particle path, independent of the
// gEffectMgr+0x24 model-descriptor table.
static void **const efGlobal = (void **)0x8058C208;

#endif // KAR_H_EFFECT
