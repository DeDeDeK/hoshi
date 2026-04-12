#ifndef KAR_H_TOPRIDE
#define KAR_H_TOPRIDE

#include "datatypes.h"

// Top Ride Kirby (player) system — completely separate from 3D mode Rider/Machine.
// Top Ride does NOT use Player_Create, Rider_Create, Machine_Create, stc_playerdata,
// RiderData, or MachineData. It uses minor 19 (not 18), so On3DLoadEnd doesn't fire.
// See docs/topride-system.md for full documentation.

// Charge component — inline sub-object starting at TopRideKirby+0x80.
// Initialized by TopRide_KirbyChargeInit (0x802d1fe8).
// Per-frame update in TopRide_ChargeUpdate (0x802df900).
typedef struct TopRideChargeComponent
{
    void *kirby_ptr;          // 0x00, pointer back to TopRideKirby base
    u8 x04[0x04];            // 0x04
    Vec3 position;            // 0x08
    Vec3 facing_dir;          // 0x14
    Vec3 velocity;            // 0x20
    u8 is_charging;           // 0x2C, 1 = A button held (charging), 0 = idle
    u8 charge_ready;          // 0x2D, 1 = charge depleted to 0.0, can accumulate again
    u8 x2E[0x02];            // 0x2E
    float speed_factor;       // 0x30, per-frame speed scaling
    float charge_value;       // 0x34, current charge level (0.0 to ~1.0)
    float prev_charge;        // 0x38, previous frame's charge value
    float charge_at_release;  // 0x3C, snapshot of charge_value at moment of A release
    float angular_velocity;   // 0x40, rotation rate from steering
} TopRideChargeComponent;

// Per-player Kirby object. Vtable at 0x804d2304, RTTI name "Kirby".
// Created by TopRide_KirbyInit (0x802d4d64). ~0x500+ bytes total.
typedef struct TopRideKirby
{
    void *vtable;                       // 0x00
    void *session_data;                 // 0x04
    u8 x08[0x04];                       // 0x08
    u8 player_slot;                     // 0x0C
    u8 char_type;                       // 0x0D
    u8 cpu_level;                       // 0x0E, 0 = human, 1-3 = CPU difficulty
    u8 x0F;                             // 0x0F
    u8 is_active;                       // 0x10
    u8 x11[0x3B];                       // 0x11
    Vec3 position;                      // 0x4C
    Vec3 target_pos;                    // 0x58
    int angles[3];                      // 0x64
    u8 x70[0x0C];                       // 0x70
    void *state_handler;                // 0x7C, input/state handler (charge state machine)
    TopRideChargeComponent charge;      // 0x80, inline charge component
} TopRideKirby;

// KirbyMgr singleton — top-level manager for all Top Ride players.
// Created by TopRide_FielderInit (0x802dafb4). ~0x4080 bytes total.
typedef struct TopRideKirbyMgr
{
    void *vtable;                       // 0x00
    void *game_config;                  // 0x04
    TopRideKirby *kirbys[4];            // 0x08, per-player Kirby pointers (NULL if slot unused)
} TopRideKirbyMgr;

// KirbyMgr singleton pointer. NULL when not in Top Ride gameplay.
static TopRideKirbyMgr **stc_topride_kirbymgr = (TopRideKirbyMgr **)(0x805dd0e0 + 0xA64); // 0x805ddb44

#endif // KAR_H_TOPRIDE
