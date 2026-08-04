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
// "Thrown" kinds fly and damage on impact; "Aura" kinds are spawned
// with zero velocity and stored at rider+0x3F0 - they hover on the rider as
// the visual/hitbox representation of the active copy ability. (Fire/Spike/Ice
// *_AbilityInit all do `stw r3, 0x3F0(rider)`.)
typedef enum ProjectileKind
{
    PROJKIND_SWORD_STAR_A       = 0,  // spawnStarBullet @ 0x801a8c80 (flag=0)
    PROJKIND_SWORD_STAR_B       = 1,  // spawnStarBullet @ 0x801a8c80 (flag=1)
    PROJKIND_FIRE_BULLET        = 2,  // spawnFireBullet - Fire ability thrown shot
    PROJKIND_FIRE_AURA          = 3,  // spawnFireAura - Fire ability on-rider aura
    PROJKIND_BOMB               = 4,  // spawnBomb - Bomb ability thrown bomb
    PROJKIND_PLASMA_A           = 5,  // spawnPlasmaBullet, charge mode 5
    PROJKIND_PLASMA_B           = 6,  // spawnPlasmaBullet, charge mode 6
    PROJKIND_PLASMA_SPREAD_MID  = 7,  // spawnPlasmaSpread, mode-7 center shot
    PROJKIND_PLASMA_SPREAD_SIDE = 8,  // spawnPlasmaSpread, mode-7 angled side shots
    PROJKIND_PLASMA_C           = 9,  // spawnPlasmaBullet, charge mode 9
    PROJKIND_PLASMA_D           = 10, // spawnPlasmaBullet, charge mode 10
    PROJKIND_SWORD_STAR_CHARGED = 11, // spawnStarBullet @ 0x801a8df8 (alt/charged variant)
    PROJKIND_SPIKE_AURA         = 12, // spawnSpikeAura - Spike (Needle) ability on-rider aura
    PROJKIND_ICE_AURA           = 13, // spawnIceAura - Ice ability on-rider aura
    PROJKIND_FIRECRACKER        = 14, // spawnCrackerBullet - Firecracker powerup projectile
    PROJKIND_SENSORBOMB         = 15, // spawnSensorBomb - Bomb ability sensor variant
    PROJKIND_GORDO              = 16, // spawnGordo - Phan Phan enemy throw
    PROJKIND_NUM                = 17,
} ProjectileKind;

// 76-byte descriptor passed to Projectile_Create. The vanilla callers
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
    Vec3 velocity;          // 0x38: initial velocity (vanilla: md->world_velocity + rider->self_vel)
    int type_flag;          // 0x44: vanilla spawners all pass 1. Copied verbatim to proj+0x78;
                            //       not routed into any per-frame branch. Treat values >1 as uncharted.
    float charge;           // 0x48: vanilla reads md->projectile_charge_scale
} ProjectileDesc;

// Per-kind state machine indices. The argument to Projectile_SetState is the
// entry INDEX into the kind's state table, not the state_id field; where two
// table entries share a state_id (SENSORBOMB, GORDO, FIRE_BULLET) the index
// is the only disambiguator.
//
// For single-state kinds, Projectile_Create already leaves the projectile in
// its one flying state - no Projectile_SetState call is needed.

typedef enum BombState {
    BOMB_STATE_HELD      = 0, // pinned to rider hand; no physics, no detonate
    BOMB_STATE_THROWN    = 1, // flies under physics; detonates on env collision
    BOMB_STATE_EXPLODING = 2, // hitbox active; short timer before fade
    BOMB_STATE_FADE      = 3, // alpha fades to 0, then GObj_Destroy
} BombState;

typedef enum SensorBombState {
    SENSOR_BOMB_STATE_HELD             = 0,
    SENSOR_BOMB_STATE_ARMED_FLYING     = 1, // thrown, physics + proximity scan
    SENSOR_BOMB_STATE_ARMED_STATIONARY = 2, // landed, waiting; short-range sensor
    SENSOR_BOMB_STATE_EXPLODING        = 3,
    SENSOR_BOMB_STATE_FADE             = 4, // state_id duplicates index 2
} SensorBombState;

