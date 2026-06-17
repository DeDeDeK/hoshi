#ifndef SAVE_H
#define SAVE_H

#include "hoshi.h"
#include "os.h"

// Bumped when KARPlusSave's fixed layout changes (2: added the card-tile header). Saves are not
// migrated - a file of an unexpected size simply fails to load (the player is expected to start
// fresh), so this is informational only.
#define VERSION_MAJOR 2
#define VERSION_MINOR 0

// Number of RGB5A3 icon frames reserved in the card tile. Each frame is CARD_ICON_SIZE_RGB5A3
// (2KB), so this dominates the save size: 1 frame keeps the save at one 8KB block; raise it (up
// to CARD_ICON_MAX) for an animated icon at the cost of a larger save.
#define HOSHI_SAVE_ICON_FRAMES 1

#define SAVE_SIZE ((sizeof(KARPlusSave) + CARD_BLOCK_SIZE - 1) / CARD_BLOCK_SIZE) * CARD_BLOCK_SIZE

typedef enum DataKind
{
    DATAKIND_SETTINGS,
    DATAKIND_USER,
} DataKind;

// Card-manager tile (comment + animated icon) embedded in the save file. The CARDStat directory
// entry points at these bytes by offset, so it must live inside the file. RGB5A3, no banner.
// Populated via Hoshi_SetSaveIcon -> KARPlusSave_SetIcon; left zeroed (blank tile) if no mod sets it.
typedef struct KARPlusSaveTile
{
    char comment[CARD_COMMENT_SIZE];                                      // title[0..31] + description[32..63]
    u16 icon[HOSHI_SAVE_ICON_FRAMES][CARD_ICON_WIDTH * CARD_ICON_HEIGHT]; // RGB5A3 frames
    u8 frame_num;                                                         // valid frames (0 = no icon)
    u8 speed;                                                             // CARD_STAT_SPEED_* applied to every frame
    u8 is_set;                                                            // a mod supplied a tile
    u8 pad;
} KARPlusSaveTile;

typedef struct KARPlusSave
{
    u8 version_major;
    u8 version_minor;
    u8 mod_num;
    struct
    {
        int mod_hash;       // mod identifier
        struct              //
        {                   //
            int major : 15; //
            int minor : 16; //
        } version;          //
        int is_present : 1; // bool indicating the mod is currently in use
        struct
        {
            int offset : 16; // offset relative to KARPlusSave::data
            int size : 16;   // size of the data
        } menu_data;
        struct
        {
            int offset : 16; // offset relative to KARPlusSave::data
            int size : 16;   // size of the data
        } user_data;
    } metadata[50];
    KARPlusSaveTile tile; // card-manager tile, fixed header ahead of the per-mod data region
    u8 data[];
} KARPlusSave;

void KARPlusSave_Init();
void KARPlusSave_SetIcon(const char *title, const char *description, const void *frames_rgb5a3, int frame_num, int speed);
int KARPlusSave_CheckModDataExists(GlobalMod *mod);
void KARPlusSave_GetModSaveData(GlobalMod *mod, void **out_menu_data, int *out_menu_data_size, void **out_user_data, int *out_user_data_size);
void *KARPlusSave_Alloc(GlobalMod *mod, int menu_size, int user_size);
int KARPlusSave_CreateOrLoad();
KARPlusSave *KARPlusSave_Get();
int KARPlusSave_Write();
int KARPlusSave_VerifySize(GlobalMod *mod, int menu_size, int user_size);
int KARPlusSave_CheckFreeData();

int _Hoshi_GetBackupSize();
int _Hoshi_BackupModSave(u8 *save);
void _Hoshi_RestoreModSave(u8 *save, int num);
#endif