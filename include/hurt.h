#ifndef KAR_H_HURT
#define KAR_H_HURT

#include "datatypes.h"
#include "obj.h"

typedef enum HurtKind
{
    HURTKIND_NONE = -1,     // the static dummy attacker Machine_ApplyHurt uses, made by the pool init at 0x8018c0ec
    HURTKIND_RIDER,
    HURTKIND_MACHINE,       // a ridden machine; mounting switches HurtData.kind from HURTKIND_MACHINE_EMPTY
    HURTKIND_MACHINE_EMPTY,
    HURTKIND_EVENTACTOR,    // enemy, Dyna Blade
    HURTKIND_ITEM,          // every city item, boxes included
    HURTKIND_WEAPON,
    HURTKIND_MAP,
} HurtKind;

// Attack cause, AttackData.kind. PlayerStats.attack_num counts causes below 27.
typedef enum AttackKind
{
    ATK_0,
    ATK_FIREGET,        // fire aura
    ATK_FIRESHOOT,      // fire bullet
    ATK_3,
    ATK_SLEEP,
    ATK_SWORD1,
    ATK_SWORD2,
    ATK_SWORD3,
    ATK_BOMB,
    ATK_PLASMA,
    ATK_NEEDLEEND,
    ATK_NEEDLEHOLD,
    ATK_MIKE,
    ATK_ICE,            // ice aura
    ATK_TORNADO,
    ATK_SPITSTAR,       // small exhaled star
    ATK_SPIN,           // quick spin
    ATK_FIREWORKS,
    ATK_SENSORBOMB,
    ATK_GORDO,
    ATK_PANICSPIN,
    ATK_SPITSTAR_LARGE,
    ATK_NUM = 36,
} AttackKind;

typedef struct HurtDesc
{
    int joint_idx;
    int x4;
    float scale;
    Vec3 offset;
} HurtDesc;

typedef struct HitRegion HitRegion;

typedef struct HurtData
{
    HurtKind kind;       // 0x0
    GOBJ *gobj;          // 0x4, owner, the attacker identity when this HurtData deals a hit
    int region_count;    // 0x8, attack regions: stages=2, riders/machines=4, enemies=2 (Dyna Blade=8)
    HitRegion *regions;  // 0xc, pooled in blocks of 2, 4 or 8
    int sub_region_count; // 0x10, hurt sub-regions, 1 or 2
    void *sub_regions;   // 0x14, sub-hurt data array (stride 0x44 per entry, indexed by Machine_ApplyHurt)
    void *x18;           // 0x18
    int hitcoll_log_idx; // 0x1c, index of strongest hit in HitCollData::log
    int attacker_kind;   // 0x20, HurtKind of the attacker that dealt the strongest hit
    float kb_mag;        // 0x24, knockback magnitude of the strongest hit
    float dmg_taken;     // 0x28, cumulative damage taken this frame
    float max_single_hit; // 0x2c, largest single-hit damage this frame
    Vec3 contact_point;  // 0x30, collision contact position
    Vec3 knockback_dir;  // 0x3c, knockback direction vector (from HitColl_CalcKnockbackDir)
    Vec3 hit_pos;        // 0x48, the victim sub-region position logged with the strongest hit
    int attacker_flags;  // 0x54, the top 5 bits of the attacker region's params.hit_flags
    float max_dmg_dealt; // 0x58, largest damage this HurtData dealt this frame, before reduction
    GOBJ *max_dmg_victim; // 0x5c, the victim of that hit
    int x60;             // 0x60
    int x64;             // 0x64
    void *pos_tracker;   // 0x68, position-tracking object (for velocity computation in HitColl_GetDamageDealt)
    float radius;        // 0x6c, hurtbox radius (set per-frame by HurtData_UpdatePerFrame)
    Vec3 center_pos;     // 0x70, hurtbox center position (set per-frame)
    float dmg_scale;     // 0x7c, multiplies the damage this HurtData deals
    float dmg_reduction; // 0x80, fraction taken off the damage and knockback it receives
                         //       (MachineData+0x4EC for machines)
    int x84;             // 0x84
    struct
    {
        int kind;         // 0x88, 0 = vulnerable, 1 = invincible, 2 = intangible
        void (*on_damage_callback)(void *, void *); // 0x8c, called by HitColl_SetDamageLog on damage;
                                                    //       set during the per-kind InitHurtData
        int x90;          // 0x90
        int intang_timer; // 0x94, intangibility timer (counts down, prevents damage while > 0)
        int invuln_timer; // 0x98, invulnerability timer (counts down, prevents damage while > 0)
    } vuln;
    u8 flags2;           // 0x9c, bit 7 cleared each frame by HurtData_UpdateVulnState; bits 4-5 set by UpdatePerFrame
} HurtData;


