#ifndef MEX_H_WEAPON
#define MEX_H_WEAPON

#include "datatypes.h"
#include "obj.h"
#include "collision.h"
#include "hurt.h"

// Weapons are the game's thrown and aura objects (wnparts.c, WnCommon.dat, wnDataCommon).
#define WEAPON_GOBJ_KIND 23 // gobj->entity_class for every weapon GObj

typedef struct RiderData RiderData;

typedef enum WeaponPri
{
    WPPRI_FRAMESTART,   // framestart_callback
    WPPRI_ANIM,         // anim, anim_callback, lifetime
    WPPRI_PHYS = 4,     // phys_callback, velocity integration
    WPPRI_ENVCOLL,      // envcoll_callback
    WPPRI_POSTENVCOLL,  // post_envcoll_callback, then Weapon_SyncRootMtx
    WPPRI_TRIGGER,      // trigger_callback, HurtData_UpdatePerFrame
    WPPRI_8,
    WPPRI_HITCOLL,
    WPPRI_DMGAPPLY,     // on-hit reaction
    WPPRI_ENDOFFRAME = 21,
} WeaponPri;

// Index into the per-kind data tables at 0x8055a9a8 and 0x804b4338. Values
// outside 0..WPKIND_NUM-1 read past the tables and crash.
//
// "Thrown" kinds fly and damage on impact; "Aura" kinds spawn with zero
// velocity and are stored at rider+0x3F0, hovering on the rider as the
// visual/hitbox for the active copy ability.
typedef enum WeaponKind
{
    WPKIND_SPITSMALL          = 0,  // Rider_SpawnStarBullet (flag=0), inhale spit
    WPKIND_SPITLARGE          = 1,  // Rider_SpawnStarBullet (flag=1), inhale spit
    WPKIND_FIRE_BULLET        = 2,  // Rider_SpawnFireBullet - Fire ability thrown shot
    WPKIND_FIRE_AURA          = 3,  // Rider_SpawnFireAura - Fire ability on-rider aura
    WPKIND_BOMB               = 4,  // Rider_SpawnBomb - Bomb ability thrown bomb
    WPKIND_PLASMA_A           = 5,  // Rider_SpawnPlasmaBullet, charge mode 5
    WPKIND_PLASMA_B           = 6,  // Rider_SpawnPlasmaBullet, charge mode 6
    WPKIND_PLASMA_SPREAD_MID  = 7,  // Rider_SpawnPlasmaSpread, mode-7 center shot
    WPKIND_PLASMA_SPREAD_SIDE = 8,  // Rider_SpawnPlasmaSpread, mode-7 angled side shots
    WPKIND_PLASMA_C           = 9,  // Rider_SpawnPlasmaBullet, charge mode 9
    WPKIND_PLASMA_D           = 10, // Rider_SpawnPlasmaBullet, charge mode 10
    WPKIND_SPITCHARGED        = 11, // Rider_SpawnStarBulletCharged, inhale spit
    WPKIND_NEEDLE_AURA        = 12, // Rider_SpawnNeedleAura - Needle ability on-rider aura
    WPKIND_ICE_AURA           = 13, // Rider_SpawnIceAura - Ice ability on-rider aura
    WPKIND_FIRECRACKER        = 14, // Rider_SpawnCrackerBullet - Firecracker powerup shot
    WPKIND_SENSORBOMB         = 15, // Rider_SpawnSensorBomb - Sensor Bomb item
    WPKIND_GORDO              = 16, // Rider_SpawnGordo - Phan Phan enemy throw
    WPKIND_NUM                = 17,
} WeaponKind;

// 76-byte descriptor passed to Weapon_Create. Vanilla callers read
// pos/forward/up from the rider's hand bone matrix; direct callers may fill
// them from a machine's transform and supply their own velocity.
typedef struct WeaponDesc
{
    WeaponKind kind;    // 0x00
    GOBJ *owner_gobj;       // 0x04: owner's rider GObj, the self-hit exclusion key. FIRE_BULLET
                            //       reads rider fields through it during create, so it cannot
                            //       be NULL for that kind.
    GOBJ *owner_gobj2;      // 0x08: vanilla spawners pass the rider GObj here too
    u8  owner_byte;         // 0x0c: 0 in vanilla
    u8  pad_0d[3];          // 0x0d
    Vec3 pos;               // 0x10: spawn world position
    Vec3 forward;           // 0x1c: forward unit vector
    Vec3 up;                // 0x28: up unit vector
    float scale;            // 0x34: size scale -> WeaponData.base_scale. Drives the hitbox size and
                            //       model scale. Vanilla passes 1.0; 0.0 gives a zero-size hitbox.
    Vec3 vel;               // 0x38: initial velocity (vanilla: md->world_velocity + rider->self_vel)
    int type_flag;          // 0x44: vanilla always passes 1; copied to wp+0x78 and never
                            //       branched on.
    float charge;           // 0x48: the HurtData's dmg_scale, 0 = no damage; vanilla reads md->weapon_charge_scale
} WeaponDesc;

