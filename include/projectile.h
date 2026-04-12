#ifndef KAR_H_PROJECTILE
#define KAR_H_PROJECTILE

#include "datatypes.h"
#include "obj.h"

// Forward-declare RiderData to avoid pulling in rider.h. The rider-based
// spawn functions below take a pointer but never dereference struct fields
// visible to callers of this header.
typedef struct RiderData RiderData;

// ProjectileKind is the index into the per-kind data tables at 0x8055a9a8
// and 0x804b4338 that Projectile_Create uses to resolve the projectile's
// model, stats, and hurt data. Range is 0..PROJKIND_NUM-1; passing a value
// outside this range reads past the table into the neighboring .data
// section (filename strings) and will crash.
//
// Mapping assembled from the per-kind spawn functions and their ability
// callers. "Thrown" kinds fly and damage on impact; "Aura" kinds are spawned
// with zero velocity and stored in rider[0xfc] — they hover on the rider as
// the visual/hitbox representation of the active copy ability.
typedef enum ProjectileKind
{
    PROJKIND_SWORD_STAR_A       = 0,  // spawnStarBullet @ 0x801a8c80 (flag=0)
    PROJKIND_SWORD_STAR_B       = 1,  // spawnStarBullet @ 0x801a8c80 (flag=1)
    PROJKIND_FIRE_BULLET        = 2,  // spawnFireBullet — Fire ability thrown shot
    PROJKIND_FIRE_AURA          = 3,  // spawnFireAura — Fire ability on-rider aura
    PROJKIND_BOMB               = 4,  // spawnBomb — Bomb ability thrown bomb
    PROJKIND_PLASMA_A           = 5,  // spawnPlasmaBullet, charge mode 5
    PROJKIND_PLASMA_B           = 6,  // spawnPlasmaBullet, charge mode 6
    PROJKIND_PLASMA_SPREAD_MID  = 7,  // spawnPlasmaSpread, mode-7 center shot
    PROJKIND_PLASMA_SPREAD_SIDE = 8,  // spawnPlasmaSpread, mode-7 angled side shots
    PROJKIND_PLASMA_C           = 9,  // spawnPlasmaBullet, charge mode 9
    PROJKIND_PLASMA_D           = 10, // spawnPlasmaBullet, charge mode 10
    PROJKIND_SWORD_STAR_CHARGED = 11, // spawnStarBullet @ 0x801a8df8 (alt/charged variant)
    PROJKIND_SPIKE_AURA         = 12, // spawnSpikeAura — Spike (Needle) ability on-rider aura
    PROJKIND_ICE_AURA           = 13, // spawnIceAura — Ice ability on-rider aura
    PROJKIND_FIRECRACKER        = 14, // spawnCrackerBullet — Firecracker powerup projectile
    PROJKIND_SENSORBOMB         = 15, // spawnSensorBomb — Bomb ability sensor variant
    PROJKIND_GORDO              = 16, // spawnGordo — Phan Phan enemy throw
    PROJKIND_NUM                = 17,
} ProjectileKind;

// 76-byte descriptor passed to Projectile_Create. Field layout derived from
// spawnBomb / spawnGordo / spawnSensorBomb disassembly. The vanilla callers
// read pos/forward/up from the rider's hand bone matrix, then add the
// rider's self_vel to a base velocity read from the machine's vcData.
// Direct callers (bypassing the rider bone chain) may fill these from a
// machine's pos/forward/up and supply their own velocity.
typedef struct ProjectileDesc
{
    ProjectileKind kind;    // 0x00: indexes per-kind data tables in Projectile_Create
    int owner_unk1;         // 0x04: copy of rider->x0 (unknown purpose, owner ID?)
    int owner_unk2;         // 0x08: copy of rider->x0
    u8  owner_byte;         // 0x0c: set to 0 in vanilla
    u8  pad_0d[3];          // 0x0d: padding
    Vec3 position;          // 0x10: spawn world position
    Vec3 forward;           // 0x1c: forward unit vector
    Vec3 up;                // 0x28: up unit vector
    float velocity_scale;   // 0x34: scalar multiplier, vanilla passes 1.0 (FLOAT_805e1348)
    Vec3 velocity;          // 0x38: initial velocity (vanilla: md->projectile_inherit_velocity + rider->self_vel)
    int type_flag;          // 0x44: vanilla passes 1
    float charge;           // 0x48: vanilla reads md->projectile_charge_scale
} ProjectileDesc;

// Creates a projectile from a pre-filled descriptor. Does not touch rider
// bones or rider state — takes everything it needs from the desc. The
// `kind` field indexes global tables at 0x8055a9a8 and 0x804b4338, so
// only kinds in 0..PROJKIND_NUM-1 are valid.
int Projectile_Create(ProjectileDesc *desc); // 0x8021f428

// Rider-side projectile spawn helpers used by copy abilities. Each one
// reads the rider's ability_hat_model chain at (*rd->ability_hat_model +
// 0x120) to get the throw bone and calls Projectile_Create with a kind-
// specific ID. spawnBomb is called by the Bomb copy ability; spawnGordo
// is called by Phan Phan's throw AI; spawnSensorBomb is the sensor bomb
// variant of the Bomb ability. These require ability_hat_model to be
// non-null (Bomb hat loaded) or they assert.
void spawnBomb(RiderData *rd);         // 0x801a9410, PROJKIND_BOMB
void spawnSensorBomb(RiderData *rd);   // 0x801a9e78, PROJKIND_SENSORBOMB
void spawnGordo(RiderData *rd);        // 0x801aa028, PROJKIND_GORDO

#endif // KAR_H_PROJECTILE
