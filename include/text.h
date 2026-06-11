#ifndef MEX_H_TEXT
#define MEX_H_TEXT

#include "inline.h"
#include "structs.h"
#include "datatypes.h"
#include "obj.h"
#include "gx.h"

/*** Structs ***/

// SIS text-stream opcode IDs. Sizes (incl. opcode byte) are noted per-entry; data
// follows the opcode byte big-endian. The renderer at Text_GXLink (0x804516e4)
// is the source of truth - see docs/sis-text-system.md for full semantics.
typedef enum TextCmdOpcode
{
    TEXTCMD_TERMINATE,        // 0x00, 1 byte. Pops a CALL return marker if present, else ends rendering.
    TEXTCMD_SUBTEXT_RESET,    // 0x01, 1 byte. Resets cursor + temp render state, clears state stack. Falls through to SUBTEXT_BREAK.
    TEXTCMD_SUBTEXT_BREAK,    // 0x02, 1 byte. Bookmarks parse position; resets char counter.
    TEXTCMD_LINEBREAK,        // 0x03, 1 byte. Newline (advance cursor.y by line height * scale).
    TEXTCMD_LINEBREAK_REFLOW, // 0x04, 1 byte. LINEBREAK + sets reflow flag for next-frame re-entry.
    TEXTCMD_DELAY,            // 0x05, 3 bytes (u16 frames). Typewriter pause counter.
    TEXTCMD_TIMING,           // 0x06, 5 bytes (u16 char_delay, u16 space_delay). Sets temp.char_delay then temp.space_delay (operand order verified at Text_GXLink 0x80451e20).
    TEXTCMD_POS,              // 0x07, 5 bytes (s16 x_px, s16 y_lines). Subtext header.
    TEXTCMD_JUMP,             // 0x08, 5 bytes (s32 abs ptr). Absolute pointer jump (HSD-relocated).
    TEXTCMD_CALL,             // 0x09, 5 bytes (s32 abs ptr). Push return marker, jump absolute.
    TEXTCMD_POSPUSH,          // 0x0a, 5 bytes (s16 x, s16 y). Inline relative cursor push.
    TEXTCMD_POSPUSHEND,       // 0x0b, 1 byte. Pops POSPUSH.
    TEXTCMD_COLOR,            // 0x0c, 4 bytes (u8 R, G, B). Pushes color, sets temp.color RGB. Alpha unchanged.
    TEXTCMD_COLOREND,         // 0x0d, 1 byte. Pops COLOR.
    TEXTCMD_SCALE,            // 0x0e, 5 bytes (u16 sx_q8, u16 sy_q8). Pushes scale, sets temp.scale = (sx,sy)/256.
    TEXTCMD_SCALEEND,         // 0x0f, 1 byte. Pops SCALE.
    TEXTCMD_ALIGNCENTER,      // 0x10, 1 byte. Pushes align, sets temp.align = CENTER.
    TEXTCMD_ALIGNCENTEREND,   // 0x11, 1 byte. Pops align (alias of 0x13/0x15).
    TEXTCMD_ALIGNLEFT,        // 0x12, 1 byte. Pushes align, sets temp.align = LEFT.
    TEXTCMD_ALIGNLEFTEND,     // 0x13, 1 byte. Pops align.
    TEXTCMD_ALIGNRIGHT,       // 0x14, 1 byte. Pushes align, sets temp.align = RIGHT.
    TEXTCMD_ALIGNRIGHTEND,    // 0x15, 1 byte. Pops align.
    TEXTCMD_KERNING,          // 0x16, 1 byte. Sets temp.kerning = 1 (no push).
    TEXTCMD_KERNINGEND,       // 0x17, 1 byte. Sets temp.kerning = 0.
    TEXTCMD_FIT,              // 0x18, 1 byte. Sets temp.use_aspect_fit = 1 (auto-shrink to aspect.x).
    TEXTCMD_FITEND,           // 0x19, 1 byte. Sets temp.use_aspect_fit = 0.
    TEXTCMD_SPACE,            // 0x1a, 1 byte. Word separator (advances cursor.x by space-width).
    TEXTCMD_NOOP_1B,          // 0x1b, 1 byte. No-op.
    TEXTCMD_NOOP_1C,          // 0x1c, 1 byte. No-op.
    TEXTCMD_NOOP_1D,          // 0x1d, 1 byte. No-op.
    TEXTCMD_NOOP_1E,          // 0x1e, 1 byte. No-op.
    TEXTCMD_NOOP_1F,          // 0x1f, 1 byte. No-op.
    TEXTCMD_NUM,              // = 0x20; bytes >= 0x20 are 2-byte glyph codes.
} TextCmdOpcode;