// State enums below are entry INDICES into the kind's state table, not
// state_id values; where two entries share a state_id (SENSORBOMB, GORDO,
// FIRE_BULLET) the index is the only disambiguator. Single-state kinds
// (SPITSMALL/LARGE/CHARGED, PLASMA_SPREAD_MID/SIDE) have no enum - pass 0, and
// Weapon_Create already leaves them in that state.

typedef enum BombState {
    BOMB_STATE_HELD   = 0, // pinned to rider hand; no physics, no detonate
    BOMB_STATE_THROWN = 1, // falls under stage gravity; Bomb_DetonationTrigger on any env contact
    BOMB_STATE_PRIMED = 2, // frozen for BombParams.blast_delay frames, no hitbox
    BOMB_STATE_BLAST  = 3, // hitbox, fireball, quake and wind; destroyed when the fireball ends
} BombState;

typedef enum SensorBombState {
    SENSOR_BOMB_STATE_HELD             = 0,
    SENSOR_BOMB_STATE_ARMED_FLYING     = 1, // falls under stage gravity; SensorBomb_Land on a floor contact
    SENSOR_BOMB_STATE_ARMED_STATIONARY = 2, // embedded; the sensor runs from trigger_callback
    SENSOR_BOMB_STATE_EXPLODING        = 3,
    SENSOR_BOMB_STATE_FADE             = 4, // state_id duplicates index 2
} SensorBombState;

typedef enum GordoState {
    GORDO_STATE_HELD             = 0,
    GORDO_STATE_THROWN_ASCENDING = 1, // state_id=1, scales up while flying
    GORDO_STATE_THROWN_TIMED     = 2, // state_id=1 again, self-despawn timer
    GORDO_STATE_DESPAWN          = 3, // sentinel (-1); anim_callback runs particle burst
} GordoState;

typedef enum FireBulletState {
    FIRE_BULLET_STATE_THROWN    = 0,
    FIRE_BULLET_STATE_HIT_PAUSE = 1, // every callback is a bare return
    FIRE_BULLET_STATE_DESPAWN   = 2, // sentinel (-1) with cleanup-watchdog anim_callback
} FireBulletState;

// Auras share structure across fire/needle/ice: IDLE re-snaps to the rider hand
// every frame; FIRING is per-kind.
typedef enum FireAuraState {
    FIRE_AURA_STATE_IDLE    = 0,
    FIRE_AURA_STATE_FIRING  = 1,
    FIRE_AURA_STATE_COOLING = 2,
} FireAuraState;

typedef enum NeedleAuraState {
    NEEDLE_AURA_STATE_IDLE     = 0,
    NEEDLE_AURA_STATE_DEPLOYED = 1,
    NEEDLE_AURA_STATE_SETTLED  = 2,
} NeedleAuraState;

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

// One entry of a kind's state table. All four callbacks are per-frame, running
// at priorities 1/4/5/6. There is no on-enter/on-exit slot - transitions come
// from SetState calls inside these callbacks, and once-per-create work belongs
// in the vtable's init / post_init.
typedef struct WeaponStateEntry
{
    s32   state_id;                          // 0x00: written to wp+0x2c. 0xffffffff = sentinel/terminator
    u32   flags;                             // 0x04: attack word, copied to wp+0x17c; low byte = attack cause
    void (*anim_callback)(void *wp);         // 0x08: WPPRI_ANIM (early AI, timers)
    void (*phys_callback)(void *wp);         // 0x0c: WPPRI_PHYS (pre-physics velocity update)
    void (*envcoll_callback)(void *wp);      // 0x10: WPPRI_ENVCOLL (main state tick, collision response)
    void (*post_envcoll_callback)(void *wp); // 0x14: WPPRI_POSTENVCOLL (aura re-snap)
} WeaponStateEntry;

#define WP_ATTACK_CAUSE_MASK 0xff  // WeaponStateEntry.flags / DmgLog.credited_attack
#define WP_ATTACK_ACTIVE     0x100 // set on every active vanilla state

// What WeaponKindData.model_desc points at. Weapon_CollectParts (0x80221914)
// walks the tree and asserts if it holds a different number of joints, or more than 10.
typedef struct WeaponModelBlock
{
    JOBJDesc *tree;        // +0x00
    u8        joint_num;   // +0x04: the tree's joint count
    u8        anchor_part; // +0x05: part whose world position WPPRI_POSTENVCOLL writes to anchor_pos
    u8        pad_06[2];   // +0x06
} WeaponModelBlock;

// One per state_id. Weapon_BindStateAnim binds the anims onto whatever model the
// weapon loaded; the script carries the hitbox commands.
typedef struct WeaponStateAnimSpec
{
    void       *anim_joint;    // +0x00: may be NULL
    void       *matanim_joint; // +0x04: may be NULL
    const void *script;        // +0x08: run by Weapon_RunScript
    u32         flags;         // +0x0c: bit 6 = loop
} WeaponStateAnimSpec;

