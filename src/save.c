/*---------------------------------------------------------------------------*
    Handles lowest level save file operations.
        - allocates runtime save struct
        - creates save file on memcard
        - loads save file from memcard
        - returns runtime save file struct
 *---------------------------------------------------------------------------*/

#include "memcard.h"
#include "os.h"
#include "hsd.h"
#include "inline.h"
#include "game.h"
#include <string.h>
#include <stddef.h>

#include "save.h"
#include "hash.h"
#include "hoshi.h"
#include "settings.h"

#include "hoshi/func.h"
#include "hoshi/log.h"

#include "code_patch/code_patch.h"

static char *save_name = "hoshi";
static KARPlusSave *stc_hoshi_save;
static int stc_hoshi_save_hash;

// Disc filenames (without ".dat") for the tile art, registered at mod boot and read into the tile
// during save create (KARPlusSave_LoadTileArt). Kept out of the tile itself so no path string is
// written to the card. NULL = no art of that kind. The names must contain no "_" or "." - the DVD
// loader only appends ".dat" to names without one (any "_"/"." makes it treat the name as complete).
static const char *stc_tile_icon_file;
static const char *stc_tile_banner_file;

extern ModloaderData *stc_modloader_data;

/*---------------------------------------------------------------------*
Name:           KARPlusSave_SetIconFile

Description:    Public API (Hoshi_SetSaveIconFile). Registers a card-manager
                comment + icon for the shared "hoshi" save file. The comment
                and animation params are stored in the tile now; the icon
                pixels are read from icon_file on disc during save create
                (KARPlusSave_LoadTileArt), so no large image is carried in the
                mod's code. Call at mod boot.

Arguments:      title       - card title string (<= 31 chars used)
                description - card description string (<= 31 chars used)
                icon_file   - disc filename without ".dat" (and no "_" or ".");
                              a raw frame_num * CARD_ICON_SIZE_RGB5A3 RGB5A3 blob
                frame_num   - 1..HOSHI_SAVE_ICON_FRAMES
                speed       - CARD_STAT_SPEED_FAST/MIDDLE/SLOW (all frames)

Returns:        none.

*---------------------------------------------------------------------*/
void KARPlusSave_SetIconFile(const char *title, const char *description, const char *icon_file, int frame_num, int speed)
{
    if (!stc_hoshi_save)
        return;

    if (frame_num < 1)
        frame_num = 1;
    if (frame_num > HOSHI_SAVE_ICON_FRAMES)
        frame_num = HOSHI_SAVE_ICON_FRAMES;
    if (speed < CARD_STAT_SPEED_FAST || speed > CARD_STAT_SPEED_SLOW)
        speed = CARD_STAT_SPEED_MIDDLE;

    KARPlusSaveTile *tile = &stc_hoshi_save->tile;

    memset(tile, 0, sizeof(*tile)); // clears comment + (deferred) image regions + has_banner
    // comment is two 32-byte halves: title then description (strncpy leaves the rest zeroed)
    if (title)
        strncpy(&tile->comment[0], title, (CARD_COMMENT_SIZE / 2) - 1);
    if (description)
        strncpy(&tile->comment[CARD_COMMENT_SIZE / 2], description, (CARD_COMMENT_SIZE / 2) - 1);
    tile->frame_num = frame_num;
    tile->speed = speed;
    tile->is_set = 1;

    stc_tile_icon_file = icon_file; // pixels loaded at save create

    LOG_INFO("Save icon registered from \"%s\" (%d frame(s), speed %d).", icon_file ? icon_file : "?", frame_num, speed);
}

/*---------------------------------------------------------------------*
Name:           KARPlusSave_SetBannerFile

Description:    Public API (Hoshi_SetSaveBannerFile). Registers a 96x32 RGB5A3
                banner for the shared "hoshi" save tile. The banner pixels are
                read from banner_file during save create. Call AFTER
                Hoshi_SetSaveIconFile - that one clears the whole tile.

Arguments:      banner_file - disc filename without ".dat" (and no "_" or ".");
                              a raw CARD_BANNER_SIZE_RGB5A3 RGB5A3 blob.

Returns:        none.

*---------------------------------------------------------------------*/
void KARPlusSave_SetBannerFile(const char *banner_file)
{
#if HOSHI_SAVE_BANNER
    if (!stc_hoshi_save || !banner_file)
        return;

    KARPlusSaveTile *tile = &stc_hoshi_save->tile;

    tile->has_banner = 1;
    tile->is_set = 1;

    stc_tile_banner_file = banner_file; // pixels loaded at save create

    LOG_INFO("Save banner registered from \"%s\".", banner_file);
#else
    LOG_WARN("Save banner ignored (HOSHI_SAVE_BANNER disabled).");
#endif
}