typedef struct HitCollLog
{
    HurtData *attacker;     // 0x0
    HitRegion *attacker_region; // 0x4
    void *victim_region;    // 0x8, victim's hurt sub-region
    Vec3 coll_pos;          // 0xc, copied from victim_region+0x38
    float knockback;        // 0x18, computed knockback magnitude for this hit
} HitCollLog;


typedef struct HitCollData
{
    int x0; // 0x0
    int x4; // 0x4
    u8 active; // 0x8, set to 1 by HitColl_Init
    u8 x9;  // 0x9
    u8 xa;  // 0xa
    u8 xb;  // 0xb
    HitCollLog log[20];  // 0xc
    int coll_num;        // 0x23c. amount of collisions found against this hurtbox
    HurtData *hurt_data; // 0x240. hurt data we are checking for hit collisions against
} HitCollData;

// An attack word. The machine rebuilds its own from the state table on every state change.
typedef struct AttackData
{
    int x0 : 8;                     // 0x0
    int x1 : 8;                     // 0x1
    int x2 : 8;                     // 0x2, flags: 0x01 active, 0x10 charge/push state, 0x20 rail state, 0x80 machine credit gate
    AttackKind kind : 8;            // 0x3
} AttackData;

typedef struct DmgHitRecord
{
    int x0;                         // 0x0
    u16 x4;                         // 0x4
    u16 x6_fe00 : 7;                // 0x6
    u16 victim_mask : 8;            // 0x6, 0x1fe, bit per ply this attack hit (Machine_StoreAttacker)
    u16 x6_0001 : 1;                // 0x6, 0x0001
} DmgHitRecord;

// The credited_* fields and attacker_ply are written together by Machine_StoreAttacker
// (0x80231d90) for each new attack instance, so they always name the same attack.
typedef struct DmgLog
{
    AttackData attack_data;         // 0xbac, 0x0, own attack word when this object is the attacker
    int credited_attack;            // 0xbb0, 0x4, the credited attack's word; low byte = attack cause
    DmgHitRecord hits;              // 0xbb4, 0x8
    DmgHitRecord credited_hits;     // 0xbbc, 0x10, the attacker's hits, copied by Machine_StoreAttacker
    u16 attack_id;                  // 0xbc4, 0x18, own attack instance, minted when the attack kind changes
    u16 credited_attack_id;         // 0xbc6, 0x1a, its attack instance, so one attack credits once
    int attacker_ply;               // 0xbc8, 0x1c
} DmgLog;

// Damage configuration used by Machine_ApplyHurt and Machine_OnTouchItem.
// Zeroed by Trigger_ClearParameterStruct, filled in, then copied into
// HitRegion.params by Trigger_InitParameters.
typedef struct HurtParams
{
    int base_damage;          // 0x00, base damage value (int, converted to float by HitColl_GetDamageDealt)
    float dmg_distance_factor; // 0x04, damage scales with relative velocity magnitude (0 = fixed damage)
    Vec3 offset;              // 0x08, joint offset
    float radius;             // 0x14, base radius
    int x18;                  // 0x18
    float x1c;                // 0x1c, scale / magnitude factor
    float base_knockback;     // 0x20, base knockback magnitude
    float kb_distance_factor; // 0x24, knockback scales with relative velocity (0 = fixed knockback)
    int x28;                  // 0x28
    int hit_flags;            // 0x2c, its top 5 bits become HurtData.attacker_flags
    int x30;                  // 0x30, top byte: 0xe0 region group, which HitColl_SetDamageLog matches
                              //       across the attacker's regions; 0x08 zeroes this region's velocity
} HurtParams;