// WeaponKindData.params, copied into wp+0xf4..0x100 by Weapon_LoadKindParams.
typedef struct WeaponKindParams
{
    float model_scale; // 0x00: root model scale, times WeaponData.scale (Weapon_SyncRootMtx)
    float cull_scale;  // 0x04: camera visibility radius, times WeaponData.scale
    float x08;         // 0x08: Weapon_InitRuntimeState branches on it; 0 for plasma
    int   lifetime;    // 0x0c: default lifetime in frames, 0 = infinite
} WeaponKindParams;

// WeaponKindData.mpcoll_desc, handed to mpColl_Init by Weapon_RebuildCollShape. The
// radius is not scaled by WeaponData.scale.
typedef struct WeaponCollDesc
{
    float radius;  // 0x00
    Vec3  extents; // 0x04: zero for every round kind
} WeaponCollDesc;

// A sensor bomb's WeaponData.kind_params, a per-bomb copy of the kind data's block.
typedef struct SensorBombParams
{
    float x00;              // 0x00
    int   fuse;             // 0x04: frames from create to self-detonation (2400)
    int   arm_delay;        // 0x08: frames after landing before the sensor scans (60)
    u8    x0c[0x2c - 0x0c]; // 0x0c: explosion size and timing
    float sensor_radius;    // 0x2c: rider hand-bone distance that sets off an embedded bomb (30)
    float anim_rate_start;  // 0x30: blink anim rate at first (1.0)
    float anim_rate_end;    // 0x34: blink anim rate at the end of the ramp (5.0)
    int   anim_rate_frames; // 0x38: frames of burnt fuse the ramp takes (280)
    int   blinks;           // 0x3c: the blink interval is fuse / blinks frames (60)
    int   max_active;       // 0x40: SensorBomb_EnforceMaxActive fades thrown bombs past this count (2)
} SensorBombParams;

// A bomb's WeaponData.kind_params, a per-bomb copy of the kind data's block. The blast
// scale ramps from blast_scale_start to blast_scale_end over ramp_frames, scaling both
// attack regions, the fireball and the wind.
typedef struct BombParams
{
    u8    x00[0x0c];         // 0x00
    int   blast_delay;       // 0x0c: frames in BOMB_STATE_PRIMED (14)
    int   ramp_frames;       // 0x10: blast scale ramp length (450)
    int   self_hit_delay;    // 0x14: blast frames before the bomb can hit its owner (40)
    float quake_amplitude;   // 0x18: (3.0)
    u8    x1c[0x28 - 0x1c];  // 0x1c
    float wind_scale;        // 0x28: wind radius over region 1's
    float blast_scale_start; // 0x2c: (1.0)
    float blast_scale_end;   // 0x30: (170), also the fireball's outer joint and the contact particle
    float light_height;      // 0x34: the blast's area light sits this far above the bomb
    u8    x38[0x84 - 0x38];  // 0x38: light radius, fade and AreaLightData
} BombParams;

// Per-kind data at *(0x8055a9a8 + kind*4). Registration is all-or-nothing and
// rider-driven: Rider_Create feeds the full 17-kind list from RdKirbyAbility.dat
// to Weapon_RegisterKindDataList, so once any rider exists the whole table
// is live. Weapon_ClearKindDataTable zeroes it at system init and
// Weapon_Create dereferences the slot unchecked, so code spawning outside
// the normal rider lifetime must guard on the slot being non-NULL.
typedef struct WeaponKindData
{
    const WeaponKindParams       *params;              // +0x00
    const void                 *kind_params;         // +0x04: tuning block WeaponData.kind_params copies. word0 is
                                                     //        the plasma / spit-star muzzle speed, plasma's
                                                     //        word1 its burst angle in degrees.
    WeaponModelBlock             *model_desc;          // +0x08: NULL falls back to a global default model
    const WeaponStateAnimSpec    *state_anim_spec_array; // +0x0c: indexed by state_id
    const void                 *vuln_region_spec;    // +0x10: vulnerable-region list (0x44 stride). NULL for
                                                     //        every kind but FIRE_BULLET and SENSORBOMB; the
                                                     //        two attack regions are hardcoded in
                                                     //        Weapon_InitHurtData instead.
    const WeaponCollDesc         *mpcoll_desc;         // +0x14: NULL = no environment collision
} WeaponKindData;

