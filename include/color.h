#ifndef MEX_H_COLOR
#define MEX_H_COLOR

#include "gx.h"
#include "structs.h"
#include "datatypes.h"

/*** Structs ***/

struct ColAnimDesc // in PlCo
{
    u8 *cmd_data; // 0x0
    u8 priority;  // 0x4
    u8 x5;        // 0x5
};

// One color-overlay slot. ColAnim_Apply (0x8006a3f0) latches a ColAnimDesc here and the
// per-frame evaluation of cmd_data drives color and light; NULLing cmd_data freezes the
// slot on whatever it last wrote.
struct ColAnimSlot
{
    int     x00;            // 0x00, zeroed by ColAnim_Apply
    int     param;          // 0x04, the param ColAnim_Apply was called with
    u8     *cmd_data;       // 0x08, ColAnimDesc.cmd_data of the running anim
    int     x0c;            // 0x0c, zeroed by ColAnim_Apply
    int     x10[6];         // 0x10
    int     anim_index;     // 0x28, running ColAnimDesc index; 0 = slot inactive
    GXColor color;          // 0x2c, body tint the resolver copies out
    float   color_f[4];     // 0x30, the same tint as 0..255 floats
    int     x40[4];         // 0x40
    float   color_blend[4]; // 0x50, per-channel weight; 1.0 unless ColAnim_ApplyEx (0x8006a2d8) set it
    GXColor light_color;    // 0x60
    float   light_f[4];     // 0x64
    int     x74[4];         // 0x74
    float   light_blend[4]; // 0x84, as color_blend
    Vec3    light_pos;      // 0x94
    int     xa0[2];         // 0xa0
    u8      ratio;          // 0xa8, overlay blend ratio; 0xff = full strength
    u8      pri;            // 0xa9, ColAnim_Apply rejects a ColAnimDesc of lower priority
    u8      flags;          // 0xaa, 0x80 = tint live, 0x40 = light live, 0x20/0x10 select light submodes
    u8      xab;            // 0xab
};
_Static_assert(sizeof(ColAnimSlot) == 0xac, "ColAnimSlot must be 0xac bytes");

// Three overlay slots plus the state the renderer reads. ColAnim_Resolve (0x8006ae7c) picks
// the active slot of highest priority and copies its tint, light and ratio down here;
// ColAnim_SetupTev (0x8006aaa4) then uploads `color` to a free TEV color register, and does
// nothing at all when flags bit 0x80 is clear and ratio is 0xff.
struct ColAnimState
{
    ColAnimSlot slot[3];     // 0x000, 0x0ac, 0x158
    LOBJ       *lobj;        // 0x204, the overlay's own light, built by LObj_CreateAll
    LOBJ       *lobj_prev;   // 0x208, the light set restored while no slot drives the light
    void       *x20c;        // 0x20c
    GXColor     light_color; // 0x210
    float       light_f[3];  // 0x214
    float       light_scale; // 0x220, divides light_color before HSD_LObjSetColor
    GXColor     color;       // 0x224, the resolved tint
    Vec3        light_pos;   // 0x228
    u8          ratio;       // 0x234, 0xff = no ratio blend
    u8          flags;       // 0x235, 0x80 = tint live, 0x40 = light live, 0x08 = light_pos is model-local
    u8          x236[2];     // 0x236
};
_Static_assert(sizeof(ColAnimState) == 0x238, "ColAnimState must be 0x238 bytes");

#define COLANIM_FLAG_TINT  0x80 // tint override is live
#define COLANIM_FLAG_LIGHT 0x40 // light override is live
#define COLANIM_PRI_MAX    0xff
#define COLANIM_RATIO_NONE 0xff // ColAnimState.ratio value meaning "no ratio blend"

// Requests `index` from `table` into one slot. Refuses, returning 0, when the slot already
// holds an anim of higher priority.
int  ColAnim_Apply(ColAnimSlot *slot, ColAnimDesc *table, int index, int param); // 0x8006a3f0
// Clears the slot: no anim, priority 0, tint and light overrides off.
void ColAnim_Reset(ColAnimSlot *slot);                                           // 0x8006a250

#endif
