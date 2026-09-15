#ifndef MEX_H_HSD
#define MEX_H_HSD

#include "structs.h"
#include "datatypes.h"
#include "obj.h"
#include "gx.h"

// button bits
#define PAD_BUTTON_DPAD_LEFT 0x1
#define PAD_BUTTON_DPAD_RIGHT 0x2
#define PAD_BUTTON_DPAD_DOWN 0x4
#define PAD_BUTTON_DPAD_UP 0x8
#define PAD_TRIGGER_Z 0x10
#define PAD_TRIGGER_R 0x20
#define PAD_TRIGGER_L 0x40
#define PAD_BUTTON_A 0x100
#define PAD_BUTTON_B 0x200
#define PAD_BUTTON_X 0x400
#define PAD_BUTTON_Y 0x800
#define PAD_BUTTON_START 0x1000
#define PAD_BUTTON_UP 0x10000
#define PAD_BUTTON_DOWN 0x20000
#define PAD_BUTTON_LEFT 0x40000
#define PAD_BUTTON_RIGHT 0x80000

#define HSD_BUTTON_DPAD_LEFT 0x0001
#define HSD_BUTTON_DPAD_RIGHT 0x0002
#define HSD_BUTTON_DPAD_DOWN 0x0004
#define HSD_BUTTON_DPAD_UP 0x0008
#define HSD_TRIGGER_Z 0x0010
#define HSD_TRIGGER_R 0x0020
#define HSD_TRIGGER_L 0x0040
#define HSD_BUTTON_A 0x0100
#define HSD_BUTTON_B 0x0200
#define HSD_BUTTON_X 0x0400
#define HSD_BUTTON_Y 0x0800
#define HSD_BUTTON_START 0x1000
#define HSD_BUTTON_UP 0x10000
#define HSD_BUTTON_DOWN 0x20000
#define HSD_BUTTON_LEFT 0x40000
#define HSD_BUTTON_RIGHT 0x80000

#define HSD_VI_XFB_MAX 3

typedef void (*HSD_VIGXDrawDoneCallback)(int);
typedef void (*HSD_VIRetraceCallback)(u32);

typedef enum DebugLevel
{
    DB_MASTER,        // off
    DB_NO_DEBUG_ROM,  //
    DB_DEBUG_DEVELOP, //
    DB_DEBUG_ROM,     //
    DB_DEVELOP,       //
} DebugLevel;

typedef enum _HSD_VIXFBDrawDispStatus {
    HSD_VI_XFB_NONE,
    HSD_VI_XFB_NOUSE,
    HSD_VI_XFB_FREE,
    HSD_VI_XFB_DRAWING,
    HSD_VI_XFB_WAITDONE,
    HSD_VI_XFB_DRAWDONE,
    HSD_VI_XFB_NEXT,
    HSD_VI_XFB_DISPLAY,
    HSD_VI_XFB_COPYEFB,
    HSD_VI_XFB_TERMINATE
} HSD_VIXFBDrawDispStatus;

typedef enum _HSD_VIEFBDrawDispStatus {
    HSD_VI_EFB_FREE,
    HSD_VI_EFB_DRAWDONE,
    HSD_VI_EFB_TERMINATE
} HSD_VIEFBDrawDispStatus;

/*** Structs ***/

struct HSD_IDEntry
{
    struct HSD_IDEntry *next; // 0x00
    u32 id;                   // 0x04
    void *data;               // 0x08
};

struct HSD_IDTable
{
    struct HSD_IDEntry *table[101];
};

struct HSD_ClassInfoHead
{
    void (*info_init)();   // 0x00
    u32 flags;             // 0x04, 0x01 = is initialized
    char *library_name;    // 0x08
    char *class_name;      // 0x0C
    s16 obj_size;          // 0x10
    s16 info_size;         // 0x12
    HSD_ClassInfo *parent; // 0x14
    HSD_ClassInfo *next;   // 0x18
    HSD_ClassInfo *child;  // 0x1C
    u32 nb_exist;          // 0x20
    u32 nb_peak;           // 0x24
};

