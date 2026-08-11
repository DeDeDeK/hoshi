#ifndef KAR_H_STADIUM
#define KAR_H_STADIUM

#include "datatypes.h"

typedef enum StadiumKind
{
    STKIND_DRAG1,
    STKIND_DRAG2,
    STKIND_DRAG3,
    STKIND_DRAG4,
    STKIND_AIRGLIDER,
    STKIND_TARGETFLIGHT,
    STKIND_HIGHJUMP,
    STKIND_MELEE1,
    STKIND_MELEE2,
    STKIND_DESTRUCTION1,
    STKIND_DESTRUCTION2,
    STKIND_DESTRUCTION3,
    STKIND_DESTRUCTION4,
    STKIND_DESTRUCTION5,
    STKIND_SINGLERACE1,
    STKIND_SINGLERACE2,
    STKIND_SINGLERACE3,
    STKIND_SINGLERACE4,
    STKIND_SINGLERACE5,
    STKIND_SINGLERACE6,
    STKIND_SINGLERACE7,
    STKIND_SINGLERACE8,
    STKIND_SINGLERACE9,
    STKIND_VSKINGDEDEDE,
    STKIND_NUM,
} StadiumKind;

static const char *const StadiumKind_Names[STKIND_NUM] = {
    [STKIND_DRAG1]         = "Drag Race 1",
    [STKIND_DRAG2]         = "Drag Race 2",
    [STKIND_DRAG3]         = "Drag Race 3",
    [STKIND_DRAG4]         = "Drag Race 4",
    [STKIND_AIRGLIDER]     = "Air Glider",
    [STKIND_TARGETFLIGHT]  = "Target Flight",
    [STKIND_HIGHJUMP]      = "High Jump",
    [STKIND_MELEE1]        = "Kirby Melee 1",
    [STKIND_MELEE2]        = "Kirby Melee 2",
    [STKIND_DESTRUCTION1]  = "Destruction Derby 1",
    [STKIND_DESTRUCTION2]  = "Destruction Derby 2",
    [STKIND_DESTRUCTION3]  = "Destruction Derby 3",
    [STKIND_DESTRUCTION4]  = "Destruction Derby 4",
    [STKIND_DESTRUCTION5]  = "Destruction Derby 5",
    [STKIND_SINGLERACE1]   = "Single Race 1",
    [STKIND_SINGLERACE2]   = "Single Race 2",
    [STKIND_SINGLERACE3]   = "Single Race 3",
    [STKIND_SINGLERACE4]   = "Single Race 4",
    [STKIND_SINGLERACE5]   = "Single Race 5",
    [STKIND_SINGLERACE6]   = "Single Race 6",
    [STKIND_SINGLERACE7]   = "Single Race 7",
    [STKIND_SINGLERACE8]   = "Single Race 8",
    [STKIND_SINGLERACE9]   = "Single Race: Nebula Belt",
    [STKIND_VSKINGDEDEDE]  = "Vs. King Dedede",
};

typedef enum StadiumGroup
{
    STGROUP_DRAGRACE,
    STGROUP_AIRGLIDER,
    STGROUP_TARGETFLIGHT,
    STGROUP_HIGHJUMP,
    STGROUP_MELEE,
    STGROUP_DESTRUCTION,
    STGROUP_SINGLERACE,
    STGROUP_VSKINGDEDEDE,
    STGROUP_NUM,
} StadiumGroup;

// End-of-round snapshot of the live per-player result arrays, latched into
// GameData by Stadium_ExitMinor's copy loop (0x80015164) right before the
// On3DExit hook site, so every field here is final while On3DExit runs.
//
// The loop runs p = 0..3 unconditionally - CPU racers are latched too, and
// Stadium_ComputeRank* rank them alongside humans. It is skipped entirely when
// 3D_CheckIfReplay() (GameData.is_replay) or 3DHud_GetUnkFromPKind()
// (major_cur == MJRKIND_TITLE) holds, leaving the previous round's values in
// place - check is_replay before reading.
//
// Which field carries the round's score depends on the stadium: the ranker
// picked by GameData.city_kind is by time (0/7/15), by points (9), or by
// distance (1/8/11/16).
typedef struct StadiumResults
{
    u8 ply_placement[4];   // 0xbc8, from GameData.player_finish_rank; 0 = 1st
    u8 ply_finished[4];    // 0xbcc, from GameData.player_finished_flag; crossed the goal
    int ply_race_time[4];  // 0xbd0, from GameData.player_finish_time; frames @60fps, 0 = DNF
    int ply_freerun_time[4]; // 0xbe0, from GameData+0x8cc (Free Run's own time slot)
    int ply_lap_count[4];  // 0xbf0, from GameData.player_lap_count
    u8 xc00[4];            // 0xc00, from GameData+0xa30; validity gate - must be 0 to record
    int ply_points[4];     // 0xc04, from GameData.destruction_derby_ko_num; polymorphic
                           //        score - Target Flight points / Derby KOs / Melee KOs
    float ply_dist[4];     // 0xc14, from GameData.player_race_distance; METRES
                           //        (feet = metres / 0.3048)
} StadiumResults;

StadiumGroup Gm_GetStadiumGroupFromKind(StadiumKind st_kind);
StadiumKind Gm_GetCurrentStadiumKind();
StadiumGroup Gm_GetCurrentStadiumGroup();
int Gm_StadiumIsDefaultUnlocked(StadiumKind kind);  // 8000C148
int Gm_StadiumIsUnlocked(StadiumKind kind);          // 8000C17C
int Gm_StadiumIsAvailable(StadiumKind kind);          // 8000C228 - composite check (default + checklist + bitfield)
int Gm_StadiumCheckUnlocked(StadiumKind kind);       // 80007EE4 - reads unlock bitfield (handles cache)
void Gm_StadiumWriteUnlocked(StadiumKind kind, int unlock); // 80007F6C - writes unlock bitfield (handles cache)
int Gm_StadiumCheckNewLabel(StadiumKind kind);        // 80008038 - reads new-label bitfield (handles cache)
void Gm_StadiumWriteNewLabel(StadiumKind kind, int set);    // 800080C0 - writes new-label bitfield (handles cache)
// StadiumResults accessors. All take the player index and index the results
// block directly, so they carry CPU entries and stay valid until the next
// stadium overwrites them.
u8 Ply_GetStadiumRacePlacement(int ply); // 0x8000B674 - 0 = 1st
int Ply_GetStadiumRaceTime(int ply);     // 0x8000B6AC - frames @60fps, 0 = DNF
int Ply_GetStadiumPoints(int ply);       // 0x8000B75C
float Ply_GetStadiumDistance(int ply);   // 0x8000B798 - metres

// Increments GameData.city.stadium_round (stb r0,0x5af(r31) at 0x800406FC)
// before returning - calling it to read the round corrupts the counter.
int Gm_StadiumRoundNum();
u8 Gm_GetStadiumRound(); // 0x8000AE08 - the pure read of GameData.city.stadium_round

// The two StadiumKind-indexed bitfields the Write/Check functions above front.
// Touching them directly bypasses the temporary cache those route through while
// the checklist menu is open - whose writes are discarded when it closes - so a
// permanent unlock has to be written here.
static volatile u32 *stc_stadium_unlocked = (volatile u32 *)0x80536EE8;
static volatile u32 *stc_stadium_new_label = (volatile u32 *)0x80536EEC;

#endif