typedef enum GordoState {
    GORDO_STATE_HELD             = 0,
    GORDO_STATE_THROWN_ASCENDING = 1, // state_id=1, scales up while flying
    GORDO_STATE_THROWN_TIMED     = 2, // state_id=1 again, self-despawn timer
    GORDO_STATE_DESPAWN          = 3, // sentinel (-1); fn0 runs particle burst
} GordoState;

typedef enum FireBulletState {
    FIRE_BULLET_STATE_THROWN    = 0,
    FIRE_BULLET_STATE_HIT_PAUSE = 1, // all-blr anim pad, likely hit-flash freeze
    FIRE_BULLET_STATE_DESPAWN   = 2, // sentinel (-1) with cleanup-watchdog fn0
} FireBulletState;

// Auras share structure across fire/spike/ice: IDLE re-snaps to the rider
// hand every frame; FIRING is per-kind (spike differs only in anim flags,
// ice runs real per-frame work, fire has a watchdog fn0). SETTLED is a
// plausibility label - could equally be "idle2" or "retract".
typedef enum FireAuraState {
    FIRE_AURA_STATE_IDLE    = 0,
    FIRE_AURA_STATE_FIRING  = 1,
    FIRE_AURA_STATE_COOLING = 2,
} FireAuraState;

typedef enum SpikeAuraState {
    SPIKE_AURA_STATE_IDLE     = 0,
    SPIKE_AURA_STATE_DEPLOYED = 1,
    SPIKE_AURA_STATE_SETTLED  = 2,
} SpikeAuraState;

typedef enum IceAuraState {
    ICE_AURA_STATE_IDLE     = 0,
    ICE_AURA_STATE_EMITTING = 1,
    ICE_AURA_STATE_SETTLED  = 2,
} IceAuraState;

// Plasma A/B share a state table; C and D each have their own.
typedef enum PlasmaState {
    PLASMA_STATE_FLYING   = 0,
    PLASMA_STATE_ABSORBED = 1, // merged with rider/charger; rider-alive watchdog
} PlasmaState;

typedef enum FirecrackerState {
    FIRECRACKER_STATE_FLYING   = 0, // physics + timer + trail emitter
    FIRECRACKER_STATE_ABSORBED = 1,
} FirecrackerState;

// Plasma D has two active entries - initial shot and trail fragment.
// Plausible naming; could instead be split-phase.
typedef enum PlasmaDState {
    PLASMA_D_STATE_FLYING   = 0,
    PLASMA_D_STATE_TRAILING = 1,
} PlasmaDState;

// Single-state kinds (SWORD_STAR_A/B/CHARGED, PLASMA_SPREAD_MID/SIDE) all
// map index 0 to their only flying state. No enum provided - just pass 0.

// 24-byte state-table entry. Each kind's state table is an array of these;
// Projectile_SetState indexes it by entry index and copies fn0..fn3 into
// proj+0x150..0x15c for per-frame dispatch.
//
// All four fn slots are per-frame. They run in priority order during the
// projectile proc update: fn0 at prio 1, fn1 at prio 4, fn2 at prio 5,
// fn3 at prio 6. State transitions are driven by SetState calls from inside
// these callbacks; there is NO entry-level on-enter/on-exit slot. For
// once-per-create init, use the per-kind vtable's init / postInit (see
// ProjKindVTable).
typedef struct ProjectileStateEntry
{
    s32   state_id;                // 0x00: written to proj+0x2c. 0xffffffff = sentinel/terminator
    u32   flags;                   // 0x04: anim class (upper byte) + per-state bits
    void (*fn0)(void *proj);       // 0x08: prio-1 tick (early AI, timers)
    void (*fn1)(void *proj);       // 0x0c: prio-4 tick (pre-physics velocity update)
    void (*fn2)(void *proj);       // 0x10: prio-5 tick (main state tick, collision response)
    void (*fn3)(void *proj);       // 0x14: prio-6 tick (post-collision, aura re-snap)
} ProjectileStateEntry;

