#ifndef KAR_H_PROJECTILE
#define KAR_H_PROJECTILE

#include "datatypes.h"
#include "obj.h"
#include "collision.h"

#define PROJECTILE_GOBJ_KIND 23 // gobj->entity_class for every projectile GObj

typedef struct RiderData RiderData;

// Index into the per-kind data tables at 0x8055a9a8 and 0x804b4338. Values
// outside 0..PROJKIND_NUM-1 read past the tables and crash.
//
// "Thrown" kinds fly and damage on impact; "Aura" kinds spawn with zero
// velocity and are stored at rider+0x3F0, hovering on the rider as the
// visual/hitbox for the active copy ability.
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

// 76-byte descriptor passed to Projectile_Create. Vanilla callers read
// pos/forward/up from the rider's hand bone matrix; direct callers may fill
// them from a machine's transform and supply their own velocity.
typedef struct ProjectileDesc
{
    ProjectileKind kind;    // 0x00
    void *owner_gobj;       // 0x04: owner's rider GObj, the self-hit exclusion key. FIRE_BULLET
                            //       reads rider fields through it during create, so it cannot
                            //       be NULL for that kind.
    int owner_unk2;         // 0x08: copy of rider->x0
    u8  owner_byte;         // 0x0c: 0 in vanilla
    u8  pad_0d[3];          // 0x0d
    Vec3 position;          // 0x10: spawn world position
    Vec3 forward;           // 0x1c: forward unit vector
    Vec3 up;                // 0x28: up unit vector
    float velocity_scale;   // 0x34: misnomer - a SIZE scale, not a speed one. Drives the hitbox
                            //       size multiplier and model scale. Vanilla passes 1.0;
                            //       0.0 gives a zero-size hitbox.
    Vec3 velocity;          // 0x38: initial velocity (vanilla: md->world_velocity + rider->self_vel)
    int type_flag;          // 0x44: vanilla always passes 1; copied to proj+0x78 and never
                            //       branched on. Values > 1 are uncharted.
    float charge;           // 0x48: vanilla reads md->projectile_charge_scale
} ProjectileDesc;

// State enums below are entry INDICES into the kind's state table, not
// state_id values; where two entries share a state_id (SENSORBOMB, GORDO,
// FIRE_BULLET) the index is the only disambiguator. Single-state kinds
// (SWORD_STAR_A/B/CHARGED, PLASMA_SPREAD_MID/SIDE) have no enum - pass 0, and
// Projectile_Create already leaves them in that state.

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

// Auras share structure across fire/spike/ice: IDLE re-snaps to the rider hand
// every frame; FIRING is per-kind. SETTLED is a plausibility label.
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
typedef enum PlasmaDState {
    PLASMA_D_STATE_FLYING   = 0,
    PLASMA_D_STATE_TRAILING = 1,
} PlasmaDState;

// One entry of a kind's state table. All four fn slots are per-frame, running
// at priorities 1/4/5/6. There is no on-enter/on-exit slot - transitions come
// from SetState calls inside these callbacks, and once-per-create work belongs
// in the vtable's init / post_init.
typedef struct ProjectileStateEntry
{
    s32   state_id;                // 0x00: written to proj+0x2c. 0xffffffff = sentinel/terminator
    u32   flags;                   // 0x04: anim class (upper byte) + per-state bits
    void (*fn0)(void *proj);       // 0x08: prio-1 tick (early AI, timers)
    void (*fn1)(void *proj);       // 0x0c: prio-4 tick (pre-physics velocity update)
    void (*fn2)(void *proj);       // 0x10: prio-5 tick (main state tick, collision response)
    void (*fn3)(void *proj);       // 0x14: prio-6 tick (post-collision, aura re-snap)
} ProjectileStateEntry;

// Per-kind data at *(0x8055a9a8 + kind*4). Registration is all-or-nothing and
// rider-driven: Rider_Create feeds the full 17-kind list from RdKirbyAbility.dat
// to Projectile_RegisterKindDataList, so once any rider exists the whole table
// is live. Projectile_ClearKindDataTable zeroes it at system init and
// Projectile_Create dereferences the slot unchecked, so code spawning outside
// the normal rider lifetime must guard on the slot being non-NULL.
typedef struct ProjKindData
{
    const void                 *params;              // +0x00: 4 words; params[3] = default lifetime in frames
    const void                 *render_state_tmpl;   // +0x04: copied into proj+0x104. word0 is the muzzle
                                                     //        speed plasma / sword-star post_inits apply.
    void                       *model_desc;          // +0x08: two words - the model's JOBJDesc, then a word
                                                     //        whose top byte is that tree's joint count.
                                                     //        NULL falls back to a global default model.
                                                     //        The walker at 0x80221914 asserts if the tree
                                                     //        holds a different number, or more than 10
    const void                 *state_anim_spec_array; // +0x0c: 16-byte stride by state_id
    const void                 *vuln_region_spec;    // +0x10: vulnerable-region list (0x44 stride). NULL for
                                                     //        every kind but FIRE_BULLET and SENSORBOMB; the
                                                     //        two attack regions are hardcoded in
                                                     //        Projectile_InitHurtData instead.
    const void                 *mpcoll_desc;         // +0x14: 0 = no environment collision
} ProjKindData;