/*---------------------------------------------------------------------*
Name:           KARPlusSave_FileExists

Description:    Reports whether a disc file exists WITHOUT panicking. File_GetSize
                and File_LoadSync assert-fail on a missing file, so their result
                can't be used as a presence check. The DVD loader mangles a name
                with no "_"/"." into "<name>.dat", so build that path here and
                resolve it directly - DVDConvertPathToEntrynum returns -1 (no
                assert) when the file is absent.

Arguments:      file - disc filename without ".dat" (and with no "_" or ".")

Returns:        1 if the file is on the disc, 0 otherwise.

*---------------------------------------------------------------------*/
static int KARPlusSave_FileExists(const char *file)
{
    char path[36];
    int n = strlen(file);

    if (n <= 0 || n + 4 >= (int)sizeof(path))
        return 0;

    memcpy(path, file, n);
    memcpy(path + n, ".dat", 5); // includes the NUL terminator

    return DVDConvertPathToEntrynum(path) >= 0;
}

/*---------------------------------------------------------------------*
Name:           KARPlusSave_LoadImageInto

Description:    Reads a raw RGB5A3 blob off the disc into a tile image slot.
                DVD reads need a 32-byte aligned destination and the tile
                fields are not aligned, so the read goes through a temporary
                aligned buffer (freeable - this runs after boot) and is then
                copied into the tile. A missing/wrong-size file is skipped.

Arguments:      file          - disc filename without ".dat" (no "_" or ".")
                dst           - tile image slot to fill
                expected_size - exact byte size the file must be

Returns:        none.

*---------------------------------------------------------------------*/
static void KARPlusSave_LoadImageInto(const char *file, void *dst, int expected_size)
{
    // File_GetSize/File_LoadSync panic on a missing file, so bail out gracefully first
    // if the blob was not shipped on the disc rather than hard-crashing at save create.
    if (!KARPlusSave_FileExists(file))
    {
        LOG_WARN("Tile art \"%s\" not found on disc - skipping.", file);
        return;
    }

    int size = File_GetSize((char *)file);
    if (size != expected_size)
    {
        LOG_WARN("Tile art \"%s\" is %d bytes, expected %d - skipping.", file, size, expected_size);
        return;
    }

    void *buf = HSD_MemAlloc(OSRoundUp32B(size));
    if (!buf)
    {
        LOG_WARN("Tile art \"%s\" alloc failed.", file);
        return;
    }

    int got = 0;
    File_LoadSync((char *)file, buf, &got);
    if (got == expected_size)
        memcpy(dst, buf, expected_size);
    else
        LOG_WARN("Tile art \"%s\" read %d bytes, expected %d.", file, got, expected_size);

    HSD_Free(buf);
}

/*---------------------------------------------------------------------*
Name:           KARPlusSave_LoadTileArt

Description:    Pulls the registered icon/banner blobs off the disc into the
                live tile, just before the save struct is written to the card.

Arguments:      none.

Returns:        none.

*---------------------------------------------------------------------*/
static void KARPlusSave_LoadTileArt()
{
    KARPlusSaveTile *tile = &stc_hoshi_save->tile;

    if (stc_tile_icon_file)
        KARPlusSave_LoadImageInto(stc_tile_icon_file, tile->icon, tile->frame_num * CARD_ICON_SIZE_RGB5A3);
#if HOSHI_SAVE_BANNER
    if (stc_tile_banner_file && tile->has_banner)
        KARPlusSave_LoadImageInto(stc_tile_banner_file, tile->banner, CARD_BANNER_SIZE_RGB5A3);
#endif
}