// Per-kind data struct at *(0x8055a9a8 + kind*4). The table is populated at
// stage load (writers live in .data, not executable code). The unload-side
// clearer is at 0x8022011c (zeroes all 17 slots). Fields are read across
// Projectile_Create, Projectile_SetState, and Projectile_InitHurtData:
//
//   +0x00: ProjectileStateEntry * state_table      (Solid; vtable[0] copy)
//   +0x04: always NULL                             (Solid)
//   +0x08: void * model_desc                       (Solid; Projectile_Create
//          reads word0 -> proj+0x18 and loads it via HSD_JObjLoadJoint to
//          build the projectile's model joint; word1 -> proj+0x1c. When this
//          pointer is NULL, a global default model is used instead.)
//   +0x0c: void * state_anim_spec_array            (Solid; 16-byte stride,
//          indexed by state_id, result at proj+0x38)
//   +0x10: void * hurt_region_spec                 (Solid; +0x00 = region-desc
//          array base, +0x04 = region count; consumed by
//          Projectile_InitHurtData at 0x80221440 -> HurtData_Create +
//          HurtData_InitRegion, 0x18-byte stride. This is where a kind's
//          damage/knockback/radius live - baked into HurtData at create.)
//   +0x14..0x30: unknown / possibly unused. No bomb state function
//          dereferences kind_data here.
typedef struct ProjKindData
{
    const ProjectileStateEntry *state_table;           // +0x00
    void                       *reserved_04;            // +0x04: always NULL
    void                       *model_desc;             // +0x08: model joint descriptor (HSD_JObjLoadJoint)
    const void                 *state_anim_spec_array;  // +0x0c: 16-byte stride by state_id
    const void                 *hurt_region_spec;       // +0x10: +0x00 region base, +0x04 count; 0x18 stride
    // +0x14 onward: unknown / possibly unused; untyped.
} ProjKindData;

// Per-kind vtable at *(0x804b4338 + kind*4). One vtable per ProjectileKind;
// two kind pairs (5/6, 7/8) alias to the same vtable.
typedef struct ProjKindVTable
{
    const ProjectileStateEntry *state_table; // 0x00: per-kind state entries
    void                       *reserved04;  // 0x04: always 0
    void                      (*init)(void *proj);          // 0x08: one-shot at create
    void                      (*refresh_xfm_a)(void *proj); // 0x0c: per-frame JObj mtx refresh
    void                      (*refresh_xfm_b)(void *proj); // 0x10: identical to refresh_xfm_a in every vanilla kind
    void                      (*aux_a)(void *proj);         // 0x14: state-exit cleanup; NULL for some kinds
    void                      (*post_init)(void *proj);     // 0x18: one-shot at create, after proc registration
    void                      (*aux_b)(void *proj);         // 0x1c: per-frame kind-specific hook; NULL for some kinds
} ProjKindVTable;