static HitCollData *stc_hitcolldata = (HitCollData *)0x80559bf4;

// Per-frame damage flow in Machine_UpdateHitColl (0x801c67a0): HitColl_Init, the
// Machine_Check*Collision checks and Machine_ApplyHurt log hits through
// HitColl_SetDamageLog, HitColl_ActOnCollision takes the strongest into kb_mag, and
// Machine_ActOnHitCollision credits the attacker when kb_mag != 0. Outside it,
// Machine_GiveDamage applies damage alone.

// `hit` is a HurtData's hit record (&hitcoll_log_idx). Normalizes its knockback_dir
// (+0x20 from the record) and mirrors it off the ground plane, or returns the
// reversed fallback when it is within 0.9 of vertical.
void Hit_CalcDeflectDir(int *hit, Vec3 *fallback, Vec3 *out); // 0x80194ca4

void HitColl_Init(HurtData *hurt);                     // 0x8018cf64. Clears global collision log counter, sets victim hurt_data pointer
void Trigger_ClearParameterStruct(HurtParams *params);  // 0x8018a0c0. memset(params, 0, 0x34). Zeroes a HurtParams struct.
HurtData *HurtData_Create(GOBJ *gobj, HurtKind kind, int region_count, int sub_region_count, int has_pos_tracker); // 0x8018c1c8
// Sets intangibility for at least `timer` frames, only extending the current
// value, and sets vuln.kind = 2.
void HurtData_GiveIntangibility(HurtData *hurt, int timer); // 0x8018cb5c
// Clears the flag at +0x9c and sets vuln.kind from the intang/invuln timers.
void HurtData_UpdateVulnState(HurtData *hurt);         // 0x8018cb28
void HurtData_UpdateAttackRegions(HurtData *hurt);     // 0x8018c998, Trigger_UpdatePosition per region
// Calculates damage via HitColl_GetDamageDealt, logs it (up to 20 entries),
// applies knockback and fires on_damage_callback if set.
void HitColl_SetDamageLog(HurtData *victim, void *victim_region, HurtData *attacker, HitRegion *attacker_region); // 0x8018cf94
// Processes the global collision log, taking the max knockback into
// HurtData.kb_mag along with the collision position.
void HitColl_ActOnCollision(HurtData *hurt);           // 0x8018d878
// params.base_damage, plus params.dmg_distance_factor times the regions' relative
// speed; each tracker (HurtData.pos_tracker) may be NULL.
float HitColl_GetDamageDealt(HitRegion *atk_region, void *atk_tracker, void *vic_region, void *vic_tracker); // 0x8018ace4
// Copies params into region->params and sets region->jobj.
void Trigger_InitParameters(HitRegion *region, HurtParams *params, JOBJ *jobj); // 0x8018a118

// Candy invincibility: sets flags2 bit 6 and vuln.kind = 1 (invincible). The clear
// drops the bit and restores vuln.kind from the two timers.
void HurtData_SetCandyInvincible(HurtData *hurt);   // 0x8018cbc8
void HurtData_ClearCandyInvincible(HurtData *hurt); // 0x8018cbe8

// Each attack region keeps 12 {victim HurtData, frames} pairs at +0x68 so it hits a
// victim once per rehit interval.
int Hit_IsVictimRecorded(HurtData *victim, HitRegion *region); // 0x8018a408, 1 while the victim is listed
void Hit_ResetVictimList(HitRegion *region);                   // 0x80189d34
void Hit_TickVictimTimers(HitRegion *region);                  // 0x80189fd4, drops each pair whose timer runs out

#endif