typedef enum TextAlignKind
{
    TEXTALIGN_LEFT,
    TEXTALIGN_CENTER,
    TEXTALIGN_RIGHT,
} TextAlignKind;

// Per-SIS-slot glyph bank, used at render time for character codes >= 0x4000.
// Codes < 0x4000 use the master Latin bank baked into main.dol at 0x8050a040
// (image, 256 * 0x200) and 0x80509dc0 (kerning, 256 * 2). Most SIS files leave
// these pointers null and rely entirely on the master bank; only SisSmmenu.dat
// (Japanese kana) and a few icon-carrying files (SisClrChk*, SisSelply*) have
// real per-slot glyph data.
struct SISData
{
    u8 *image_data_arr;   // I4 32x32 glyphs, stride 0x200, indexed by (code - 0x4000) & 0xFF
    u8 *kerning_data_arr; // {u8 left_pad, u8 right_edge} per glyph, stride 0x2
};

struct TextHeapCell
{
    TextHeapCell *next;
    void *alloc;
    int size;
};

struct TextCanvas
{
    TextCanvas *next;   // 0x0, intrusive next in stc_textcanvas_first chain
    GOBJ *cam_gobj;     // 0x4, owning camera GObj (set unless no_create_cam_gobj < 0)
    u16 size;           // 0x8, heap cell size remaining
    u16 sis_idx;        // 0xa, SIS slot bound to this canvas
    u8 entity_class;    // 0xC, GObj entityclass passed to Text_CreateCanvas
    u8 p_link;          // 0xD, GObj plink
    u8 gx_link;         // 0xE, GX link bit for child Text GObjs
    u8 gx_pri;          // 0xF, GX priority for child Text GObjs
};

struct Text
{
    Vec3 trans;             // 0x00, per-vertex anchor in canvas-ortho pixel space. (x: pixels right of canvas left, y: pixels above canvas bottom, z: depth)
    Vec2 aspect;            // 0x0C, bbox width/height in pixels. Used by use_aspect auto-shrink, viewport_color background size, and scissor reference frame.
    float scissor_top;      // 0x14, per-quad clip top    (gated by is_scissor - NOT GXSetScissor)
    float scissor_bot;      // 0x18, per-quad clip bottom (gated by is_scissor)
    float scissor_left;     // 0x1C, per-quad clip left   (gated by is_scissor)
    float scissor_right;    // 0x20, per-quad clip right  (gated by is_scissor)
    Vec2 viewport_scale;    // 0x24, multiplies everything: glyph quad size, x-advance, background, scissor reference. ~0.4 yields HUD-readable size.
    GXColor viewport_color; // 0x2C, RGBA. If alpha != 0, drawn first as a flat-shaded background quad covering aspect * viewport_scale.
    GXColor color;          // 0x30, RGBA. Default per-character color before any TEXTCMD_COLOR. Loaded into temp.color at every subtext start. Alpha is render-state, not affected by COLOR opcodes.
    Vec2 scale;             // 0x34, initial per-character scale; opcode TEXTCMD_SCALE overrides per-glyph.
    float subtext_init_x;   // 0x3C, initial X cursor offset for the subtext (loaded into temp.cursor_x).
    float subtext_init_y;   // 0x40, initial Y cursor offset for the subtext (loaded into temp.cursor_y).
    u16 char_delay_init;    // 0x44, *designed* seed for temp.char_delay: frames to pause after each revealed glyph (typewriter speed; 0 = reveal instantly). GOTCHA: the renderer copies this into temp.char_delay ONLY at a 0x01/0x02 SUBTEXT opcode (the lone write to temp.char_delay is Text_GXLink 0x80451cec). A Text_AddSubtext buffer is 0x07-POS-delimited with NO 0x01/0x02, so the copy never fires and this field is dead for it - seed temp.char_delay directly instead. (reveal_count is likewise reset only on 0x01/0x02, not 0x07, so a multi-subtext buffer reveals sequentially across ALL its segments.)
    u16 space_delay_init;   // 0x46, seeds temp.space_delay: per-0x1a-SPACE / 0x03-LINEBREAK reveal pause (frames). (Formerly "dwell_init".)