// Inner projectile data - 0x220 bytes, allocated by Projectile_Create from
// the HSD object pool at 0x8055a8f8 and reached via *(handle + 0x2c). Known
// fields only; unknown regions are padding.
typedef struct ProjectileData
{
    void          *gobj;                 // 0x00: back-pointer to outer GObj
    ProjectileKind kind;                 // 0x04
    void          *owner_gobj;           // 0x08: self-hit exclusion key
    int            owner_unk2;           // 0x0c: duplicate owner id for hit attribution
    u8             pad_10[4];            // 0x10: text/vfx slot ptr (internal)
    u8             owner_byte;           // 0x14: usually 0
    u8             pad_15[0x20 - 0x15];  // 0x15..0x1f
    void          *kind_data;            // 0x20: per-kind scenario data (0x8055a9a8[kind])
    int            state_index;          // 0x24: arg of Projectile_SetState
    int            state_table_split;    // 0x28: cutoff for the two-table switch in Projectile_SetState:
                                         //       if state_index < split, use proj+0x30; else use proj+0x34.
                                         //       Projectile_Create hardcodes this to 0, and no vanilla code
                                         //       rewrites it, so the proj+0x30 branch is dead - every state
                                         //       transition falls into the proj+0x34 branch. Left as a field
                                         //       in case a future caller wants an extension table at +0x30.
    s32            state_id;             // 0x2c: table entry's state_id field, written by Projectile_SetState
    const ProjectileStateEntry *state_table_ext;  // 0x30: extension state table. Never written by vanilla
                                         //       (stays at its memset-0 default) and never read thanks to
                                         //       proj+0x28 being 0. Intended for future/mod extension sets.
    const ProjectileStateEntry *state_table;      // 0x34: primary state table, loaded by Projectile_Create
                                         //       from kind_data+0x00 (which is vtable[0].state_table).
                                         //       Every vanilla SetState dispatch reads from here.
    void          *state_anim_spec;      // 0x38: per-state animation/blend data, written by Projectile_SetState
                                         //       as `kind_data[0x0C] + state_id*16` - a 16-byte array indexed by
                                         //       state_id. Not the 24-byte state_table entry; the entry itself
                                         //       is used transiently and not stored back on proj.
    u8             pad_3c[0x70 - 0x3c];  // 0x3c..0x6f: anim accumulator + sub-vtable refs (internal)
    float          velocity_scale;       // 0x70: desc.velocity_scale copy
    float          cur_scale;            // 0x74
    int            type_flag;            // 0x78: desc.type_flag copy
    u8             pad_7c[0x88 - 0x7c];  // 0x7c..0x87: linear accel (per-frame integrated)
    Vec3           spawn_velocity;       // 0x88: desc.velocity snapshot (read-only after create)
    Vec3           velocity;             // 0x94: live physics velocity
    u8             pad_a0[0xac - 0xa0];  // 0xa0
    Vec3           position;             // 0xac: live world position
    Vec3           position_prev;        // 0xb8: previous frame (used by swept collision)
    Vec3           position_init;        // 0xc4: spawn position (collision anchor)
    u8             pad_d0[0x104 - 0xd0]; // 0xd0..0x103: rotation basis, sub-object refs (internal)
    void          *render_state;         // 0x104: HSD_ObjAlloc'd block (alloc'd by 0x802205b0) holding the
                                         //        alpha/color/scale ramp fields the FADE state reads
                                         //        (+0x10 alpha, +0x14 lifetime, +0x2c/0x30 fade endpoints).
                                         //        Per-projectile - NOT the per-kind data table.
    void          *hurt_data;            // 0x108: HurtData (Projectile_InitHurtData); +0x0c = region array (0xC8 stride)
    int            lifetime;             // 0x10c: frames remaining; prio-1 decrements
    int            frame_counter;        // 0x110: monotonic, incremented by prio-0
    void          *effect_handle_a;      // 0x114: particle-effect handle (Effect module). Allocated by
                                         //        0x802364e0, passed to Effect_SpawnSync as attach parent;
                                         //        freed in the dtor. NOT an audio voice.
    void          *effect_handle_b;      // 0x118: second particle-effect handle, freed alongside 0x114
    u8             pad_11c[0x14c - 0x11c]; // 0x11c..0x14b
    float          charge;               // 0x14c: desc.charge copy
    void         (*state_fn0)(void *p);  // 0x150: copied from state entry by Projectile_SetState
    void         (*state_fn1)(void *p);  // 0x154
    void         (*state_fn2)(void *p);  // 0x158
    void         (*state_fn3)(void *p);  // 0x15c
    void         (*user_hook_0)(void *p);     // 0x160: per-state hook, invoked at prio 0
    void         (*user_hook_1)(void *p);     // 0x164: per-state hook, invoked at prio 7
    void         (*user_hook_2)(void *p);     // 0x168
    int          (*user_hook_on_hit)(void *p);// 0x16c: prio-10 on-hit callback; return non-zero to request state transition
    u8             pad_170[0x1b4 - 0x170]; // 0x170..0x1b3: hit sub-struct (internal)
    u8             flag_a;               // 0x1b4: bit 0 = allow-self-hit (inbound-scan side; default 0).
                                          //        Other bits set during damage logging.
                                          //        See PROJ_ALLOW_SELF_HIT_INBOUND below.
    u8             flag_b;               // 0x1b5: bit 0 = env-colliding this frame (set/cleared by mpColl tick);
                                          //        bit 2 = alive marker (always on);
                                          //        bit 4 = allow-self-hit (outbound-scan side; default 0).
                                          //        Inbound and outbound scans are gated by *separate* flags;
                                          //        a projectile that needs to damage its own owner-player must
                                          //        set both. See PROJ_ALLOW_SELF_HIT_OUTBOUND below.
    u8             flag_c;               // 0x1b6: effect/anim-state bits. Bit 7 = state-changed-this-frame
                                          //        (set by every transition); lower bits flag effect liveness.
    u8             pad_1b7;              // 0x1b7
    u8             kind_scratch[0x218 - 0x1b8]; // 0x1b8..0x217: per-kind scratch (timers / particle-effect handles).
                                          //        BOMB: 0x1c0 = detonation countdown, then reused as a FADE effect
                                          //        handle; 0x1c8/0x1cc = positional FADE effect (reaped by aux_a);
                                          //        0x1f8/0x1fc = EXPLODING burst effect; 0x1d0..0x1ec = fade ramp.
    u8             flag_d;               // 0x218: subproc-gating; bit 0 always set
    u8             pad_219[0x220 - 0x219]; // 0x219..0x21f
} ProjectileData;
// _Static_assert(sizeof(ProjectileData) == 0x220, "ProjectileData size mismatch");

