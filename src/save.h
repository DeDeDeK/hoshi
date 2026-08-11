#ifndef SAVE_H
#define SAVE_H

#include "hoshi.h"
#include "os.h"

// Bumped when KARPlusSave's fixed layout changes. Informational only - saves are
// never migrated, so an older card file has to be deleted by hand.
#define VERSION_MAJOR 4
#define VERSION_MINOR 0

// RGB5A3 icon frames reserved in the card tile. At 2KB each these dominate the
// save size - 1 frame keeps it inside one 8KB block.
#define HOSHI_SAVE_ICON_FRAMES 1

// Reserve a 96x32 RGB5A3 card banner ahead of the icon. Costs 6KB, pushing the
// save past one 8KB block, so leave it 0 unless a mod supplies one.
#define HOSHI_SAVE_BANNER 1

#define SAVE_SIZE ((sizeof(KARPlusSave) + CARD_BLOCK_SIZE - 1) / CARD_BLOCK_SIZE) * CARD_BLOCK_SIZE

typedef enum DataKind
{
    DATAKIND_SETTINGS,
    DATAKIND_USER,
} DataKind;

// Card-manager tile embedded in the save file - the CARDStat directory entry
// points at these bytes by offset, so they must live inside it. A banner must
// come immediately before the icon: the CARD library derives the icon's offset
// as iconAddr + banner size, making the image block one contiguous run. All
// images are RGB5A3. Left zeroed for a blank tile if no mod sets one.
typedef struct KARPlusSaveTile
{
    char comment[CARD_COMMENT_SIZE]; // title[0..31] + description[32..63]
#if HOSHI_SAVE_BANNER
    u16 banner[CARD_BANNER_WIDTH * CARD_BANNER_HEIGHT];                   // RGB5A3, precedes the icon
#endif
    u16 icon[HOSHI_SAVE_ICON_FRAMES][CARD_ICON_WIDTH * CARD_ICON_HEIGHT]; // RGB5A3 frames
    u8 frame_num;                                                         // valid frames (0 = no icon)
    u8 speed;                                                             // CARD_STAT_SPEED_* applied to every frame
    u8 is_set;                                                            // a mod supplied a tile
    u8 has_banner;                                                        // a mod supplied a banner
} KARPlusSaveTile;

typedef struct KARPlusSave
{
    u8 version_major;
    u8 version_minor;
    u8 mod_num;
    u8 pad;
    // Placed before metadata[] so the icon starts within the file's first 512
    // bytes - CARDSetStatus rejects an iconAddr at or past 512, and metadata[]
    // alone is 1000 bytes.
    KARPlusSaveTile tile;
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
    u8 data[];
} KARPlusSave;

void KARPlusSave_Init();
void KARPlusSave_SetIconFile(const char *title, const char *description, const char *icon_file, int frame_num, int speed);
void KARPlusSave_SetBannerFile(const char *banner_file);
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