    u8 use_aspect;          // 0x48, auto-shrink horizontally to fit aspect.x.
    u8 kerning;             // 0x49, use kerning advances vs. fixed-cell width.
    u8 align;               // 0x4A, TextAlignKind.
    u8 reflow_flag;         // 0x4B, internal - set by TEXTCMD_LINEBREAK_REFLOW to re-enter renderer next frame. Leave at 0 from external code.
    u8 is_depth_compare;    // 0x4C, if 1, GX_LEQUAL Z mode (text z-tests). If 0, always on top.
    u8 hidden;              // 0x4D, non-zero -> Text_GXLink early-returns.
    u8 is_scissor;          // 0x4E, enables the per-quad scissor_* clip rect.
    u8 sis_id;              // 0x4F, index into stc_sis_data[5] for per-SIS image/kerning bank (codes >= 0x4000).

    Text *next;             // 0x50, intrusive next in stc_text_first chain.
    GOBJ *gobj;             // 0x54, owning Text GObj (gobj->userdata at +0x2C points back here).
    void (*render_callback)(GOBJ *text_gobj); // 0x58, invoked once per render in pass 2 after GX state setup, before any drawing. Hook for fade/wave effects. NULL for none. (Advancing text_end here does NOT drive a typewriter - see text_end / char_delay_init.)
    u8 *text_start;         // 0x5C, opcode stream start (NULL gates the entire render).
    u8 *text_end;           // 0x60, typewriter reveal frontier - NOT a hard render limit (the parser halts on the inline 0x00 TERMINATE, not here). The built-in typewriter sets this to parse+1 at each revealed glyph; the loop head (0x80451c44) pauses parsing here while wait_countdown > 0. Reset to 0 at 0x01 SUBTEXT_RESET.
    TextHeapCell *alloc;    // 0x64, primary text-data heap cell.
    TextHeapCell *state_stack; // 0x68, lazily-allocated state-history buffer used by Text_PushState/Text_PopState (color/scale/pos/align frames).
    u16 pospush_flags;      // 0x6C, bitfield gating TEXTCMD_POSPUSH/POSPUSHEND (1=enabled, requires temp.use_aspect != 0).
    u16 char_visited_len;   // 0x6E, length of per-character "visited" tracking buffer (cleared per subtext).