// Self-hit allow flags. The hit pipeline runs two scans every frame: an
// outbound one where the projectile walks the rider/machine/box lists, and
// an inbound one where each victim walks the projectile global list. Each
// scan gates owner-exclusion on a *different* flag bit, so a projectile
// that needs to damage its own owner-player has to set both - setting only
// PROJ_ALLOW_SELF_HIT_INBOUND can drop the hit if the outbound scan
// resolves it first, and vice versa. Custom-spawned trap projectiles
// (where owner = the trapped player) are the most common case for this.
//
// Inbound side (rider-side scan, e.g. Rider_CheckProjectileHit @ 0x801963c8)
// reads proj+0x1b4 bit 0. Sensor bomb's post_init sets this bit by default;
// bomb and gordo do not.
//
// Outbound side (Projectile_CheckRiderCollision @ 0x802215a4) reads
// proj+0x1b5 bit 4. Vanilla never sets this bit on bomb / sensor bomb /
// gordo at create time - vanilla projectiles target *other* players, so
// the default exclusion is what they want.
#define PROJ_ALLOW_SELF_HIT_INBOUND  0x01  // OR into proj->flag_a (proj+0x1b4)
#define PROJ_ALLOW_SELF_HIT_OUTBOUND 0x10  // OR into proj->flag_b (proj+0x1b5)

// Creates a projectile from a pre-filled descriptor. Does not touch rider
// bones or rider state - takes everything it needs from the desc. The
// `kind` field indexes global tables at 0x8055a9a8 and 0x804b4338, so
// only kinds in 0..PROJKIND_NUM-1 are valid.
//
// Returns an opaque proc handle. Use Projectile_GetData() to reach the inner
// struct. Post-init leaves the projectile in state index 0; for bomb/sensor
// bomb/gordo that state is "held in the rider's hand" and requires a separate
// state transition before physics/detonation logic runs - see
// Projectile_SetState.
void *Projectile_Create(ProjectileDesc *desc); // 0x8021f428

// Transitions a projectile (inner data pointer) to the given state entry
// index. Copies the entry's fn0..fn3 into proj+0x150..0x15c, writes
// proj+0x24 = state_index and proj+0x2c = state_id.
//
// Does NOT touch physics velocity at proj+0x94..0x9c - set that yourself
// before calling, mirroring vanilla throw() ordering. `f_blend_a`/`f_blend_b`
// are animation blend params (vanilla passes 1.0 for both). `flags` bit 0
// set skips a rider-attached cleanup path (vanilla throw passes 1 for
// THROWN transitions; post-init passes 0 for initial HELD setup).
void Projectile_SetState(void *proj, int state_index,
                         float f_blend_a, float f_blend_b, int flags); // 0x8021f7dc

// Trivial 3-instruction accessor at 0x8022312c: returns proj->owner_gobj.
// Used by every victim-side hit-check for self-hit exclusion.
void *Projectile_GetOwnerGObj(void *projGObj); // 0x8022312c

// Despawn a projectile given its outer GObj handle. Thin wrapper: loads the
// inner ProjectileData from gobj+0x2c then tail-calls Projectile_Despawn, so
// the per-kind aux_a vtable slot still runs before GObj_Destroy. Vanilla uses
// this from the copy-ability LoseAbility handlers (Fire / Spike / Ice) to
// destroy the held aura stored at rider+0x3F0.
void Projectile_DespawnGObj(void *projGObj); // 0x802230a0