/*---------------------------------------------------------------------*
Name:           KARPlusSave_ApplyTile

Description:    Points the open file's CARDStat directory entry at the tile
                bytes embedded in the save (comment + RGB5A3 icon frames).
                The icon pixels themselves are persisted as part of the save
                struct via CARDWrite.

Arguments:      fileInfo - an open CARDFileInfo for the "hoshi" file.

Returns:        cardResult.

*---------------------------------------------------------------------*/
static int KARPlusSave_ApplyTile(CARDFileInfo *fileInfo)
{
    CARDStat stat;
    KARPlusSaveTile *tile = &stc_hoshi_save->tile;

    s32 cardResult = CARDGetStatus(0, fileInfo->fileNo, &stat);
    if (cardResult != CARD_RESULT_READY)
    {
        LOG_WARN("Failed to get status for save icon (%d).", cardResult);
        return cardResult;
    }

    stat.commentAddr = (u32)((int)&tile->comment - (int)stc_hoshi_save);

    // iconAddr points at the start of the contiguous image block and must stay under 512 bytes
    // (CARDSetStatus rejects an iconAddr >= 512). With a banner reserved the block starts at the
    // banner, and the CARD library derives the icon's offset as iconAddr + banner size.
#if HOSHI_SAVE_BANNER
    stat.iconAddr = (u32)((int)&tile->banner - (int)stc_hoshi_save);
    stat.bannerFormat = tile->has_banner ? CARD_STAT_BANNER_RGB5A3 : CARD_STAT_BANNER_NONE;
#else
    stat.iconAddr = (u32)((int)&tile->icon - (int)stc_hoshi_save);
    stat.bannerFormat = CARD_STAT_BANNER_NONE;
#endif

    // 2 bits/frame: RGB5A3 format + speed for each valid frame, 0 (NONE) past the end stops the loop
    u16 icon_format = 0, icon_speed = 0;
    for (int i = 0; i < tile->frame_num && i < CARD_ICON_MAX; i++)
    {
        icon_format |= (u16)(CARD_STAT_ICON_RGB5A3 << (i * 2));
        icon_speed |= (u16)((tile->speed & CARD_STAT_SPEED_MASK) << (i * 2));
    }
    stat.iconFormat = icon_format;
    stat.iconSpeed = icon_speed;

    cardResult = CARDSetStatus(0, fileInfo->fileNo, &stat);
    if (cardResult != CARD_RESULT_READY)
        LOG_WARN("Failed to set save icon status (%d).", cardResult);

    return cardResult;
}

/*---------------------------------------------------------------------*
Name:           KARPlusSave_OnSetDefault

Description:    Inserted after initializing default save file settings.

Arguments:      none.

Returns:        none.

*---------------------------------------------------------------------*/
void KARPlusSave_OnSetDefault()
{
    LOG_INFO("Initializing default save values...");
    Mods_SetDefaultSaveData();
    LOG_INFO("Done.");
}
CODEPATCH_HOOKCREATE(0x80007630, "", KARPlusSave_OnSetDefault, "", 0)

/*---------------------------------------------------------------------*
Name:           KARPlusSave_OnSaveCreateOrLoad

Description:    Inserted at the end of vanilla memcard initialization.
                Calls KARPlusSave_CreateOrLoad.

Arguments:      none.

Returns:        none.

*---------------------------------------------------------------------*/
void KARPlusSave_OnSaveCreateOrLoad()
{

    LOG_INFO("---------------------");
    LOG_INFO("[hoshi] Initializing save...");

    // check if main save was created
    if (Memcard_GetState() == MEMCARD_SUCCESS && Memcard_GetSaveStatus() == CARDSAVE_EXIST)
    {
        // create/load custom save file
        int is_created = KARPlusSave_CreateOrLoad(); // create/load the save file

        // update hash
        stc_hoshi_save_hash = _hash_32(stc_hoshi_save, SAVE_SIZE);

        // enter a new scene and ask the user if they just created a custom save
        if (is_created)
        {
            ;
        }
    }

    // run on load callbacks
    Mods_OnLoadSaveData();

    LOG_INFO("Finished initializing save.");
    LOG_INFO("---------------------\n");

    return;
}
CODEPATCH_HOOKCREATE(0x80047834, "", KARPlusSave_OnSaveCreateOrLoad, "", 0)
CODEPATCH_HOOKCREATE(0x80047720, "", KARPlusSave_OnSaveCreateOrLoad, "", 0)

/*---------------------------------------------------------------------*
Name:           KARPlusSave_OnMainMenuEnter

Description:    Inserted at the end of vanilla main menu initialization.
                Determines whether or not to write out data to memcard.

Arguments:      none.

Returns:        none.

*---------------------------------------------------------------------*/
void KARPlusSave_OnMainMenuEnter()
{
    KARPlusSave_Write();
}
CODEPATCH_HOOKCREATE(0x800189a4, "", KARPlusSave_OnMainMenuEnter, "", 0)