    // Mutable per-subtext render state. The opcodes 0x10/0x12/0x14, 0x16/0x17,
    // 0x18/0x19 mutate the *temp* fields, NOT the public ones at 0x48-0x4A.
    struct
    {
        float cursor_x;       // 0x70, running X cursor for the current subtext (pre-viewport_scale units).
        float cursor_y;       // 0x74, running Y cursor.
        float init_x;         // 0x78, copy of subtext_init_x at subtext start.
        float init_y;         // 0x7C, copy of subtext_init_y at subtext start.
        float scale_x;        // 0x80, current scale.x (init from Text.scale.x; overridden by TEXTCMD_SCALE).
        float scale_y;        // 0x84, current scale.y.
        float fit_squeeze;    // 0x88, width-fit auto-scale factor (= aspect.x / measured_width when fitting; 1.0 otherwise).
        GXColor color;        // 0x8C, TEV reg-1 source per glyph. RGB updated by TEXTCMD_COLOR; alpha sourced from Text.color.a at init.
        u16 char_delay;        // 0x90, per-glyph typewriter pause in frames. Copied into wait_countdown on each glyph/SPACE reveal (0x80452628 / 0x804521f4). Seeded from char_delay_init ONLY at a 0x01/0x02 SUBTEXT opcode (0x80451cec) - for 0x07-delimited Text_AddSubtext buffers nothing seeds it, so write it directly. The renderer reloads it into a working register at render top (0x80451c34) and never clears it, so one write persists across frames. Live-settable mid-stream via TEXTCMD_TIMING (updates only the working register, not this field). (Formerly "char_count_max" - it is a delay, not a draw ceiling.)
        u16 space_delay;       // 0x92, per-space / linebreak pause in frames (init from space_delay_init; live-settable via TEXTCMD_TIMING). (Formerly "dwell".)
        int wait_countdown;    // 0x94, frames left to pause at the reveal frontier; decremented once per render at 0x80451c64, gating the text_end dwell-pause. Set from char_delay/space_delay on each reveal, or directly by TEXTCMD_DELAY.
        int reveal_count;      // 0x98, glyphs revealed so far - engine-written reveal progress. NOT reset by 0x07 POS, only by 0x01/0x02 SUBTEXT or Text creation. The built-in typewriter advances from this; mods can read it for per-glyph effects. (Formerly "char_display_num".)
        u8 use_aspect;         // 0x9C, mirror of Text.use_aspect, mutable via TEXTCMD_FIT/FITEND.
        u8 kerning;            // 0x9D, mirror of Text.kerning, mutable via TEXTCMD_KERNING/KERNINGEND.
        u8 align;              // 0x9E, mirror of Text.align, mutable via TEXTCMD_ALIGN*.
        u8 pad9F;              // 0x9F, padding.
        int pad100;            // 0xA0, padding/scratch.
    } temp;
};

// Text Commands
typedef struct TextCmdPos
{
    u16 x;
    u16 y;
} TextCmdPos;
typedef struct TextCmdColor
{
    u8 r;
    u8 g;
    u8 b;
} TextCmdColor;
typedef struct TextCmdScale
{
    u16 x;
    u16 y;
} TextCmdScale;

