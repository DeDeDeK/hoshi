#ifndef KAR_H_TRIGGER
#define KAR_H_TRIGGER

#include "datatypes.h"
#include "obj.h"
#include "hurt.h"

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

// One attack region. HurtData.regions holds these at a 0xc8 stride and
// TriggerData embeds one; Trigger_UpdatePosition places it each frame.
typedef struct HitRegion
{
    TriggerStatus state;   // 0x00
    HurtParams params;     // 0x04, copied in by Trigger_InitParameters; +0x08..+0x10 is the
                           //       joint offset and +0x14 the base radius
    JOBJ *jobj;            // 0x38, joint the region follows; NULL for a region placed by hand
    int x3c;               // 0x3c
    Vec3 pos;              // 0x40, world position
    float radius;          // 0x4c, base radius times the update's scale
    Vec3 pos_prev;         // 0x50
    Vec3 x5c;              // 0x5c
    struct
    {
        HurtData *victim;  // 0x00
        int frames;        // 0x04
    } victims[12];         // 0x68, so a victim is hit once per rehit interval
} HitRegion;               // 0xc8


// A HitRegion set up by Trigger_Init (0x8018afe4) from a TriggerDesc and a joint.
// MachineData, ItemData and RiderData embed one.
typedef struct TriggerData
{
    int x0;                // 0x00, TriggerDesc.x4
    HitRegion region;      // 0x04, TriggerDesc.scale lands in region.params as the base radius
    float xcc;             // 0xcc
    float scale;           // 0xd0, Trigger_Init f1; MachineData passes model_scale
    float xd4;             // 0xd4, Trigger_Init f2; MachineData passes coll_radius_base
} TriggerData;             // 0xd8


#endif