/*---------------------------------------------------------------------*
Name:           KARPlusSave_OnNoCardInserted

Description:    Inserted at the end of vanilla main menu initialization.
                Determines whether or not to write out data to memcard.

Arguments:      none.

Returns:        none.

*---------------------------------------------------------------------*/
void KARPlusSave_OnNoSave()
{
    // check if no card inserted
    if (Memcard_GetState() == MEMCARD_ERROR)
    {
        // set as no save to skip prompt
        Memcard_SetSaveStatus(CARDSAVE_IGNORE);

        // boot to main menu
        Scene_SetNextMajor(MJRKIND_MENU);

        // run on load callbacks
        Mods_OnLoadSaveData();

        // skip all tutorial vids
        GameData *gd = Gm_GetGameData();
        gd->is_showed_tutorial_airride = 1;
        gd->is_showed_tutorial_topride = 1;
        gd->is_showed_tutorial_city = 1;

        /*
        025367BC 003BFFFF
        025369FC 003BFFFF
        025368d4 003BFFFF
        04536EE8 FFFFFFFF
        04536EEC 00000000
        */

        // unlock everything
        int *unlock_bitfield = (int *)0x80536EE8;
        unlock_bitfield[0] = -1;
        unlock_bitfield[1] = 0;

        // completed checklist
        int *airride_checklist = (int *)0x805367BC;
        memset(airride_checklist, -1, 0x78);
        int *topride_checklist = (int *)0x805369FC;
        memset(topride_checklist, -1, 0x78);
        int *city_checklist = (int *)0x805368d4;
        memset(city_checklist, -1, 0x78);

        MainMenu_InitAllVariables();
        Scene_ExitMajor();
    }

    // otherwise card is inserted but has no save file, so continue to prompt
}
CODEPATCH_HOOKCREATE(0x80047718, "", KARPlusSave_OnNoSave, "", 0)

/*---------------------------------------------------------------------*
Name:           KARPlusSave_CreateOrLoad

Description:    Handles creating and loading the custom save file (done
                automatically without asking user). Executes on boot up
                when loading the memcard prompt scene.

Arguments:      none.

Returns:        is_created bool.

*---------------------------------------------------------------------*/
int KARPlusSave_CreateOrLoad()
{
    s32 cardResult;
    CARDFileInfo fileInfo;
    CARDStat stat;

    int is_created = 0;

    // Mount the memory card
    cardResult = CARDMount(0, (void *)0x805a7e80, 0);
    if (cardResult != CARD_RESULT_READY)
    {
        LOG_WARN("Failed to mount memory card.");
        return 0;
    }

    // check for existing file
    cardResult = CARDOpen(0, save_name, &fileInfo);

    // no exist
    if (cardResult == CARD_RESULT_NOFILE)
    {
        LOG_DEBUG("No save file found, creating one...");

        // create save file
        cardResult = CARDCreate(0, save_name, SAVE_SIZE, &fileInfo);
        if (cardResult != CARD_RESULT_READY)
        {
            LOG_WARN("Error creating save file.");
            CARDClose(&fileInfo);
            CARDUnmount(0);
            return 0;
        }

        LOG_DEBUG("Save file created with size 0x%x", SAVE_SIZE);

        // pull the tile art off the disc into the tile before it is persisted (an existing file
        // already carries its art, so this only runs on the create path)
        KARPlusSave_LoadTileArt();

        // write to card
        cardResult = CARDWrite(&fileInfo, stc_hoshi_save, SAVE_SIZE, 0);
        if (cardResult != CARD_RESULT_READY)
        {
            LOG_WARN("Error initializing save file (result %d).", cardResult);
            CARDClose(&fileInfo);
            CARDUnmount(0);
            return 0;
        }

        LOG_DEBUG("Created save file on card.");

        is_created = 1;
    }
    // exists
    else if (cardResult == CARD_RESULT_READY)
    {
        LOG_DEBUG("Save file found.");

        // Get file status to retrieve size in blocks
        cardResult = CARDGetStatus(0, fileInfo.fileNo, &stat);
        if (cardResult != CARD_RESULT_READY)
        {
            LOG_WARN("Failed to get file status.");
            CARDClose(&fileInfo);
            CARDUnmount(0);
            return 0;
        }

        // ensure this save file is the size we expect it to be
        if (stat.length != SAVE_SIZE)
        {
            LOG_WARN("Error, expected file to be size %d, is %d", SAVE_SIZE, stat.length);
            CARDClose(&fileInfo);
            CARDUnmount(0);
            return 0;
        }

        LOG_DEBUG("Loading it");

        cardResult = CARDRead(&fileInfo, stc_hoshi_save, OSRoundUp512B(SAVE_SIZE), 0);
        if (cardResult != CARD_RESULT_READY)
        {
            LOG_WARN("Failed to read save data.");
            CARDClose(&fileInfo);
            CARDUnmount(0);
            return 0;
        }
    }

    // point the card directory entry at the embedded tile so the icon shows in the card manager
    if (cardResult == CARD_RESULT_READY && stc_hoshi_save->tile.is_set)
        KARPlusSave_ApplyTile(&fileInfo);

    CARDClose(&fileInfo);
    CARDUnmount(0);

    return is_created;
}