struct HSD_ClassInfo
{
    HSD_ClassInfoHead head;
    void *(*alloc)(HSD_ClassInfo *o);     // 0x28
    void (*init)(HSD_ClassInfo *o);          // 0x2C
    void (*release)(HSD_ClassInfo *o);       // 0x30
    void (*destroy)(HSD_ClassInfo *o);       // 0x34
    void (*amnesia)(HSD_ClassInfo *info); // 0x38
};

typedef struct _HSD_ObjAllocLink
{
    struct _HSD_ObjAllocLink *next;
} HSD_ObjAllocLink;

struct HSD_ObjAllocData
{
    u32 flags;                     // 0x00 - Technically 2 diff flags
    HSD_ObjAllocLink *freehead;    // 0x04
    u32 used;                      // 0x08
    u32 free;                      // 0x0C
    u32 peak;                      // 0x10
    u32 num_limit;                 // 0x14
    u32 heap_limit_size;           // 0x18
    u32 heap_limit_num;            // 0x1C
    u32 size;                      // 0x20
    u32 align;                     // 0x24
    struct HSD_ObjAllocData *next; // 0x28
};

struct HSD_Material
{
    GXColor ambient;
    GXColor diffuse;
    GXColor specular;
    float alpha;
    float shininess;
};

struct HSD_Pad
{
    int held;            // 0x0
    int heldPrev;        // 0x4
    int down;            // 0x8
    int rapidFire;       // 0xc
    int up;              // 0x10
    int rapidTimer;      // 0x14
    s8 stickX;           // 0x18
    s8 stickY;           // 0x19
    s8 substickX;        // 0x1a
    s8 substickY;        // 0x1b
    u8 triggerLeft;      // 0x1c
    u8 triggerRight;     // 0x1d
    float fstickX;       // 0x20
    float fstickY;       // 0x24
    float fsubstickX;    // 0x28
    float fsubstickY;    // 0x2c
    float ftriggerLeft;  // 0x30
    float ftriggerRight; // 0x34
    float x38;           // 0x38
    float x3c;           // 0x3c
    u8 x40;              // 0x40
    s8 status;           // 0x41   0 = plugged, -1 = unplugged
};

struct HSD_Pads
{
    HSD_Pad pad[4];
};

struct HSD_Update
{
    u8 is_req_exit_minor;              // 0x7e0
    u32 engine_frames;              // 0x7e4
    u32 rendered_frames;            // 0x7e8
    u32 x7ec;                       // 0x7ec,
    u8 pause_kind;                  // 0x7f0, 1 << PauseKind
    u8 pause_kind_prev;             // 0x7f1, 1 << PauseKind
    u8 is_frame_advance;            // 0x7f2, 0 = none, 1 = advance
    u8 is_frame_advance_prev;       // 0x7f3, 0 = none, 1 = advance
    int (*isRequestPause)();        // 0x7f4,
    int (*isRequestFrameAdvance)(); // 0x7f8,
    int x7fc;                       // 0x7fc
    u64 plink_blacklist;            // 0x800, p_links GObj_UpdateAll skips (bit = 1 << p_link); updateFunction ORs in stc_pause_plink_blacklists per set pause kind
    u64 plink_blacklist_prev;       // 0x808
    void *funcs;                    // 0x814
    int x818;                       // 0x818
    int x81c;                       // 0x81c
    int x820;                       // 0x820
    int x824;                       // 0x824
};

struct HSD_VI
{
    int x0;
    int x4;
    int is_prog;
};

typedef struct _HSD_VIStatus {
    GXRenderModeObj rmode;
    s32 black;
    u8 vf;
    GXGamma gamma;
    GXColor clear_clr;
    u32 clear_z;
    u8 update_clr;
    u8 update_alpha;
    u8 update_z;
} HSD_VIStatus;

typedef struct _current {
    struct _HSD_VIStatus vi;
    u8 chg_flag;
} Current;

typedef struct _XFB {
    void* buffer;
    HSD_VIXFBDrawDispStatus status;
    Current vi_all;
} XFB;

typedef struct _HSD_VIInfo {
    Current current;

    XFB xfb[3];

    struct _EFB {
        HSD_VIEFBDrawDispStatus status;
        Current vi_all;
    } efb;

    s32 nb_xfb;

    void (*pre_cb)();
    HSD_VIRetraceCallback post_cb;

    struct drawdone {
        s32 waiting;
        s32 arg;
        HSD_VIGXDrawDoneCallback cb;
    } drawdone;

    struct perf {
        s32 frame_period;
        s32 frame_renew;
    } perf;

} HSD_VIInfo;

