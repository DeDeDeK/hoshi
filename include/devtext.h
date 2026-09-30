#ifndef MEX_H_DEVTEXT
#define MEX_H_DEVTEXT

#include "structs.h"
#include "gx.h"

/*** Structs ***/

struct DevText
{
    u16 x;                    // 0x0
    u16 y;                    // 0x2
    u8 width;                 // 0x4
    u8 height;                // 0x5
    u8 cursor_x;              // 0x6
    u8 cursor_y;              // 0x7
    Vec2 scale;               // 0x8
    GXColor bg_color;         // 0x10
    int x14;                  // 0x14
    int x18;                  // 0x18
    int x1c;                  // 0x1c
    int x20;                  // 0x20
    s8 priority;              // 0x24, the DevText list is kept sorted by it
    char x25;                 // 0x25
    char hide_text : 1;       // 0x26
    char hide_background : 1; // 0x26
    char hide_cursor : 1;     // 0x26
    char x27;                 // 0x27
    u8 *text_data;            // 0x28
    DevText *prev;            // 0x2c
    DevText *next;            // 0x30
    void *buf;                // 0x34, DevelopText_Create's buf; text_data is allocated only when it is NULL
    int x38;                  // 0x38
    int x3c;                  // 0x3c
    int x40;                  // 0x40
    int x44;                  // 0x44
    int x48;                  // 0x48
    int x4c;                  // 0x4c
    int x50;                  // 0x50
    int x54;                  // 0x54
    int x58;                  // 0x58
    int x5c;                  // 0x5c
};

/*** Functions ***/

DevText *DevelopText_Create(int priority, int x, int y, int width, int height, void *buf); // 0x800ab2d4
void DevelopText_AddString(DevText *text, ...); // 0x800ab78c
void DevelopText_EraseAllText(DevText *text); // 0x800ab5a4
void DevelopText_SetCursorXY(DevText *text, int x, int y); // 0x800ab4c0
void DevelopText_StoreTextColor(DevText *text, u8 *RGBA); // 0x800ab55c
void DevelopText_StoreBGColor(DevText *text, u8 *RGBA); // 0x800ab584
void DevelopText_StoreTextScale(DevText *text, float x, float y); // 0x800ab534

#endif