/*---------------------------------------------------------------------*
Name:           KARPlusSave_Write

Description:    Writes the save data to the memory card syncronously.

Arguments:      none.

Returns:        cardResult.

*---------------------------------------------------------------------*/
int KARPlusSave_Write()
{

    // check if save contents were modified
    int cur_hash = _hash_32(stc_hoshi_save, SAVE_SIZE);
    if (cur_hash != stc_hoshi_save_hash)
    {
        stc_hoshi_save_hash = cur_hash;

        // to-do: put this on a thread and make operations async
        if (Memcard_GetSaveStatus() == CARDSAVE_NOEXIST)
            return CARD_RESULT_NOCARD;

        s32 cardResult;
        CARDFileInfo fileInfo;
        CARDStat stat;

        // Mount the memory card
        cardResult = CARDMount(0, (void *)0x805a7e80, 0);
        if (cardResult != CARD_RESULT_READY)
        {
            LOG_DEBUG("Failed to mount memory card.");
            return cardResult;
        }

        // check for existing file
        cardResult = CARDOpen(0, save_name, &fileInfo);

        if (cardResult != CARD_RESULT_READY)
        {
            LOG_DEBUG("Failed to open save file.");
            CARDClose(&fileInfo);
            CARDUnmount(0);
            return cardResult;
        }

        // write save data
        cardResult = CARDWrite(&fileInfo, stc_hoshi_save, SAVE_SIZE, 0);
        if (cardResult != CARD_RESULT_READY)
        {
            LOG_WARN("Error updating save file.");
            CARDClose(&fileInfo);
            CARDUnmount(0);
            return cardResult;
        }

        CARDClose(&fileInfo);
        CARDUnmount(0);
        return cardResult;
    }

    return CARD_RESULT_READY;
}

/*---------------------------------------------------------------------*
Name:           KARPlusSave_Alloc

Description:    Returns a ptr to save data

Arguments:      hash - unique identifier for this mod
                size - size of the alloc

Returns:        ptr to mod's user_data.

*---------------------------------------------------------------------*/
void *KARPlusSave_Alloc(GlobalMod *mod, int menu_size, int user_size)
{
    int mod_hash = _hash_32_str(mod->desc->name);

    if (stc_hoshi_save->mod_num > GetElementsIn(stc_hoshi_save->metadata))
    {
        // to-do, free something and ensure it gets alloc'd
        LOG_ERROR("Save: mod_num over %d", GetElementsIn(stc_hoshi_save->metadata));
        return 0;
    }

    // copy mod hash
    stc_hoshi_save->metadata[stc_hoshi_save->mod_num].mod_hash = mod_hash;

    // Reject the alloc if it would not fit in the data region. Without this an
    // over-budget mod silently writes past SAVE_SIZE into whatever the persistent
    // allocator placed next (the mod-loader registry), zeroing it on save load.
    if (menu_size + user_size > KARPlusSave_CheckFreeData())
    {
        LOG_ERROR("Save: no room for mod %s (need 0x%x, free 0x%x)",
                  mod->desc->name, menu_size + user_size, KARPlusSave_CheckFreeData());
        return 0;
    }

    // Data offsets are relative to data[] (mod 0 starts at 0). All readers index
    // &stc_hoshi_save->data[offset], which already adds offsetof(data) - so the
    // offset must NOT include it again.
    int menu_offset;
    if (stc_hoshi_save->mod_num == 0)
        menu_offset = 0;
    else
    {
        menu_offset = stc_hoshi_save->metadata[stc_hoshi_save->mod_num - 1].user_data.offset +
                      stc_hoshi_save->metadata[stc_hoshi_save->mod_num - 1].user_data.size;
    }
    int user_offset = menu_offset + menu_size;

    stc_hoshi_save->metadata[stc_hoshi_save->mod_num].is_present = 1; // set mod as present

    // init menu
    stc_hoshi_save->metadata[stc_hoshi_save->mod_num].menu_data.offset = menu_offset;
    stc_hoshi_save->metadata[stc_hoshi_save->mod_num].menu_data.size = menu_size;
    if (menu_size > 0)
        memset(&stc_hoshi_save->data[menu_offset], -1, menu_size); // fill with -1's to indicate unused space

    // init user
    stc_hoshi_save->metadata[stc_hoshi_save->mod_num].user_data.offset = user_offset;
    stc_hoshi_save->metadata[stc_hoshi_save->mod_num].user_data.size = user_size;

    // get mem ptr (data[]-relative, matching mod->save.user_data below)
    void *save_data_ptr = 0;
    if (user_size > 0)
        save_data_ptr = (void *)&stc_hoshi_save->data[user_offset];

    LOG_DEBUG("alloc'd mod %s index %d hash 0x%x metadata @ %p",
              mod->desc->name,
              stc_hoshi_save->mod_num,
              stc_hoshi_save->metadata[stc_hoshi_save->mod_num].mod_hash,
              &stc_hoshi_save->metadata[stc_hoshi_save->mod_num]);

    stc_hoshi_save->mod_num++;

    // save pointers to mod data
    mod->save.menu_data = (MenuSave *)(&((u8 *)&stc_hoshi_save->data)[menu_offset]);
    mod->save.menu_num = menu_size / sizeof(MenuSave);
    mod->save.user_data = (MenuSave *)(&((u8 *)&stc_hoshi_save->data)[user_offset]);
    mod->save.user_size = user_size;

    return save_data_ptr;
}