// Per-kind vtable at *(0x804b4338 + kind*4). Kind pairs 5/6 and 7/8 alias to
// the same vtable. The table holds exactly 17 entries: 0x804b437c is the
// "WnCommon.dat" string Weapon_LoadCommonArchive reads.
typedef struct WeaponKindVTable
{
    const WeaponStateEntry *state_table; // 0x00: per-kind state entries, loaded into wp+0x34
    void                      (*system_init)(void); // 0x04: run by Weapon_SystemInit on every 3D load
    void                      (*init)(void *wp);          // 0x08: one-shot at create
    void                      (*load_kind_params)(void *wp);   // 0x0c: fills WeaponData.kind_params
    void                      (*reload_kind_params)(void *wp); // 0x10: the same copy again, on a reload
    void                      (*aux_a)(void *wp);         // 0x14: teardown cleanup, run by the dtor
    void                      (*post_init)(void *wp);     // 0x18: one-shot at create, after proc registration
    int                       (*on_hit)(void *wp, void *hit); // 0x1c: prio-10 hit reaction; non-zero destroys
    void                      (*despawn)(void *wp);       // 0x20: Weapon_Despawn's exit; NULL falls back to
                                                            //       GObj_Destroy. Most kinds install a bare one
} WeaponKindVTable;

// Indexed by WeaponKind. Every function slot is NULL-checked before it is called.
// Every reader forms the table's address with a lis / addi pair, so a mod can relocate it.
static WeaponKindVTable **wp_kind_vtables = (WeaponKindVTable **)0x804b4338;

// Indexed by WeaponKind. Slots are NULL until a rider registers the list. The
// word after the 17 slots (0x8055a9ec) is padding that nothing clears or fills.
static WeaponKindData **wp_kind_data = (WeaponKindData **)0x8055a9a8;