// Per-kind vtable at *(0x804b4338 + kind*4). Kind pairs 5/6 and 7/8 alias to
// the same vtable.
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
    void                      (*despawn)(void *proj);       // 0x20: Projectile_Despawn's exit; NULL falls back to
                                                            //       GObj_Destroy. Most kinds install a bare one
} ProjKindVTable;

// Inner projectile data - 0x220 bytes, reached via *(handle + 0x2c). Known
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
    int            state_table_split;    // 0x28: state_index < split selects proj+0x30, else proj+0x34.
                                         //       Hardcoded to 0 and never rewritten, so the +0x30 branch
                                         //       is dead.
    s32            state_id;             // 0x2c: the table entry's state_id
    const ProjectileStateEntry *state_table_ext;  // 0x30: extension table; never written or read by vanilla
    const ProjectileStateEntry *state_table;      // 0x34: primary table, from kind_data+0x00. Every vanilla
                                         //       SetState dispatch reads from here.
    void          *state_anim_spec;      // 0x38: kind_data[0x0C] + state_id*16, a 16-byte per-state
                                         //       animation/blend record
    u8             pad_3c[0x70 - 0x3c];  // 0x3c..0x6f: anim accumulator + sub-vtable refs (internal)
    float          velocity_scale;       // 0x70: desc.velocity_scale copy
    float          cur_scale;            // 0x74: live size scale, seeded from velocity_scale. The shared
                                         //       pipeline only reads it - HurtData size, env sweep radius
                                         //       and render cull radius all come off this one field
    int            type_flag;            // 0x78: desc.type_flag copy
    Vec3           accel;                // 0x7c: per-frame acceleration, zeroed at prio 0 and integrated
                                         //       into velocity at prio 4. Nothing supplies gravity - a
                                         //       ballistic arc must rewrite this every frame.
    Vec3           spawn_velocity;       // 0x88: desc.velocity snapshot (read-only after create)
    Vec3           velocity;             // 0x94: live physics velocity
    u8             pad_a0[0xac - 0xa0];  // 0xa0
    Vec3           position;             // 0xac: live world position
    Vec3           position_prev;        // 0xb8: previous frame (used by swept collision)
    Vec3           position_init;        // 0xc4: spawn position (collision anchor)
    u8             pad_d0[0x104 - 0xd0]; // 0xd0..0x103: rotation basis, sub-object refs (internal)
    void          *render_state;         // 0x104: per-projectile block holding the alpha/color/scale ramp
                                         //        the FADE state reads (+0x10 alpha, +0x14 lifetime,
                                         //        +0x2c/0x30 fade endpoints)
    void          *hurt_data;            // 0x108: HurtData; +0x0c = region array (0xC8 stride)
    int            lifetime;             // 0x10c: frames remaining, decremented at prio 1. Seeded from
                                         //        kind_data params[3]. 0 means INFINITE - bomb, sensor
                                         //        bomb and the three auras ship this way.
    int            frame_counter;        // 0x110: monotonic, incremented by prio-0
    void          *effect_handle_a;      // 0x114: particle-effect handle (not an audio voice), freed in the dtor
    void          *effect_handle_b;      // 0x118: second particle-effect handle, freed alongside 0x114
    u8             pad_11c[0x138 - 0x11c]; // 0x11c..0x137
    CollData      *coll_data;            // 0x138: environment collider, NULL when the kind has no
                                         //        mpcoll_desc. Its sweep is what runs the yakumono
                                         //        break dispatch, and its owner GObj is this projectile
    u8             pad_13c[0x14c - 0x13c]; // 0x13c..0x14b
    float          charge;               // 0x14c: desc.charge copy
    void         (*state_fn0)(void *p);  // 0x150: copied from state entry by Projectile_SetState
    void         (*state_fn1)(void *p);  // 0x154
    void         (*state_fn2)(void *p);  // 0x158
    void         (*state_fn3)(void *p);  // 0x15c
    void         (*user_hook_0)(void *p);     // 0x160: per-state hook, invoked at prio 0
    void         (*user_hook_1)(void *p);     // 0x164: per-state hook, invoked at prio 7
    void         (*user_hook_2)(void *p);     // 0x168
    int          (*user_hook_on_hit)(void *p);// 0x16c: prio-10 on-hit; non-zero requests a state transition
    u8             pad_170[0x1b4 - 0x170]; // 0x170..0x1b3: hit sub-struct (internal)
    u8             flag_a;               // 0x1b4: bit 0 = PROJ_ALLOW_SELF_HIT_INBOUND. Other bits set
                                          //        during damage logging.
    u8             flag_b;               // 0x1b5: bit 0 = env-colliding this frame; bit 2 = alive marker
                                          //        (always on); bit 5 = PROJ_ALLOW_SELF_HIT_OUTBOUND
    u8             flag_c;               // 0x1b6: bit 7 = state changed this frame; lower bits flag
                                          //        effect liveness
    u8             pad_1b7;              // 0x1b7
    u8             kind_scratch[0x218 - 0x1b8]; // 0x1b8..0x217: per-kind timers / effect handles.
                                          //        BOMB: 0x1c0 detonation countdown then FADE handle,
                                          //        0x1c8/0x1cc positional FADE effect, 0x1f8/0x1fc
                                          //        EXPLODING burst, 0x1d0..0x1ec fade ramp.
                                          //        FIRE_BULLET: 0x1b8/0x1bc = owner's Fire charge as
                                          //        (charge/max, charge); a custom spawner must seed both
                                          //        or the shot bursts at zero size with no hitbox.
                                          //        FIRECRACKER: 0x1b8/0x1bc = fuse, independent of lifetime.
    u8             flag_d;               // 0x218: subproc-gating; bit 0 always set
    u8             pad_219[0x220 - 0x219]; // 0x219..0x21f
} ProjectileData;