/*---------------------------------------------------------------------*
Name:           KARPlusSave_VerifySize

Description:    Returns a ptr to save data

Arguments:      hash - unique identifier for this mod
                size - size of the alloc

Returns:        bool - if the save size changed.

*---------------------------------------------------------------------*/
int KARPlusSave_VerifySize(GlobalMod *mod, int menu_size, int user_size)
{
    int is_size_changed = 0;
    int mod_hash = _hash_32_str(mod->desc->name);
    int new_size = menu_size + user_size;

    // find this save
    for (int i = 0; i < stc_hoshi_save->mod_num; i++)
    {
        if (mod_hash == stc_hoshi_save->metadata[i].mod_hash)
        {
            int saved_size = stc_hoshi_save->metadata[i].menu_data.size + stc_hoshi_save->metadata[i].user_data.size;

            if (new_size > saved_size)
            {
                LOG_WARN("Current version save is larger (0x%08x / 0x%08x).",
                         new_size,
                         saved_size);

                int free_size = KARPlusSave_CheckFreeData();
                int shift_amt = new_size - saved_size;

                // resize
                if (free_size >= new_size)
                {
                    LOG_INFO("Resizing save...");

                    // shift other data
                    for (int j = stc_hoshi_save->mod_num - 1; j > i; j--)
                    {
                        // shift data
                        int data_size_old = stc_hoshi_save->metadata[j].menu_data.size + stc_hoshi_save->metadata[j].user_data.size;
                        u8 *data_ptr_old = &stc_hoshi_save->data[stc_hoshi_save->metadata[j].menu_data.offset];
                        memmove(data_ptr_old + shift_amt, data_ptr_old, data_size_old);

                        // update metadata
                        stc_hoshi_save->metadata[j].menu_data.offset += shift_amt;
                        stc_hoshi_save->metadata[j].user_data.offset += shift_amt;
                    }

                    // fill new menu data with -1
                    int old_menusave_size = stc_hoshi_save->metadata[i].menu_data.size;
                    if (menu_size > old_menusave_size)
                    {
                        LOG_DEBUG("Nulling additional menu saves.");
                        memset(&stc_hoshi_save->data[stc_hoshi_save->metadata[i].menu_data.offset + old_menusave_size], -1, menu_size - old_menusave_size); // fill with -1's to indicate unused space
                    }

                    // update metadata
                    stc_hoshi_save->metadata[i].menu_data.size = menu_size;
                    stc_hoshi_save->metadata[i].user_data.size = user_size;

                    // re-init data?
                    is_size_changed = 1;
                }
                // unable to resize
                else
                {
                    LOG_WARN("Not enough free space to resize 0x%08x -> 0x%08x.", new_size, free_size);
                }
            }
            else if (new_size < saved_size)
            {
                LOG_WARN("Current version save is smaller than expected. (0x%08x / 0x%08x).",
                         new_size,
                         saved_size);
                LOG_WARN("Resizing save...");

                int shift_amt = new_size - saved_size;

                // shift other data
                for (int j = i + 1; j > stc_hoshi_save->mod_num; j++)
                {
                    // shift data
                    int data_size_old = stc_hoshi_save->metadata[j].menu_data.size + stc_hoshi_save->metadata[j].user_data.size;
                    u8 *data_ptr_old = &stc_hoshi_save->data[stc_hoshi_save->metadata[j].menu_data.offset];
                    memmove(data_ptr_old + shift_amt, data_ptr_old, data_size_old);

                    // update metadata
                    stc_hoshi_save->metadata[j].menu_data.offset += shift_amt;
                    stc_hoshi_save->metadata[j].user_data.offset += shift_amt;
                }

                // fill all menu data with -1 if menu size changed
                int old_menusave_size = stc_hoshi_save->metadata[i].menu_data.size;
                if (menu_size < old_menusave_size)
                {
                    LOG_DEBUG("Nulling all menu saves.");
                    memset(&stc_hoshi_save->data[stc_hoshi_save->metadata[i].menu_data.offset], -1, menu_size); // fill with -1's to indicate unused space
                }

                // update metadata
                stc_hoshi_save->metadata[i].menu_data.size = menu_size;
                stc_hoshi_save->metadata[i].user_data.size = user_size;

                // re-init data?
                is_size_changed = 1;
            }

            int menu_data_offset = stc_hoshi_save->metadata[i].menu_data.offset;
            int user_data_offset = stc_hoshi_save->metadata[i].user_data.offset;

            // save pointers
            mod->save.menu_data = (MenuSave *)(&((u8 *)&stc_hoshi_save->data)[menu_data_offset]);
            mod->save.menu_num = menu_size / sizeof(MenuSave);
            mod->save.user_data = (MenuSave *)(&((u8 *)&stc_hoshi_save->data)[user_data_offset]);
            mod->save.user_size = user_size;

            LOG_DEBUG("verified mod %s index %d hash 0x%x metadata @ %p",
                      mod->desc->name,
                      i,
                      stc_hoshi_save->metadata[i].mod_hash,
                      &stc_hoshi_save->metadata[i]);

            break;
        }
    }

    return is_size_changed;
}

