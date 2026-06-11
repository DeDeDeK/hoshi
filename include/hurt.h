#ifndef KAR_H_HURT
#define KAR_H_HURT

#include "datatypes.h"
#include "obj.h"

typedef enum HurtKind
{
    HURTKIND_RIDER,
    HURTKIND_1,
    HURTKIND_MACHINE,
    HURTKIND_3,
    HURTKIND_POWERUP,
    HURTKIND_5,
    HURTKIND_STAGE,
} HurtKind;

typedef struct HurtDesc
{
    int joint_idx;
    int x4;
    float scale;
    Vec3 offset;
} HurtDesc;

typedef struct HurtData
{
    HurtKind kind;       // 0x0
    HurtDesc *desc;      // 0x4
    int region_count;    // 0x8, collision region tier: stages=2, riders/machines=4, enemies=2 (meteor=8)
    void *regions;       // 0xc, collision region array (allocated from object pool, stride 0xC8)
    int sub_region_count; // 0x10, sub-region count tier
    void *sub_regions;   // 0x14, sub-hurt data array (stride 0x44 per entry, indexed by Machine_ApplyHurt)
    void *x18;           // 0x18
    int hitcoll_log_idx; // 0x1c, index of strongest hit in HitCollData::log
    int attacker_kind;   // 0x20, HurtKind of the attacker that dealt the strongest hit
    float kb_mag;        // 0x24, knockback magnitude - non-zero triggers hit reaction
    float dmg_taken;     // 0x28, cumulative damage taken this frame
    float max_single_hit; // 0x2c, largest single-hit damage this frame
    Vec3 contact_point;  // 0x30, collision contact position
    Vec3 knockback_dir;  // 0x3c, knockback direction vector (from HitColl_CalcKnockbackDir)
    Vec3 attacker_pos;   // 0x48, attacker's position at time of collision
    int attacker_flags;  // 0x54, attacker's hit flags (bits from trigger param +0x30)
    int x58;             // 0x58
    int x5c;             // 0x5c
    int x60;             // 0x60
    int x64;             // 0x64
    void *pos_tracker;   // 0x68, position-tracking object (for velocity computation in HitColl_GetDamageDealt)
    float radius;        // 0x6c, hurtbox radius (set per-frame by HurtData_UpdatePerFrame)
    Vec3 center_pos;     // 0x70, hurtbox center position (set per-frame)
    int x7c;             // 0x7c
    float dmg_multiplier; // 0x80, damage scaling factor (from MachineData+0x4EC for machines)
    int x84;             // 0x84
    struct
    {
        int kind;         // 0x88, 0 = vulnerable, 1 = invincible, 2 = intangible
        void (*on_damage_callback)(void *, void *); // 0x8c, called by HitColl_SetDamageLog when damage is applied. Set during Machine_InitHurtData / CityItem_InitHurtData
        int x90;          // 0x90
        int intang_timer; // 0x94, intangibility timer (counts down, prevents damage while > 0)
        int invuln_timer; // 0x98, invulnerability timer (counts down, prevents damage while > 0)
    } vuln;
    u8 flags2;           // 0x9c, bit 7 cleared each frame by HurtData_UpdateVulnState; bits 4-5 set by UpdatePerFrame
} HurtData;

typedef struct HitCollData
{
    int x0; // 0x0
    int x4; // 0x4
    u8 active; // 0x8, set to 1 by HitColl_Init
    u8 x9;  // 0x9
    u8 xa;  // 0xa
    u8 xb;  // 0xb
    struct
    {
        void *victim_coll_data;     // 0x0, victim's collision context
        void *attacker_trigger;     // 0x4, attacker's trigger/param data
        void *attacker_hurt_entry;  // 0x8, attacker's hurt entry
        Vec3 attacker_pos;          // 0xc, attacker position at collision
        float knockback;            // 0x18, computed knockback magnitude for this hit
    } log[20];
    int coll_num;        // 0x23c. amount of collisions found against this hurtbox
    HurtData *hurt_data; // 0x240. hurt data we are checking for hit collisions against
} HitCollData;

typedef struct DmgLog //
{                     //
    int xbac;         // 0xbac, 0x0
    int xbb0;         // 0xbb0, 0x4
    int xbb4;         // 0xbb4, 0x8
    int xbb8;         // 0xbb8, 0xC
    int xbbc;         // 0xbbc, 0x10
    int xbc0;         // 0xbc0, 0x14
    u16 xbc4;         // 0xbc4, 0x18
    u16 xbc6;         // 0xbc6, 0x1a
    int attacker_ply; // 0xbc8, 0x1c
} DmgLog;             //