// The hit pipeline runs two scans per frame - outbound (the projectile walks
// the victim lists) and inbound (each victim walks the projectile list) - and
// each gates owner-exclusion on a different bit. A projectile that must damage
// its own owner has to set both, since either scan can resolve the hit first.
// Vanilla sets neither at create time except sensor bomb's inbound bit.
#define PROJ_ALLOW_SELF_HIT_INBOUND  0x01  // OR into proj->flag_a (proj+0x1b4)
#define PROJ_ALLOW_SELF_HIT_OUTBOUND 0x20  // OR into proj->flag_b (proj+0x1b5)

// Creates a projectile from a pre-filled descriptor, touching no rider bones or
// rider state. The ProjectileData hangs off the returned GObj's userdata. Leaves
// the projectile in state index 0, which for bomb / sensor bomb / gordo is "held
// in the rider's hand" and needs a Projectile_SetState before physics or
// detonation runs.
GOBJ *Projectile_Create(ProjectileDesc *desc); // 0x8021f428

// Transitions to the given state entry index. Does NOT touch physics velocity -
// set that first, mirroring vanilla throw() ordering. f_blend_a/f_blend_b are
// animation blend params (vanilla passes 1.0). flags bit 0 skips a
// rider-attached cleanup path; vanilla passes 1 for THROWN transitions and 0
// for the initial HELD setup.
void Projectile_SetState(void *proj, int state_index,
                         float f_blend_a, float f_blend_b, int flags); // 0x8021f7dc

// Returns proj->owner_gobj; used by every victim-side self-hit exclusion check.
void *Projectile_GetOwnerGObj(void *projGObj); // 0x8022312c

// Despawn by outer GObj handle, running the per-kind aux_a slot before
// GObj_Destroy. Vanilla uses it from the Fire/Spike/Ice LoseAbility handlers to
// destroy the held aura at rider+0x3F0.
void Projectile_DespawnGObj(void *projGObj); // 0x802230a0

// Writes a state entry's flags word into the projectile's per-state animation
// bytes (proj+0x17c / 0x184 / 0x18a / 0x18b).
void Projectile_AssignStateFlags(void *proj, int flags); // 0x80222298

// Tail of Projectile_Create: seeds cur_scale, lifetime, the animation
// accumulators and the alive flags. A projectile is unusable before it runs.
void Projectile_InitRuntimeState(void *proj); // 0x8021f2a0

// Ends a projectile through its kind's despawn slot. The prio-1 proc calls it the
// frame lifetime reaches zero.
void Projectile_Despawn(void *proj); // 0x80220364

// Runs one frame of environment collision on proj->coll_data: position update,
// map sweep and pushback. Every kind whose state_fn2 does env collision calls it
// first; a mod that replaces state_fn2 has to call it to keep the sweep - and
// with it any scene-object break - running.
void Projectile_UpdateEnvColl(void *proj); // 0x80221fd4

// Rider-side spawn helpers used by copy abilities. All assert on the rider
// having the matching ability hat model loaded. Position/forward/up come from
// the rider's hand bone, velocity from the machine's world_velocity plus
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

// Throw / transition wrappers, mutating an already-created projectile in place.
void Rider_TryThrowBomb(void *projGObj, Vec3 *unused, Vec3 *velocity); // 0x801a9580 -> 0x80225824
void Rider_TryThrowSensorBomb(void *projGObj, Vec3 *velocity);         // 0x801a9fe8 -> 0x80228f08
int  Rider_IsGordoThrowable(void *projGObj);                           // 0x801aa008 -> 0x8022a244 (PREDICATE, not a throw)

// Gordo HELD -> THROWN_ASCENDING. Reads orientation from owner_gobj's rider
// data rather than a hand-bone matrix, so it works whenever owner_gobj is a
// valid rider GObj even with no Phan-Phan ability initialised. It also seeds all
// the per-kind scratch gordo state 1 reads each frame (spin, decay, impulse,
// lifetime), which a bare Projectile_SetState(proj, 1, ...) leaves at zero.
void Gordo_EnterThrownState(void *projGObj, Vec3 *velocity, Vec3 *position); // 0x8022a544

#endif // KAR_H_PROJECTILE