/*---------------------------------------------------------------------*
Name:           KARPlusSave_CheckModDataExists

Description:    Returns a bool if a mod's save data exists

Arguments:      mod_name - unique mod name string.

Returns:        bool, isExists

*---------------------------------------------------------------------*/
int KARPlusSave_CheckModDataExists(GlobalMod *mod)
{
    int mod_hash = _hash_32(mod->desc->name, strlen(mod->desc->name));

    for (int i = 0; i < stc_hoshi_save->mod_num; i++)
    {
        if (mod_hash == stc_hoshi_save->metadata[i].mod_hash)
            return 1;
    }

    return 0;
}

/*---------------------------------------------------------------------*
Name:           KARPlusSave_CheckFreeData

Description:    Returns the amount of free memory in the data section

Arguments:      none.

Returns:        Amount of free memory in the data section.

*---------------------------------------------------------------------*/
int KARPlusSave_CheckFreeData()
{
    int size_used = 0;

    for (int i = 0; i < stc_hoshi_save->mod_num; i++)
        size_used += stc_hoshi_save->metadata[i].user_data.size + stc_hoshi_save->metadata[i].menu_data.size;

    return SAVE_SIZE - offsetof(KARPlusSave, data) - size_used;
}

/*---------------------------------------------------------------------*
Name:           KARPlusSave_Get

Description:    Returns a ptr to the city settings save data.

Arguments:      none.

Returns:        KARPlusSave ptr.

*---------------------------------------------------------------------*/
KARPlusSave *KARPlusSave_Get()
{
    return stc_hoshi_save;
}