static u8 *Text_GetSubtext(u8 *text_data, int idx)
{
    int cur_idx = 0;

    static u8 opcode_sizes[] = {
        -1,
        1,
        1,
        1,
        1,
        1,
        1,
        4,
        1,
        1,
        4,
        0,
        3,
        0,
        4,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    };

    while (1)
    {
        u8 opcode = text_data[0];

        if (opcode == 0)
            return 0;

        else if (opcode == 7)
        {
            if (cur_idx == idx)
                return text_data;

            cur_idx++;
        }

        if (opcode >= GetElementsIn(opcode_sizes))
            text_data++;
        else
            text_data += opcode_sizes[opcode];

        text_data++;
    }
}
static u8 *Text_GetCommand(Text *text, int idx, TextCmdOpcode cmd)
{
    u8 *subtext = Text_GetSubtext(text->text_start, idx);

    if (!subtext)
        return 0;

    int cur_idx = 0;

    static u8 opcode_sizes[] = {
        -1,
        1,
        1,
        1,
        1,
        1,
        1,
        4,
        1,
        1,
        4,
        0,
        3,
        0,
        4,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    };

    while (1)
    {
        u8 opcode = subtext[0];

        if (opcode == 0)
            return 0;
        else if (opcode == cmd)
            return &subtext[1];

        if (opcode >= GetElementsIn(opcode_sizes))
            subtext++;
        else
            subtext += opcode_sizes[opcode];

        subtext++;
    }
}
static void Text_SetColor(Text *text, int idx, GXColor *col)
{
    TextCmdColor *col_cmd = (TextCmdColor *)Text_GetCommand(text, idx, TEXTCMD_COLOR);

    if (!col_cmd)
        return;

    col_cmd->r = col->r;
    col_cmd->g = col->g;
    col_cmd->b = col->b;
}
static void Text_SetScale(Text *text, int idx, float x, float y)
{
    TextCmdScale *scale_cmd = (TextCmdScale *)Text_GetCommand(text, idx, TEXTCMD_SCALE);

    if (!scale_cmd)
        return;

    scale_cmd->x = x * 256;
    scale_cmd->y = y * 256;
}
static void Text_GetWidthAndHeight(Text *t, int subtext_idx, float *width, float *height)
{
    void Text_DetermineHeightAndWidth(u8 * text_data, Text * text, float *width, float *height); // note: text_data must not start at the 7 opcode, skip past it by passing &text->text_start[5]

    u8 *subtext_ptr = Text_GetSubtext(t->text_start, subtext_idx);

    if (!subtext_ptr)
        return;

    // update temp variables that only the gx cb update
    t->temp.init_x = t->subtext_init_x;
    t->temp.init_y = t->subtext_init_y;
    t->temp.use_aspect = t->use_aspect;
    t->temp.kerning = t->kerning;
    t->temp.align = t->align;

    Text_DetermineHeightAndWidth(&subtext_ptr[5], t, width, height);
}

static int Text_CharToCommand(char c)
{
    // numbers
    if (c >= '0' && c <= '9')
    {
        return 0x2000 + (c - '0');
    }
    // capitals
    else if (c >= 'A' && c <= 'Z')
    {
        return 0x200a + (c - 'A');
    }
    // lower case
    else if (c >= 'a' && c <= 'z')
    {
        return 0x2024 + (c - 'a');
    }
    else
    {
        // symbol lookup
        struct ASCIISymbolLookup
        {
            u8 ascii;
            u16 text_code;
        };

        static struct ASCIISymbolLookup symbol_lookup[] = {
            {
                .ascii = ' ',
                .text_code = 0x20e3,
            },
            {
                .ascii = '!',
                .text_code = 0x20ec,
            },
            {
                .ascii = '"',
                .text_code = 0x20f4,
            },
            {
                .ascii = '#',
                .text_code = 0x2106,
            },
            {
                .ascii = '$',
                .text_code = 0x2104,
            },
            {
                .ascii = '%',
                .text_code = 0x2105,
            },
            {
                .ascii = '&',
                .text_code = 0x2107,
            },
            {
                .ascii = '(',
                .text_code = 0x20f5,
            },
            {
                .ascii = ')',
                .text_code = 0x20f6,
            },
            {
                .ascii = '*',
                .text_code = 0x2108,
            },
            {
                .ascii = '+',
                .text_code = 0x20fd,
            },
            {
                .ascii = ',',
                .text_code = 0x20e6,
            },
            {
                .ascii = '-',
                .text_code = 0x20fe,
            },
            {
                .ascii = '.',
                .text_code = 0x20e7,
            },
            {
                .ascii = '/',
                .text_code = 0x20f0,
            },
            {
                .ascii = ':',
                .text_code = 0x20e9,
            },
            {
                .ascii = ';',
                .text_code = 0x20ea,
            },
            {
                .ascii = '=',
                .text_code = 0x2100,
            },
            {
                .ascii = '?',
                .text_code = 0x20eb,
            },
            {
                .ascii = '@',
                .text_code = 0x2109,
            },
            {
                .ascii = '_',
                .text_code = 0x20ee,
            },
        };

        // find symbol
        u16 text_code = 0xFFFF;
        for (int i = 0; i < GetElementsIn(symbol_lookup); i++)
        {
            if (symbol_lookup[i].ascii == c)
            {
                text_code = symbol_lookup[i].text_code;
                break;
            }
        }

        // write out symbol
        if (text_code != 0xFFFF)
            return text_code;
        else
            return -1;
    }
}
static float Text_GetStringWidth(char *s, float scale)
{
    struct KerningData
    {
        u8 x0;
        u8 x1;
    };
    struct KerningData *kerning_data = (struct KerningData *)0x80509dc0;

    int width = 0;

    while (*s != '\0')
    {
        int character_cmd = Text_CharToCommand(*s);
        int kerning_idx = character_cmd & 0xFF;
        width += 32 - ((kerning_data[kerning_idx].x1 - 2) + kerning_data[kerning_idx].x0);
        s++;
    }

    // scale gets floored as an int in the text library, mimic this for accuracy
    float text_scale_adjusted = (int)(scale * (256.0)) * (1.0 / 256.0);

    return (float)width * text_scale_adjusted;
}