// struct HSD_Archive
// {
//     int file_size;       // size of file
//     int *reloc_offset;   // pointer to relocation table offset?
//     int reloc_num;       // number of entries on the rleoc table
//     int symbol_num;      // total number of symbols
//     int refsymbol_num;   // number of reference symbols
//     int archive_vers;    // idk for sure sometimes 001B
//     int unk1;            //
//     int unk2;            //
//     int *general_points; // 0x20 = pointer to the "general points"
//     int *reloc_table;    // pointer to relocation table in memory
//     int *symbols1;       // pointer to symbol pointers and name offsets
//     int *refsymmbols;    // pointer to reference symbol info in memory
//     int *symbols2;       // pointer to symbol list in memory
//     int *file_start;     // pointer to the header of the dat
// };

struct HSD_ArchiveHeader
{
    u32 file_size; /* 0x00 */
    u32 data_size; /* 0x04 */
    u32 nb_reloc;  /* 0x08 */
    u32 nb_public; /* 0x0C */
    u32 nb_extern; /* 0x10 */
    u8 version[4]; /* 0x14 */
    u32 pad[2];    /* 0x18 */
};

struct HSD_ArchiveRelocationInfo
{
    u32 offset;
};

struct HSD_ArchivePublicInfo
{
    u32 offset; /* 0x00 */
    u32 symbol; /* 0x04 */
};

struct HSD_ArchiveExternInfo
{
    u32 offset; /* 0x00 */
    u32 symbol; /* 0x04 */
};

// Bit set in HSD_Archive.flags by Archive_Init for archives that own
// their file blob (loaded via Archive_LoadFile). Archive_Free asserts
// this bit is set before freeing - without it the function panics with
// "archive->flags & HSD_ARCHIVE_DONT_FREE failed in lbarchive.c:274".
// (The HAL flag name is misleading: the bit being SET is what permits
// the free; cleared means the archive wraps a borrowed buffer.)
#define HSD_ARCHIVE_DONT_FREE 0x1

struct HSD_Archive
{
    struct HSD_ArchiveHeader header;              /* 0x00 */
    u8 *data;                                     /* 0x20 */
    struct HSD_ArchiveRelocationInfo *reloc_info; /* 0x24 */
    struct HSD_ArchivePublicInfo *public_info;    /* 0x28 */
    struct HSD_ArchiveExternInfo *extern_info;    /* 0x2C */
    char *symbols;                                /* 0x30 */
    struct HSD_Archive *next;                     /* 0x34 */
    char *name;                                   /* 0x38 */
    u32 flags;                                    /* 0x3C - HSD_ARCHIVE_* bits */
    void *top_ptr;                                /* 0x40 */
};
typedef struct {
    u8 padstatus_arr_len;       // 0x00  (number of queue slots)
    u8 write_index;             // 0x01
    u8 read_index;              // 0x02
    u8 count;                   // 0x03
} HSD_PadQueueInfo;

/*** Static Variables ***/
static HSD_IDTable *stc_hsd_default_table = (HSD_IDTable *)0x804C23EC;
static HSD_VI *stc_HSD_VI = (HSD_VI *)0x8046b0f0;
static HSD_Update *stc_hsd_update = (HSD_Update *)0x80479d58;
static int **stc_rng_seed = (int **)0x805dcd38;
static HSD_Pad *stc_engine_pads = (HSD_Pad *)0x8058b634;
static u64 *stc_pause_plink_blacklists = (u64 *)0x80494f68; // per PauseKind, the p_links frozen while that kind is set (bit = 1 << p_link)
static HSD_PadQueueInfo *stc_hsd_padqueue = (HSD_PadQueueInfo *)0x8058b080;
static GXPixelFmt *stc_hsd_pixelfmt = (GXPixelFmt *)0x804d76c8;
static DebugLevel *stc_dblevel = (DebugLevel *)0x805DD630;
static int *hsd_rand_seed = (int *)0x805dcd30;
static HSD_VIInfo *hsd_vi_info = (HSD_VIInfo *)0x80589a80;

/*** Functions ***/

