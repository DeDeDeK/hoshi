#ifndef KAR_H_EFFECT
#define KAR_H_EFFECT

#include "structs.h"
#include "datatypes.h"
#include "obj.h"

// Model-effect layer: standalone GObjs (entity_class 25, p_link 16) each carrying an HSD
// JObj model tree - the inhale whirlwind, hit sparks, charge flashes, etc. Spawned by
// Effect_SpawnSync (0x80236c40), built by EffectModel_CreateGObj (0x8023ccb4). The
// point-particle pool (Particle/ptclGen, exhaust/sparkles) is a separate subsystem.
//
// Effect IDs are decimal-packed: id = group*10000 + entry (group = id/10000).

// Per-kind model descriptor. Effect_GetModelData decodes an id, bounds-checks
// group in [24,37), and returns *(table + entry*8) from the per-group descriptor table at
// gEffectMgr+0x24+group*4. EffectModel_CreateGObj reads only +0x00 (the joint template).
// In the resident banks only group 24 is populated, from the EfCommon.dat efModelData
// symbol (via Effect_InstallModelData 0x8023515c). 0x14 bytes.
struct EffectModelDesc
{
    JOBJDesc *jointdesc;    // 0x00 model joint template (-> HSD_JObjLoadJoint)
    void    **anim_set0;    // 0x04 NULL-terminated anim-tree array, or NULL
    void    **anim_set1;    // 0x08 second anim-tree array, or NULL
    void     *anim_set2;    // 0x0c shape-anim set (NULL in EfCommon)
    void     *list_head[2]; // 0x10 self-referencing list-head sentinel (next/prev)
};

// A loaded effect bank's <name>_ref symbol: the bank's decimal effect-ID range key plus
// the manifest of effect IDs it provides. Parsed by psInitDataBanks (0x8042a734) into the
// efGlobal arrays; matched against the paired <name>_form's first word. NOTE this is NOT a
// {u32 count; entry[]} table - the first word is the base id, not a count.
typedef struct EffectBankRef
{
    u32 base_id; // 0x00 group*10000 (range key)
    u32 ids[];   // 0x04 decimal effect-ID manifest
} EffectBankRef;

// GObj-userdata effect-instance state (GObj+0x2c), written by the instance init at
// 0x80233e24 as Effect_Init(effect, kind, gobj). Only the confirmed fields are named; the
// full extent past +0x90 is not pinned.
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

// Effect-instance manager (SDA global, built by Effect_InitObjAllocs 0x802332c4). The
// per-group EffectModelDesc* table is at +0x24 (read by Effect_GetModelData); model-effect
// descriptor lookup reads only this table.
static void **const gEffectMgr = (void **)0x8055D7A0;

// Bank-install registry (4x u32[64] + generator-template count/array), written by
// psInitDataBanks from each loaded bank's _ptcl/_ref/_form symbols. Consumed ONLY by the
// point-particle path - independent of the gEffectMgr+0x24 model-descriptor table.
static void **const efGlobal = (void **)0x8058C208;

#endif // KAR_H_EFFECT