/*---------------------------------------------------------------------*
Name:           KARPlusSave_Init

Description:    Allocates save file runtime struct. Places branches
                to save related functions in game memory.

Arguments:      none.

Returns:        none.

*---------------------------------------------------------------------*/
void KARPlusSave_Init()
{

    stc_hoshi_save = HSD_MemAlloc(SAVE_SIZE); // alloc save data
    stc_hoshi_save_hash = -1;

    // init save data
    stc_hoshi_save->version_major = VERSION_MAJOR;
    stc_hoshi_save->version_minor = VERSION_MINOR;
    stc_hoshi_save->mod_num = 0;
    stc_hoshi_save->pad = 0;
    stc_hoshi_save->tile.is_set = 0; // no card tile unless a mod calls Hoshi_SetSaveIcon

    // install functions
    CODEPATCH_HOOKAPPLY(0x80047834);
    CODEPATCH_HOOKAPPLY(0x80047720);
    CODEPATCH_HOOKAPPLY(0x80047718);

    CODEPATCH_HOOKAPPLY(0x800189a4);
    CODEPATCH_REPLACEFUNC(Hoshi_WriteSave, KARPlusSave_Write);
    CODEPATCH_REPLACEFUNC(Hoshi_SetSaveIconFile, KARPlusSave_SetIconFile);
    CODEPATCH_REPLACEFUNC(Hoshi_SetSaveBannerFile, KARPlusSave_SetBannerFile);
    CODEPATCH_HOOKAPPLY(0x80007630);

    CODEPATCH_REPLACEFUNC(Hoshi_GetBackupSize, _Hoshi_GetBackupSize);
    CODEPATCH_REPLACEFUNC(Hoshi_BackupModSave, _Hoshi_BackupModSave);
    CODEPATCH_REPLACEFUNC(Hoshi_RestoreModSave, _Hoshi_RestoreModSave);

    return;
}

/*---------------------------------------------------------------------*
Name:           _Hoshi_GetBackupSize

Description:    Determines the size of the ModSaveBackup array which
                is comprised of the save data of mods that affect gameplay.

Arguments:      None.

Returns:        Size.

*---------------------------------------------------------------------*/
int _Hoshi_GetBackupSize()
{
    int size = 0;

    for (int i = 0; i < stc_modloader_data->mod_num; i++)
    {
        GlobalMod *mod = &stc_modloader_data->mods[i];

        if (mod->desc->affects_gameplay)
            size += sizeof(ModSaveBackup) + (mod->save.menu_num * sizeof(MenuSave)) + mod->save.user_size;
    }
    
    return size;
}
int _Hoshi_BackupModSave(u8 *save)
{
    int cur_offset = 0;
    int num = 0;

    for (int i = 0; i < stc_modloader_data->mod_num; i++)
    {
        GlobalMod *mod = &stc_modloader_data->mods[i];

        if (mod->desc->affects_gameplay)
        {
            ModSaveBackup *mod_backup = (ModSaveBackup *)&save[cur_offset];
            mod_backup->name[sizeof(mod_backup->name) - 1] = '\0';

            int save_size = (mod->save.menu_num * sizeof(MenuSave))+ mod->save.user_size;

            strncpy(mod_backup->name, mod->desc->name, sizeof(mod_backup->name));
            mod_backup->version.major = mod->desc->version.major;
            mod_backup->version.minor = mod->desc->version.minor;
            mod_backup->save.menu_num = mod->save.menu_num;
            mod_backup->save.user_size = mod->save.user_size;
            memcpy(mod_backup->save.data, mod->save.menu_data, save_size);
            
            cur_offset += sizeof(ModSaveBackup) + save_size;
            num++;
        }
    }
    
    return num;
}
void _Hoshi_RestoreModSave(u8 *save, int num)
{
    int cur_offset = 0;

    // each mod save
    for (int i = 0; i < num; i++)
    {
        ModSaveBackup *mod_backup = (ModSaveBackup *)&save[cur_offset];
        GlobalMod *mod = Mods_GetFromName(mod_backup->name);
        
        int save_size = mod_backup->save.menu_num * sizeof(MenuSave) + mod_backup->save.user_size;

        cur_offset += sizeof(ModSaveBackup) + save_size;
        
        if (!mod)
            continue; // warn about this?
        
        // each menu option in the mod save
        MenuSave *menu_save = (MenuSave *)mod_backup->save.data;
        for (int j = 0; j < mod_backup->save.menu_num; j++)
        {
            LOG_DEBUG("looking for hash %04x from backup", menu_save[j].hash);

            // each menu option in the installed mod
            for (int k = 0; k < mod->save.menu_num; k++)
            {
                if (mod->save.menu_data[k].hash == menu_save[j].hash)
                {
                    LOG_DEBUG("copying hash %04x to save with val %d", menu_save[j].hash, menu_save[j].val);
                    mod->save.menu_data[k].val = menu_save[j].val;
                    break;
                }
            }
        }

        if (mod_backup->version.major <= mod->desc->version.major)
            memcpy(mod->save.user_data, &mod_backup->save.data[mod_backup->save.menu_num * sizeof(MenuSave)], mod_backup->save.user_size);
            
    }

    Mod_CopyAllFromSave();
}