// Weapon userdata, reached via gobj->userdata. Known fields only; unknown regions
// are padding.
typedef struct WeaponData
{
    GOBJ          *gobj;                 // 0x00: back-pointer to outer GObj
    WeaponKind kind;                 // 0x04
    GOBJ          *owner_gobj;           // 0x08: self-hit exclusion key and the ply hits credit
    GOBJ          *owner_gobj2;          // 0x0c: desc.owner_gobj2 copy
    u8             pad_10[4];            // 0x10: text/vfx slot ptr (internal)
    u8             owner_byte;           // 0x14: usually 0
    u8             pad_15[0x20 - 0x15];  // 0x15..0x1f
    const WeaponKindData *kind_data;     // 0x20: wp_kind_data[kind]
    int            state;                // 0x24: arg of Weapon_StateChange
    int            common_state_num;     // 0x28: states below this index use common_state_table, the
                                         //       rest state_table. Always 0, so common_state_table is unused.
    s32            state_id;             // 0x2c: the table entry's state_id
    const WeaponStateEntry *common_state_table; // 0x30: never written by vanilla
    const WeaponStateEntry *state_table;        // 0x34: from the vtable's state_table
    const WeaponStateAnimSpec *state_anim_spec; // 0x38: kind_data->state_anim_spec_array[state_id]
    u8             pad_3c[0x70 - 0x3c];  // 0x3c..0x6f: anim accumulator + sub-vtable refs (internal)
    float          base_scale;           // 0x70: desc.scale copy
    float          scale;                // 0x74: live size scale, seeded from base_scale. The shared
                                         //       pipeline only reads it - HurtData size, env sweep radius,
                                         //       render cull radius and the root model scale (times
                                         //       params[0], via Weapon_SyncRootMtx) all come off it
    int            type_flag;            // 0x78: desc.type_flag copy
    Vec3           accel;                // 0x7c: per-frame acceleration, zeroed at prio 0 and integrated
                                         //       into vel at prio 4. Nothing supplies gravity - a
                                         //       ballistic arc must rewrite this every frame.
    Vec3           spawn_vel;            // 0x88: desc.vel snapshot (read-only after create)
    Vec3           vel;                  // 0x94: live physics velocity
    Vec3           pos_delta;            // 0xa0: pos - pos_prev, set at WPPRI_ENDOFFRAME
    Vec3           pos;                  // 0xac: live world position
    Vec3           pos_prev;             // 0xb8: previous frame (used by swept collision)
    Vec3           anchor_pos;           // 0xc4: world position of WeaponModelBlock.anchor_part, rewritten
                                         //       every WPPRI_POSTENVCOLL; the spawn position until then
    u8             pad_d0[0xf4 - 0xd0];  // 0xd0..0xf3: rotation basis (internal)
    float          model_scale;          // 0xf4: params.model_scale copy; scales the model alone
    u8             pad_f8[0x104 - 0xf8]; // 0xf8..0x103: sub-object refs (internal)
    void          *kind_params;          // 0x104: this weapon's copy of kind_data->kind_params (BombParams, ...)
    HurtData      *hurt_data;            // 0x108
    int            lifetime;             // 0x10c: frames remaining, decremented at prio 1. Seeded from
                                         //        kind_data params[3]. 0 means INFINITE - bomb, sensor
                                         //        bomb and the three auras ship this way.
    int            frame_counter;        // 0x110: monotonic, incremented by prio-0
    int            efgroup;              // 0x114: EfGroup from Weapon_AllocEfGroups, freed in the dtor
    int            efgroup2;             // 0x118: second EfGroup
    float          gravity_strength;     // 0x11c: Gr_GetDownVector at pos, refreshed by Weapon_RefreshGravity
    Vec3           gravity_down;         // 0x120: unit down at pos
    u8             pad_12c[0x138 - 0x12c]; // 0x12c..0x137
    CollData      *coll_data;            // 0x138: environment collider, NULL when the kind has no
                                         //        mpcoll_desc. Its sweep is what runs the yakumono
                                         //        break dispatch, and its owner GObj is this weapon
    u8             pad_13c[0x14c - 0x13c]; // 0x13c..0x14b
    float          charge;               // 0x14c: desc.charge copy
    // Copied from the state entry by Weapon_StateChange, which also clears 0x160..0x178
    void         (*anim_callback)(void *p);         // 0x150: WPPRI_ANIM
    void         (*phys_callback)(void *p);         // 0x154: WPPRI_PHYS
    void         (*envcoll_callback)(void *p);      // 0x158: WPPRI_ENVCOLL
    void         (*post_envcoll_callback)(void *p); // 0x15c: WPPRI_POSTENVCOLL
    void         (*framestart_callback)(void *p);   // 0x160: WPPRI_FRAMESTART
    void         (*trigger_callback)(void *p);      // 0x164: WPPRI_TRIGGER
    void         (*user_hook_2)(void *p);           // 0x168: WPPRI_DMGAPPLY
    // WPPRI_DMGAPPLY, when the HurtData took a hit; dmg = &hurt_data->hitcoll_log_idx.
    // Non-zero destroys the weapon.
    int          (*on_damage_callback)(void *p, void *dmg); // 0x16c
    u8             pad_170[0x17c - 0x170]; // 0x170..0x17b
    DmgLog         dmg_log;              // 0x17c: attack_data from the state entry's flags
    u8             pad_19c[0x1b4 - 0x19c]; // 0x19c..0x1b3: hit sub-struct (internal)
    u8             flag_a;               // 0x1b4: bit 0 = WP_ALLOW_SELF_HIT_INBOUND. Other bits set
                                          //        during damage logging.
    u8             flag_b;               // 0x1b5: bit 0 = env-colliding this frame; bit 2 = alive marker
                                          //        (always on); bit 5 = WP_ALLOW_SELF_HIT_OUTBOUND
    u8             flag_c;               // 0x1b6: bit 7 cleared by Weapon_StateChange; lower bits flag
                                          //        effect liveness
    u8             pad_1b7;              // 0x1b7
    union
    {
        // BOMB: 0x1c0 the PRIMED countdown, then the fireball's effect handle; 0x1c8 the blast
        // quake; 0x1d0..0x1ec the blast ramp; 0x1f8/0x1fc the fuse spark. FIRECRACKER:
        // 0x1b8/0x1bc the fuse, independent of lifetime.
        u8 raw[0x218 - 0x1b8];            // 0x1b8..0x217
        // The owner's Fire charge, cached by FireBullet_Init. A custom spawner must seed both
        // (1.0 is full charge) or the shot bursts at zero size with no hitbox.
        struct
        {
            float knockback_scale;        // 0x1b8, charge / max; multiplies each region's base_knockback
            float burst_scale;            // 0x1bc, charge; becomes the burst's scale
        } fire_bullet;
        struct
        {
            u8  x1b8[4];                  // 0x1b8
            int fuse;                     // 0x1bc, frames to self-detonation; 0 stops the countdown
            s16 blink_timer;              // 0x1c0
            s16 arm_delay;                // 0x1c2, frames left before an embedded bomb's sensor scans
        } sensor_bomb;
        // The flight's grow ramp into scale, which PlasmaD_Burst reseeds as the burst's
        // shrink while fading region 0's damage.
        struct
        {
            int ramp_frames;              // 0x1b8, frames of the ramp left
            float ramp_scale;             // 0x1bc, copied into scale each ramp frame
            float ramp_step;              // 0x1c0, added in flight, subtracted in the burst
            float burst_damage;           // 0x1c4
            float burst_damage_step;      // 0x1c8
            void *light;                  // 0x1cc, point light, freed only by the burst's end
        } plasma_d;
    } kind_scratch;
    u8             flag_d;               // 0x218: subproc-gating; bit 0 always set
    u8             pad_219[0x220 - 0x219]; // 0x219..0x21f
} WeaponData;

// The hit pipeline runs two scans per frame - outbound (the weapon walks
// the victim lists) and inbound (each victim walks the weapon list) - and
// each gates owner-exclusion on a different bit. A weapon that must damage
// its own owner has to set both, since either scan can resolve the hit first.
// Vanilla sets neither at create time except sensor bomb's inbound bit.
#define WP_ALLOW_SELF_HIT_INBOUND  0x01  // OR into wp->flag_a (wp+0x1b4)
#define WP_ALLOW_SELF_HIT_OUTBOUND 0x20  // OR into wp->flag_b (wp+0x1b5)