/*** Functions ***/
int Text_CreateCanvas(int sis_idx, int no_create_cam_gobj, int gobj_entityclass, int gobj_plink, int gobj_ppriority, int gxlink, int gxpri, int cobj_gxpri); // the optional gobj and cobj_gxlink are used to create a cobj as well. set gobj
Text *Text_CreateText(int sis_idx, int canvas_idx);
Text *Text_CreateTextManual(int sis_idx, int canvas_idx, float pos_x, float pos_y, float pos_z, float limit_x, float limit_y);
void Text_Destroy(Text *text);
int Text_AddSubtext(Text *text, float xPos, float yPos, char *string, ...);
void Text_SetScale(Text *text, int subtext, float x, float y);
// void Text_SetColor(Text *text, int subtext, GXColor *color);
void Text_SetPosition(Text *text, int subtext, float x, float y);
void Text_SetText(Text *text, int subtext, char *string, ...);
u8 *TextHeap_Alloc(int size);
void TextHeap_Free(u8 *alloc);
int Text_ConvertASCIIToShiftJIS(char *out, char *in);
void Text_GX(GOBJ *gobj, int pass); // 0x804516e4 - pass 0 = camera setup, pass 2 = draw + opcode walk.
void Text_LoadSisFile(int index, char *filename, char *symbol);
int Text_StorePremadeText(Text *text, int text_index); // 0x8044f9d4 - parse/count subtexts in SIS data.
void Text_InitPremadeText(Text *text, int text_index); // 0x8044f8c8 - set text from SIS slot entry by index.
void Text_DestroyAllSisCanvas(int sis_id);
void Text_DestroyCanvas(TextCanvas *);
void Text_InitSisHeap(int size);
void Text_DestroySisHeap();
// State-history stack helpers used by the renderer/composers when nesting
// COLOR/SCALE/POS/ALIGN opcodes. kind: 1=POS, 2=COLOR, 3=SCALE, 4=ALIGN, 5=int.
// High bit (0x80) on Pop = "pop without write" (sizer pass).
void Text_PushState(Text *text, void *value_ptr, u8 kind); // 0x80450828
void Text_PopState(Text *text, u8 kind);                   // 0x8045111c

/*** Variables ***/
// Text data
static int *stc_textheap_size = (int *)0x805de558;
static TextHeapCell **stc_textheap_start = (TextHeapCell **)0x805de55c;
static TextHeapCell **stc_textheap_free = (TextHeapCell **)0x805de560;

// Text GObj's data
static Text **stc_text_first = (Text **)0x805de568;

// Text canvas
static TextCanvas **stc_textcanvas_first = (TextCanvas **)0x805de56c;

// Sis Library
static HSD_Archive **stc_sis_archives = (HSD_Archive **)0x8059a848; // array of 5 sis file archive pointers
static SISData **stc_sis_data = (SISData **)0x8059a85c;             // array of 5 currently loaded sis data, indexed by sis_id

#endif