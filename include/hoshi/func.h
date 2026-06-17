#ifndef HOSHIFUNC_H
#define HOSHIFUNC_H

#include "preload.h"
#include "scene.h"

MajorSceneDesc *Hoshi_GetMajorScenes();
u8 Hoshi_InstallMajorScene(MajorSceneDesc *);
MinorSceneDesc *Hoshi_GetMinorScenes();
u8 Hoshi_InstallMinorScene(MinorSceneDesc *);

int Hoshi_WriteSave();

// Give the shared "hoshi" save file a card-manager comment + animated icon.
//   title/description : card text, up to 31 chars each
//   frames_rgb5a3     : frame_num packed 32x32 RGB5A3 frames (each 32*32*2 bytes)
//   frame_num         : 1..HOSHI_SAVE_ICON_FRAMES (1 by default; raise the define to animate)
//   speed             : CARD_STAT_SPEED_FAST/MIDDLE/SLOW, applied to every frame
// Call once at mod boot; the tile is committed to the card on the next save create/load.
void Hoshi_SetSaveIcon(const char *title, const char *description, const void *frames_rgb5a3, int frame_num, int speed);

int Hoshi_AddPreloadMenuFile(char *file_name);
int Hoshi_AddPreloadGameFile(char *file_name, PreloadHeapKind heap_kind);
#endif