// wp->flag_b bit 0: cleared at WPPRI_ENVCOLL before envcoll_callback, and Weapon_UpdateEnvColl sets it
// on any floor, wall or ceiling contact.
#define WP_FLAGB_ENV_CONTACT 0x01

// Creates a weapon from a pre-filled descriptor, touching no rider bones or
// rider state. The WeaponData hangs off the returned GObj's userdata. Leaves
// the weapon in state 0, which for bomb / sensor bomb / gordo is "held
// in the rider's hand" and needs a Weapon_StateChange before physics or
// detonation runs.
GOBJ *Weapon_Create(WeaponDesc *desc); // 0x8021f428
// Weapon_Create's prio-10 proc. On a hit it calls the vtable's on_hit.
void Weapon_Proc10_HitReact(GOBJ *wp_gobj); // 0x8021fcd4
// Gr_GetDownVector at pos into gravity_strength / gravity_down; from Weapon_Create and
// every frame from the prio-7 proc. Falling kinds also sample it themselves each frame.
void Weapon_RefreshGravity(WeaponData *wp); // 0x80220578
// The WeaponData destructor every ending runs through: frees the HurtData and EfGroups,
// runs the kind's aux_a, then frees the rest.
void Weapon_UserDataDtor(WeaponData *wp); // 0x8021ff54

// Transitions to the given state entry index. Does NOT touch physics velocity -
// set that first, mirroring vanilla throw() ordering. anim_frame/anim_rate are
// handed to Weapon_BindStateAnim for the new state's animation (vanilla passes
// 1.0 for both). flags bit 0 skips a rider-attached cleanup path; vanilla passes 1
// for THROWN transitions and 0 for the initial HELD setup.
void Weapon_StateChange(WeaponData *wp, int state,
                        float anim_frame, float anim_rate, int flags); // 0x8021f7dc

// Returns wp->owner_gobj; used by every victim-side self-hit exclusion check.
GOBJ *WeaponGObj_GetOwnerGObj(GOBJ *wp_gobj); // 0x8022312c

// The ply a weapon's hit credits: the owner's, when the owner is a rider or a
// ridden machine, else 5 (nobody).
int WeaponGObj_GetOwnerPly(GOBJ *wp_gobj); // 0x802230c4

// &dmg_log, the attack block Machine_StoreAttacker copies onto a victim's DmgLog.
// Its first word is the state entry's flags, so it names the kind, not the weapon.
DmgLog *WeaponGObj_GetAttackerLog(GOBJ *wp_gobj); // 0x80223178

// Weapon_Despawn by outer GObj handle. Vanilla uses it from the Fire/Needle/Ice
// LoseAbility handlers to destroy the held aura at rider+0x3F0.
void WeaponGObj_Despawn(GOBJ *wp_gobj); // 0x802230a0

// Resets dmg_log to a state entry's attack word: attack_data, hits.x0 and
// hits.victim_mask. A new attack cause also mints a fresh dmg_log.attack_id.
void Weapon_AssignStateFlags(WeaponData *wp, int flags); // 0x80222298
void Weapon_ClearAttackBlock(WeaponData *wp);            // 0x80222240
void WeaponGObj_RenewAttackId(GOBJ *wp_gobj);            // 0x8022230c, AllocSeqId16 into dmg_log.attack_id

WeaponKind WeaponGObj_GetKind(GOBJ *wp_gobj); // 0x80223184

// Every 3D load: resets the weapon pools, clears the kind-data table, loads
// WnCommon.dat and runs each vtable's system_init.
void Weapon_SystemInit(void);        // 0x8021f1fc
void Weapon_LoadCommonArchive(void); // 0x80220194
// Fills only the NULL slots of wp_kind_data from a {count, entries} list of
// {kind, data} pairs, without bounds-checking the kind.
void Weapon_RegisterKindDataList(void *list); // 0x802201e0
// kind_data->params into wp+0xf4..0x100, then the vtable's load_kind_params.
void Weapon_LoadKindParams(WeaponData *wp); // 0x802205e8
// The same params copy, then the vtable's load_kind_params and reload_kind_params.
// Called once, from Weapon_Create.
void Weapon_ReloadKindParams(WeaponData *wp); // 0x80220654
// Rebuilds the root JObj matrix from the basis and position, scaled by
// scale * params[0].
void Weapon_SyncRootMtx(WeaponData *wp);                            // 0x80220310
void Weapon_BindStateAnim(WeaponData *wp, float frame, float rate); // 0x80220b20
// Advances the state animation under the weapon's efgroup, then its script.
void Weapon_AnimThink(WeaponData *wp);                        // 0x802208fc
void Weapon_RunScript(WeaponData *wp);                        // 0x802211cc
void Weapon_AllocEfGroups(WeaponData *wp);                    // 0x80221b04, efgroup / efgroup2
void Weapon_CreateEnvColl(WeaponData *wp);                    // 0x80221cf0
int  Weapon_HasFloorContact(WeaponData *wp);                  // 0x80222144, under_rec_num != 0
void Weapon_GetFloorContactNormal(WeaponData *wp, Vec3 *out); // 0x802221b8