// NOTE: Archive_LoadFile internally allocates from a per-scene heap, so the
// returned pointer is only valid for the current scene (3D scene exit zeroes
// the struct in place). Don't cache returned archives across scene transitions -
// reload as needed instead. The matching free path is Archive_Free(0, archive),
// but it's optional: scene teardown reclaims the storage automatically.
HSD_Archive *Archive_LoadFile(char *filename);                                            // 0x800596b4
void Archive_GetSymbols(HSD_Archive *archive, void *symbol_out, char *symbol_name, ...);  // 0x80059520, symbol_out/symbol_name pairs terminated with 0, a missing symbol stores 0
void *Archive_GetPublicAddress(HSD_Archive *archive, char *symbol);                       // 0x8041e390
void Archive_Init(HSD_Archive *archive, void *file_data, int size); // 0x8041e224, sets HSD_ARCHIVE_DONT_FREE in archive->flags
void Archive_Free(int heap_id, HSD_Archive *archive);               // 0x80059628, heap_id matches the Heap_Alloc heap (0 for Archive_LoadFile)
char *Archive_GetExtern(HSD_Archive *archive, int index);                   // 0x8041e434, name of the nth extern symbol, 0 when out of range
void Archive_LocateExtern(HSD_Archive *archive, char *symbols, void *addr); // 0x8041e46c, points every reference to the extern named symbols at addr
int lbLoadArchive(HSD_Archive **out, char *file_name, ...); // 0x80059a20, r3 is usually 0, va args are symbol_ptr followed by symbol_name, terminate with 0
char *Archive_AppendExtension(char *filename); // 0x80058e84
int HSD_Randi(int max); // 0x8041e668, [0, max); returns 0 for max <= 0
float HSD_Randf(); // 0x8041e610
void *HSD_MemAlloc(int size); // 0x8041cee4
void HSD_Free(void *ptr); // 0x8041ceb4
// The allocator Archive_LoadFile goes through. Heap 0 selects the HSD
// heap and forwards to HSD_MemAlloc with that heap id made current, so it allocates
// from the same per-scene storage a loaded archive does and is reclaimed with it.
void *Heap_Alloc(int heap_id, int size); // 0x8005884c
// What HSD_MemAlloc/HSD_Free dispatch to: OSAllocFromHeap / OSFreeToHeap against
// HSD_GetHeapID(). Unusable before HSD_OSInit has made that heap - the id is -1
// and the alloc asserts.
void *HSD_HeapAlloc(int size); // 0x804101b4
void HSD_HeapFree(void *ptr);  // 0x80410214

// Bounds of the region HSD_OSInit (0x8040ff8c) hands to OSCreateHeap. hoshi
// carves its persistent storage off the front beforehand by bumping the start
// pointer; rewinding it before the call is how that storage is given back.
static u8 **stc_hsd_heap_start = (u8 **)0x805de290;
static u8 **stc_hsd_heap_end = (u8 **)0x805de294;

// Through OnBoot, HSD_MemAlloc is redirected to a bump allocator over that
// region, so a mark is its next address and a release rewinds to it. Only
// correct while nothing allocated since the mark is still held.
static inline void *HSD_ArenaMark(void)
{
    return *stc_hsd_heap_start;
}

static inline void HSD_ArenaRelease(void *mark)
{
    *stc_hsd_heap_start = (u8 *)mark;
}
void HSD_ObjAllocInit(HSD_ObjAllocData *data, size_t size, u32 align); // 0x8041825c
void *HSD_ObjAlloc(HSD_ObjAllocData *obj_def); // 0x804180e4
void HSD_ObjFree(HSD_ObjAllocData *obj_def, void *obj); // 0x80418234
void _hsdClassDestroy(void *hsd_class); // 0x80420b60, base class destroy, returns the object to its class memory pool
void HSD_ImageDescCopyFromEFB(_HSD_ImageDesc *image_desc, int left, int top, int clear_efb); // 0x803f7a7c, must be called from a cobj callback!
void HSD_StartRender(int unk); // 0x80410544
void EngineSpeed_Update(u64 ticks); // 0x80062874, engine frame duration in OS ticks, normal speed is bus_clock / 240
void HSD_StateInvalidate(int flags); // 0x803f898c
void HSD_StateInitTev(); // 0x803f9028
void HSD_StateInitDirect(GXVtxFmt vtxfmt, int render_flags); // 0x8040e390
void HSD_SetupChannel(void *unk); // 0x803f8a6c
void HSD_StateSetColorUpdate(GXBool update_enable); // 0x803f876c, enables or disables EFB color-buffer updates
void HSD_StateSetAlphaUpdate(GXBool update_enable); // 0x803f87b4, enables or disables EFB alpha-buffer updates
void HSD_StateSetDstAlpha(GXBool enable, u8 alpha); // 0x803f87fc
void HSD_StateSetAlphaCompare(GXCompare func0, u8 reference0,
                              GXAlphaOperation operation,
                              GXCompare func1, u8 reference1); // 0x803f86bc
