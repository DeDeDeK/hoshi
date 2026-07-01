#ifndef HOSHIFUNC_H
#define HOSHIFUNC_H

#include "preload.h"
#include "scene.h"

MajorSceneDesc *Hoshi_GetMajorScenes();
u8 Hoshi_InstallMajorScene(MajorSceneDesc *);
MinorSceneDesc *Hoshi_GetMinorScenes();
u8 Hoshi_InstallMinorScene(MinorSceneDesc *);

int Hoshi_WriteSave();

// Give the shared "hoshi" save file a card-manager comment + animated icon, loaded from a disc
// file (so the image is not baked into the mod's code/MEM1 footprint).
//   title/description : card text, up to 31 chars each
//   icon_file         : disc filename without ".dat"; a raw frame_num*32*32 RGB5A3 blob
//   frame_num         : 1..HOSHI_SAVE_ICON_FRAMES (1 by default; raise the define to animate)
//   speed             : CARD_STAT_SPEED_FAST/MIDDLE/SLOW, applied to every frame
// Call once at mod boot; the pixels are read off the disc into the tile when the save is created.
void Hoshi_SetSaveIconFile(const char *title, const char *description, const char *icon_file, int frame_num, int speed);

// Give the shared "hoshi" save file a 96x32 RGB5A3 card-manager banner, loaded from a disc file.
//   banner_file : disc filename without ".dat"; a raw CARD_BANNER_WIDTH x CARD_BANNER_HEIGHT RGB5A3 blob
// Call AFTER Hoshi_SetSaveIconFile (which clears the whole tile). No-op unless hoshi was built with
// HOSHI_SAVE_BANNER enabled (the banner roughly doubles the save file size).
void Hoshi_SetSaveBannerFile(const char *banner_file);

int Hoshi_AddPreloadMenuFile(char *file_name);
int Hoshi_AddPreloadGameFile(char *file_name, PreloadHeapKind heap_kind);
#endif