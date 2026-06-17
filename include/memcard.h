#ifndef MEX_H_MEMCARD
#define MEX_H_MEMCARD

#include "obj.h"

typedef enum MemcardStatus
{
    MEMCARD_SUCCESS,
    MEMCARD_NOFILE = 6,
    MEMCARD_BUSY = 12,
    MEMCARD_ERROR = 14,
} MemcardStatus;

typedef enum MemcardSaveStatus
{
    CARDSAVE_EXIST = 0,
    CARDSAVE_NOEXIST = 2, // will prompt to insert a memcard on scene change
    CARDSAVE_IGNORE = 4,
} MemcardSaveStatus;

typedef struct MemcardUnk
{
    int x0;               //
    int x4;               //
    int x8;               //
    MemcardStatus status; // 0xc
    GOBJ *save_cam_gobj;  // 0x10
    int x14;              // 0x10
    int x18;              // 0x10
    int x1c;              // 0x10
    int x20;              // 0x20
    char *save_name;      // 0x24
} MemcardUnk;

typedef struct SaveInfo
{
    u8 x0[0x54c];  // 0x0
    int save_size; // 0x54c
} SaveInfo;

// Vanilla game-save card tile (managed by the threaded GCP_MemCard worker, 0x8045xxxx):
//   *(stc_save_info + 0x64) -> icon format/speed descriptor (byte[0..1] feed bannerFormat;
//     per-frame entries carry format at +2 and speed at +10).
//   Tile assets are publics in LbMcGame.dat: MemCardBanner_01 (96x32 RGB5A3),
//     MemCardIcon_01 (8-frame 32x32 CI8 + 256-entry RGB5A3 TLUT).
//   The create path writes commentAddr=0, iconAddr=0x40, bannerFormat=NONE (icon-only tile).

MemcardStatus Memcard_GetState();
void Memcard_RequestCheckExist(int slot, char *save_name);
MemcardSaveStatus Memcard_GetSaveStatus();
void Memcard_SetSaveStatus(MemcardSaveStatus);

static MemcardUnk *stc_memcard_unk = (MemcardUnk *)0x805528f8;
static SaveInfo *stc_save_info = (SaveInfo *)0x8059a880;

#endif