void HSD_StateSetBlendMode(GXBlendMode type, GXBlendFactor src_factor,
                           GXBlendFactor dst_factor, GXLogicOp op); // 0x803f8528
void HSD_StateSetCullMode(GXCullMode cull_mode); // 0x803f84ec
void HSD_StateSetColorDither(GXBool dither); // 0x803f88b0
void HSD_StateSetLineWidth(u8 width, int tex_offsets); // 0x803f8488
void HSD_StateSetNumChans(int num); // 0x803f8f08
void HSD_StateSetNumTevStages(); // 0x803f905c, commits the accumulated TEV stage count to GX and resets it
void HSD_StateSetNumTexGens(); // 0x803f8ff8, commits the accumulated texgen count to GX and resets it
void HSD_StateSetPointSize(u8 size, int tex_offsets); // 0x803f8658
void HSD_StateSetZCompLoc(GXBool enable); // 0x803f8868
void HSD_StateSetZMode(GXBool compare_enable, GXCompare func, GXBool update_enable); // 0x803f85c8
void HSD_ClearVtxDesc(); // 0x80405b3c
void HSD_VICopyXFBAsync(int unk); // 0x804112b4
int HSD_VIGetDrawDoneWaitingFlag(); // 0x8041104c
void HSD_VICopyEFB2XFBPtr(HSD_VIStatus *vi, void *buffer, int rpass); // 0x8041105c
void HSD_PadFlushQueue(int type); // 0x804125f8, 0 = merge into newest, 1 = drop all, 2 = keep newest
void HSD_PadRenewMasterStatus(); // 0x80413a6c, consumes one queued PAD read into the master status
void HSD_PadRenewCopyStatus(); // 0x80413d84, derives stc_engine_pads (edges, repeat) from the master status
void HSD_PadRumbleInterpret(); // 0x80414b88
void HSD_VIPostRetraceCB(int retrace_count); // 0x80410dfc
void HSD_UpdateAllCObjs(); // 0x8042a1a8
_HSD_ImageDesc *GX_AllocImageData(_HSD_ImageDesc *image_desc, int width, int height, int fmt); // 0x8028b050, allocates the image buffer from the HSD heap into image_desc
u64 Pad_GetDown(int pad); // 0x8000ecf0
u64 Pad_GetRapidHeld(int pad); // 0x8000ed30
u64 Pad_GetHeld(int pad); // 0x8000ecd0
void Pad_StartRumble(int pad, int unk, int kind, int duration); // 0x80071d00, make unk = 0
void HSD_DumpClassStat(void *class_info, int recursive, int level); // 0x804213a4, OSReports class object counts, class_info 0 dumps from the root class
void HSD_ObjDumpStat();                       // 0x804106a0
HSD_ObjAllocData *HSD_IDGetAllocData(); // 0x8041a53c
HSD_ObjAllocData *HSD_AObjGetAllocData(); // 0x803fb16c
HSD_ObjAllocData *HSD_FObjGetAllocData(); // 0x80403594
void HSD_IDInsertToTable(HSD_IDTable *id_table, u32 id, void *data); // 0x8041a5a8
void HSD_IDRemoveByIDFromTable(HSD_IDTable *id_table, u32 id); // 0x8041a694
void *HSD_IDGetDataFromTable(HSD_IDTable *id_table, u32 id, int *success); // 0x8041a740
int HSD_GetHeapID(); // 0x80410338
void HSD_SetHeapID(int heap); // 0x8041037c

#endif