// Turns forward toward the homing tracker's target by at most turn_deg, then sets
// velocity along it, decaying any excess speed toward `speed`.
void Weapon_HomingSteer(float turn_deg, float speed, WeaponData *wp); // 0x80223298
void Weapon_HomingUpdateTracker(WeaponData *wp);                // 0x802231e8
// The tracker at wp+0x13c. It registers itself on its target rider and must be
// freed through HomingTracker_Destroy.
void *HomingTracker_Create(GOBJ *owner, const float *params); // 0x8022438c
void  HomingTracker_Destroy(void *tracker);                   // 0x80224444
void  HomingTracker_Acquire(void *tracker);                   // 0x80223738
GOBJ *HomingTracker_FindNearest(void *tracker);               // 0x80223400

// Run by Weapon_Create after proc registration: seeds scale, lifetime, the animation
// accumulators and the alive flags. A weapon is unusable before it runs.
void Weapon_InitRuntimeState(WeaponData *wp); // 0x8021f2a0

// Ends a weapon through its kind's despawn slot, else GObj_Destroy. The prio-1 proc
// calls it the frame lifetime reaches zero.
void Weapon_Despawn(WeaponData *wp); // 0x80220364

// Runs one frame of environment collision on wp->coll_data: position update,
// map sweep and pushback. Every kind whose envcoll_callback does env collision calls it
// first; a mod that replaces envcoll_callback has to call it to keep the sweep - and
// with it any scene-object break - running.
void Weapon_UpdateEnvColl(WeaponData *wp); // 0x80221fd4

// Flattens the weapon's JObj tree into its part array, asserting on more than 10
// joints or a count disagreeing with the joint count in the kind's WeaponModelBlock.
void Weapon_CollectParts(WeaponData *wp); // 0x80221914

// WPKIND_PLASMA_SPREAD_MID and _SIDE share one vtable and one state. Only the
// prio-5 tick does anything: it runs the environment sweep, then bursts the shot on a
// steep contact. The rest are bare returns, and despawn is a plain GObj_Destroy.
void PlasmaSpread_Init(void *wp);                 // 0x8022691c
void PlasmaSpread_State0_Anim(void *wp);          // 0x802269f4
void PlasmaSpread_State0_Phys(void *wp);          // 0x802269f8
void PlasmaSpread_State0_EnvCollide(void *wp);    // 0x802269fc
void PlasmaSpread_State0_PostEnvColl(void *wp);   // 0x80226abc
void PlasmaSpread_Despawn(void *wp);              // 0x80226b1c
// SetState(0), then velocity = spawn velocity + forward * kind_params word0.
void PlasmaSpread_PostInit(void *wp);             // 0x80226960
int  PlasmaSpread_OnHit(void *wp, void *hit);     // 0x80226ac8, always 0

// WPKIND_PLASMA_D, the Plasma ability's piercing full-charge ball: state 0 flies, state 1
// bursts. Only Weapon_Create needs a real owner, for RiderGObj_GetPly; a class-0 stand-in
// whose userdata byte 8 is PLY_NUM can carry the create and then be cleared.
void PlasmaD_Init(WeaponData *wp);                // 0x80227a80, owner homing tracker and the point light
// The grow ramp from scale 0.05 to 0.7 over 30 frames, overwriting desc.scale.
void PlasmaD_PostInit(WeaponData *wp);            // 0x80227b48
void PlasmaD_GrowThink(WeaponData *wp);           // 0x80227bf0, state 0 anim_callback
// State 0 envcoll_callback: bursts on a floor at 150 degrees or more to the velocity.
void PlasmaD_EnvCollide(WeaponData *wp);          // 0x80227c60
void PlasmaD_Burst(WeaponData *wp);               // 0x80227d38, the despawn: state 1 for 10 frames
void PlasmaD_BurstThink(WeaponData *wp);          // 0x80227e34, state 1 anim_callback; frees the light and the ball
// Halves a nonzero lifetime on each rider or machine hit, bursting at 0. Always 0.
int  PlasmaD_OnHit(WeaponData *wp, void *hit);    // 0x80227f68

void SpitStar_Init(void *wp);                    // 0x802244cc, creates the homing tracker
void SpitStar_State0_Homing(void *wp);           // 0x802245f0