// Writes the state-entry flags word into the projectile's per-state animation
// bytes (proj+0x17c / 0x184 / 0x18a / 0x18b). Called by Projectile_SetState
// every time a new entry is selected; this is where flags semantics are
// consumed.
void Projectile_AssignStateFlags(void *proj, int flags); // 0x80222298

// Convenience: return the inner ProjectileData from the outer handle.
// Implemented by callers as `*(ProjectileData **)((u8 *)handle + 0x2c)`;
// not a vanilla symbol, exposed here only as a documented pattern.
static inline ProjectileData *Projectile_GetData(void *handle)
{
    return *(ProjectileData **)((u8 *)handle + 0x2c);
}

// Rider-side projectile spawn helpers used by copy abilities. All require
// the rider to have the matching ability hat/model loaded; they assert on
// rd->ability_hat_model being non-null. Position/forward/up are read from
// the rider's hand bone; velocity from the machine's world_velocity plus
// rider->self_vel.
void spawnStarBullet(RiderData *rd, int flag);            // 0x801a8c80, PROJKIND_SWORD_STAR_{A,B}
void spawnStarBullet_charged(RiderData *rd);              // 0x801a8df8, PROJKIND_SWORD_STAR_CHARGED
void spawnFireBullet(RiderData *rd, Vec3 *pos);           // 0x801a8f68, PROJKIND_FIRE_BULLET
void spawnFireAura(RiderData *rd, int kind_variant);      // 0x801a9178, PROJKIND_FIRE_AURA
void spawnBomb(RiderData *rd);                            // 0x801a9410, PROJKIND_BOMB
void spawnPlasmaBullet(RiderData *rd, int charge_mode);   // 0x801a95a0, PROJKIND_PLASMA_{A,B,C,D}
void spawnPlasmaSpread(RiderData *rd, int which);         // 0x801a9870, PROJKIND_PLASMA_SPREAD_{MID,SIDE}
void spawnSpikeAura(RiderData *rd);                       // 0x801a9a54, PROJKIND_SPIKE_AURA
void spawnIceAura(RiderData *rd);                         // 0x801a9b84, PROJKIND_ICE_AURA
void spawnCrackerBullet(RiderData *rd, Vec3 *pos, Vec3 *forward); // 0x801a9cb4, PROJKIND_FIRECRACKER
void spawnSensorBomb(RiderData *rd);                      // 0x801a9e78, PROJKIND_SENSORBOMB
void spawnGordo(RiderData *rd);                           // 0x801aa028, PROJKIND_GORDO

// Throw / transition wrappers. These act on an already-created projectile
// (returned by one of the spawn* helpers above), mutating it in place.
void Rider_TryThrowBomb(void *projGObj, Vec3 *unused, Vec3 *velocity); // 0x801a9580 -> 0x80225824
void Rider_TryThrowSensorBomb(void *projGObj, Vec3 *velocity);         // 0x801a9fe8 -> 0x80228f08
int  Rider_IsGordoThrowable(void *projGObj);                           // 0x801aa008 -> 0x8022a244 (PREDICATE, not a throw)

// Gordo HELD->THROWN_ASCENDING transition. Unlike Rider_TryThrowBomb (which
// reads pos/forward/up from `*(proj+0x6c)+8`, a hand-bone matrix that only
// exists when a rider is actively holding the projectile), this function
// reads orientation directly from `proj->owner_gobj`'s rider data fields
// (rd+0x324, rd+0x330) - so it works as long as owner_gobj is a valid rider
// GObj, even when no Phan-Phan ability has been initialised. Beyond the
// state transition it sets up all per-kind scratch (proj+0x1d8 = 2,
// proj+0x1e0..0x1e8 = velocity, proj+0x1dc = randomized angular velocity,
// proj+0x7c..0x84 = velocity-derived accel from kind_data, proj+0x10c =
// lifetime) that gordo state 1 fn1 reads each frame. Calling
// `Projectile_SetState(proj, 1, ...)` directly leaves all of that at zero
// and the gordo never spins, never decays, and never gets a real impulse.
void Gordo_EnterThrownState(void *projGObj, Vec3 *velocity, Vec3 *position); // 0x8022a544

#endif // KAR_H_PROJECTILE
