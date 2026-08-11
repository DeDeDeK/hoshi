#ifndef KAR_H_TRIGGER
#define KAR_H_TRIGGER

#include "datatypes.h"
#include "obj.h"

typedef enum TriggerStatus
{
    TRIGGERSTATUS_INACTIVE,       // debug code skips drawing this type
    TRIGGERSTATUS_1,
    TRIGGERSTATUS_2,
    TRIGGERSTATUS_ACTIVE,         //
} TriggerStatus;

typedef struct TriggerDesc
{
    int joint_idx;
    int x4;
    float scale;
    Vec3 offset;
} TriggerDesc;

// TriggerData serves dual purpose:
// 1. As a trigger zone descriptor (x0=hurtdesc, state=TriggerStatus)
// 2. As an attack parameter container for enemies/items. In this case,
//    Trigger_InitParameters copies HurtParams[0..12] into offsets 0x04-0x34:
//      +0x04 = base_damage, +0x08 = dmg_distance_factor, +0x24 = base_knockback,
//      +0x28 = kb_distance_factor, +0x30 = hit_flags. Mirrors HurtParams.
typedef struct TriggerData
{
    void *x0;               // 0x00, hurtdesc pointer (trigger zones) or context ptr
    int x04;                // 0x04, TriggerStatus (zones) or base_damage (attack params)
    int x08;                // 0x08, dmg_distance_factor when used as attack params
    int x0c;                // 0x0c
    int x10;                // 0x10
    int x14;                // 0x14
    int x18;                // 0x18
    float scale;            // 0x1c
    int x20;                // 0x20
    int x24;                // 0x24, base_knockback when used as attack params
    int x28;                // 0x28, kb_distance_factor when used as attack params
    int x2c;                // 0x2c
    int x30;                // 0x30, hit_flags when used as attack params
    int x34;                // 0x34
    int x38;                // 0x38, additional flags (set by Trigger_InitParameters param3)
    Vec3 attacker_pos_cur;  // 0x3c, current attacker position (set by Machine_ApplyHurt)
    Vec3 attacker_pos_prev; // 0x48, previous attacker position
    Vec3 attacker_pos_prev2; // 0x54, second previous position
} TriggerData;

#endif