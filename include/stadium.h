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

typedef struct StadiumResults
{
    u8 ply_placement[4];  // 0xbc8
    int xbcc;             // 0xbcc
    int ply_race_time[4]; // 0xbd0
    int xbe0;             // 0xbe0
    int xbe4;             // 0xbe4
    int xbe8;             // 0xbe8
    int xbec;             // 0xbec
    int xbf0[4];          // 0xbf0
    u8 xc00[4];           // 0xc00
    int ply_points[4];    // 0xc04, 0x40
    float ply_dist[4];    // 0xc14
} StadiumResults;

static u8 *stc_stadium_option_to_kind = (u8 *)0x80535a9c;

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
int Gm_StadiumRoundNum();

// Direct bitfield writes - bypass the checklist cache.
// Use these for permanent modifications (e.g., AP unlocks) that must
// survive cache invalidation. The ROM Write/Check functions above
// route through a temporary cache when the checklist menu is active,
// and writes to the cache are discarded when the menu closes.
static inline void Gm_StadiumSetUnlockedDirect(StadiumKind kind)
{
    *(volatile u32 *)0x80536EE8 |= (1 << kind);
}

static inline void Gm_StadiumClearUnlockedDirect(StadiumKind kind)
{
    *(volatile u32 *)0x80536EE8 &= ~(1 << kind);
}

static inline void Gm_StadiumSetNewLabelDirect(StadiumKind kind)
{
    *(volatile u32 *)0x80536EEC |= (1 << kind);
}

static inline void Gm_StadiumClearNewLabelDirect(StadiumKind kind)
{
    *(volatile u32 *)0x80536EEC &= ~(1 << kind);
}

#endif