// === Hurt Parameter Struct (0x34 bytes) ===
// Used by Machine_ApplyHurt and Machine_OnTouchItem (effect type 0x25 / fake items).
// Zeroed by Trigger_ClearParameterStruct, then filled with damage configuration.
// Passed to Trigger_InitParameters which copies 13 dwords into HitCollData.
// Fields map to TriggerData offsets 0x04-0x34 when copied.
typedef struct HurtParams
{
    int base_damage;          // 0x00, base damage value (int, converted to float by HitColl_GetDamageDealt)
    float dmg_distance_factor; // 0x04, damage scales with relative velocity magnitude (0 = fixed damage)
    float x08;                // 0x08
    float x0c;                // 0x0c
    float x10;                // 0x10
    int x14;                  // 0x14
    int x18;                  // 0x18
    float x1c;                // 0x1c, scale / magnitude factor
    float base_knockback;     // 0x20, base knockback magnitude
    float kb_distance_factor; // 0x24, knockback scales with relative velocity (0 = fixed knockback)
    int x28;                  // 0x28
    int hit_flags;            // 0x2c, bitfield: bit 3 = use zero direction vectors instead of positions
    int x30;                  // 0x30
} HurtParams; // 0x34 bytes total (13 dwords)

static HitCollData *stc_hitcolldata = (HitCollData *)0x80559bf4;

// === Damage Pipeline ===
// Per-frame flow in Machine_UpdateHitColl (0x801c67a0):
//   1. HitColl_Init          - Clears global collision log counter
//   2. Machine_Check*Collision - Various collision checks call HitColl_SetDamageLog
//      (Machine_ApplyHurt also calls HitColl_SetDamageLog for item-based damage)
//   3. HitColl_ActOnCollision - Processes collision log, sets HurtData.kb_mag from max knockback
//   4. Machine_ActOnHitCollision - If kb_mag != 0, calls Machine_EnterHitReaction (state 5)
//
// To apply damage with knockback from OUTSIDE this pipeline:
//   1. Machine_GiveDamage(md, damage, 0)    - subtract HP
//   2. md->hurt_data->kb_mag = magnitude    - set knockback (optional, for physics)
//   3. Machine_EnterHitReaction(md)         - enter bounce/hit reaction state 5

// === Hurt System Functions ===
void HitColl_Init(HurtData *hurt);                     // 0x8018cf64. Clears global collision log counter, sets victim hurt_data pointer
void Trigger_ClearParameterStruct(HurtParams *params);  // 0x8018a0c0. memset(params, 0, 0x34). Zeroes a HurtParams struct. Previously named HurtDesc_Init
HurtData *HurtData_Create(HurtDesc *desc, HurtKind kind, int obj1_kind, int obj2_kind, int obj3_kind); // 0x8018c1c8
void HurtData_GiveIntangibility(HurtData *hurt, int timer); // 0x8018cb5c. Sets intangibility for at least `timer` frames (only updates if > current). Sets vuln.kind = 2. Map name: HurtData_UpdateIntangibility
void HurtData_UpdateVulnState(HurtData *hurt);         // 0x8018cb28. Updates vulnerability state: clears flag at 0x9c, sets vuln.kind based on intang/invuln timers
HurtData *RiderGObj_GetHurtData(GOBJ *rider_gobj);     // 0x80192788. Returns HurtData from rider GOBJ userdata
void HitColl_SetDamageLog(HurtData *hurt_data, void *hurt_entry, HitCollData *hitcoll_data, void *trigger_params); // 0x8018cf94. Calculates damage via HitColl_GetDamageDealt, stores in collision log (up to 20 entries), applies knockback. Calls on_damage_callback at HurtData+0x8C if set
void HitColl_ActOnCollision(HurtData *hurt);           // 0x8018d878. Processes global collision log, finds max knockback, sets HurtData.kb_mag and collision position
float HitColl_GetDamageDealt(void *trigger_params);            // 0x8018ace4. Calculates final damage from trigger parameters: base_damage + distance_multiplier * scale_factor
void Trigger_InitParameters(void *dest, void *hurt_params, int flags); // 0x8018a118. Copies 13 dwords from hurt_params into dest (offsets 0x04-0x34), sets dest+0x38 = flags

#endif