// Rider-side spawn helpers used by copy abilities. All assert on the rider
// having the matching ability hat model loaded. Unless passed in, position/forward/up
// come from the rider's hand bone, velocity from the machine's world_velocity plus
// rider->self_vel.
void Rider_SpawnStarBullet(RiderData *rd, int flag);            // 0x801a8c80, WPKIND_SPITSMALL / WPKIND_SPITLARGE
void Rider_SpawnStarBulletCharged(RiderData *rd);               // 0x801a8df8, WPKIND_SPITCHARGED
// forward is re-orthogonalized against up; vanilla passes &rd->pos, &rd->up, &rd->forward.
void Rider_SpawnFireBullet(RiderData *rd, Vec3 *pos, Vec3 *up, Vec3 *forward); // 0x801a8f68, WPKIND_FIRE_BULLET
void Rider_SpawnFireAura(RiderData *rd, int kind_variant);      // 0x801a9178, WPKIND_FIRE_AURA
void Rider_SpawnBomb(RiderData *rd);                            // 0x801a9410, WPKIND_BOMB
void Rider_SpawnPlasmaBullet(RiderData *rd, int charge_mode);   // 0x801a95a0, WPKIND_PLASMA_{A,B,C,D}
void Rider_SpawnPlasmaSpread(RiderData *rd, int which);         // 0x801a9870, WPKIND_PLASMA_SPREAD_{MID,SIDE}
void Rider_SpawnNeedleAura(RiderData *rd);                      // 0x801a9a54, WPKIND_NEEDLE_AURA
void Rider_SpawnIceAura(RiderData *rd);                         // 0x801a9b84, WPKIND_ICE_AURA
// Same argument shape as Rider_SpawnFireBullet.
void Rider_SpawnCrackerBullet(RiderData *rd, Vec3 *pos, Vec3 *up, Vec3 *forward); // 0x801a9cb4, WPKIND_FIRECRACKER
void Rider_SpawnSensorBomb(RiderData *rd);                      // 0x801a9e78, WPKIND_SENSORBOMB
void Rider_SpawnGordo(RiderData *rd);                           // 0x801aa028, WPKIND_GORDO

// Throw / transition wrappers, mutating an already-created weapon in place.
void WeaponGObj_TryThrowBomb(GOBJ *wp_gobj, Vec3 *unused, Vec3 *vel);      // 0x801a9580 -> 0x80225824
void WeaponGObj_TryThrowSensorBomb(GOBJ *wp_gobj, Vec3 *vel);              // 0x801a9fe8 -> 0x80228f08
// 1 iff the gordo is in GORDO_STATE_DESPAWN with flag_c bit 5 set.
int  WeaponGObj_IsGordoThrowable(GOBJ *wp_gobj);                           // 0x801aa008 -> 0x8022a244

// Gordo HELD -> THROWN_ASCENDING. Reads orientation from owner_gobj's rider
// data rather than a hand-bone matrix, so it works whenever owner_gobj is a
// valid rider GObj even with no Phan-Phan ability initialised. It also seeds all
// the per-kind scratch gordo state 1 reads each frame (spin, decay, impulse,
// lifetime), which a bare Weapon_StateChange(wp, 1, ...) leaves at zero.
void WeaponGObj_EnterGordoThrownState(GOBJ *wp_gobj, Vec3 *vel, Vec3 *pos); // 0x8022a544

// BOMB_STATE_THROWN's env contact: quake, particle and sound, then BOMB_STATE_PRIMED.
void Bomb_DetonationTrigger(WeaponData *wp); // 0x80225c8c

// Sets the fuse and the blink, enters HELD and starts the held sparkle.
void SensorBomb_PostInit(WeaponData *wp); // 0x80228d8c
// HELD -> ARMED_FLYING with vel, then SensorBomb_EnforceMaxActive; a no-op once the fuse is 0.
void SensorBomb_TryThrow(GOBJ *wp_gobj, Vec3 *vel); // 0x80228f08
// The ARMED_FLYING floor contact: ARMED_STATIONARY, with SensorBomb_SensorThink as trigger_callback.
void SensorBomb_Land(WeaponData *wp); // 0x802290bc
// After arm_delay, detonates when any rider's hand bone, the owner's included, is within
// sensor_radius.
void SensorBomb_SensorThink(WeaponData *wp); // 0x802292f8
// Both armed states' anim_callback: burns the fuse and blinks, detonating at 0.
void SensorBomb_FuseThink(WeaponData *wp); // 0x80229e64
void SensorBomb_Detonate(WeaponData *wp);  // 0x802295a8, -> EXPLODING
// The on_damage_callback SensorBomb_TryThrow installs: a hit in flight detonates the bomb.
int  SensorBomb_OnDamage(WeaponData *wp, void *dmg); // 0x80229a2c, always 0
// Fades each thrown sensor bomb past its max_active, counting every owner's together.
void SensorBomb_EnforceMaxActive(void); // 0x8022a270
// Sends each thrown Gordo past its kind_params cap (+0x1c) to GORDO_STATE_DESPAWN, counting
// every owner's together.
void Gordo_EnforceMaxActive(void); // 0x8022b45c

#endif // MEX_H_WEAPON
