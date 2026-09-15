#ifndef MEX_H_OS
#define MEX_H_OS

// #include <stdarg.h>

#include "structs.h"
#include "datatypes.h"

#include <math.h>

typedef s64 OSTime;

char *strrchr(const char *, int); // 0x803b7df8

// OS Macros
#define OSRoundUp32B(x) (((u32)(x) + 32 - 1) & ~(32 - 1))
#define OSRoundDown32B(x) (((u32)(x)) & ~(32 - 1))
#define OSRoundUp512B(x) (((u32)(x) + 512 - 1) & ~(512 - 1)) // using this for card reads
#define OSRoundDown512B(x) (((u32)(x)) & ~(512 - 1))         // using this for card reads
#define OSTicksToMilliseconds(ticks) ((float)(ticks) / ((os_info->bus_clock / 4) / 1000))
#define OSMillisecondsToTicks(ms) ((ms) * ((os_info->bus_clock / 4) / 1000))
#define OSTicksToMicroseconds(ticks) ((ticks) / ((os_info->bus_clock / 4) / 1000000))
#define MillisecondsSinceTick(ticks) ((float)OSTicksToMicroseconds(OSGetTick() - ticks) / 1000) // returns microseconds between tick given and the current tick
#define BytesToKB(bytes) ((float)(bytes) / 1000.0)
#define BytesToMB(bytes) ((float)(bytes) / 1000000.0)
#define BitCheck(num, bit) !!((num) & (1 << (bit))) // returns 0 or 1
#define BitCheck(num, bit) !!((num) & (1 << (bit))) // returns 0 or 1
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define assert(msg) __assert(__FILENAME__, __LINE__, msg)
#define divide_roundup(dividend, divisor) (ceil((float)dividend / (float)divisor))
#define MTXDegToRad(a) ((a) * 0.01745329252f)
#define MTXRadToDeg(a) ((a) * 57.29577951f)
#define SYS_BASE_CACHED (0x80000000)
#define SYS_BASE_UNCACHED (0xC0000000)
#define MEM_VIRTUAL_TO_PHYSICAL(x) (((u32)(x)) & ~SYS_BASE_UNCACHED)      /*!< Cast virtual address to physical address, e.g. 0x8xxxxxxx -> 0x0xxxxxxx */
#define MEM_PHYSICAL_TO_K0(x) (void *)((u32)(x) + SYS_BASE_CACHED)        /*!< Cast physical address to cached virtual address, e.g. 0x0xxxxxxx -> 0x8xxxxxxx */
#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((0x01 << (w)) - 1)) << (s))) // mask the first w bits of v before lshifting
#define _SHIFTR(v, s, w) ((u32)(((u32)(v) >> (s)) & ((0x01 << (w)) - 1))) // rshift v and mask the first w bits afterwards
#define tostr(s) xstr(s)
#define xstr(s) #s

// #define INFINITY ((float)(HUGE_ENUF * HUGE_ENUF))
// #define M_PI (3.14159265358979323846)
#define M_1DEGREE (0.0174533)
#define M_NAN ((float)(INFINITY * 0.0F))

/** Heap Definitions */ //
#define OS_MAX_HEAPS 16

/** Console Definitions */ //
#define OS_CONSOLE_RETAIL4 0x00000004
#define OS_CONSOLE_RETAIL3 0x00000003
#define OS_CONSOLE_RETAIL2 0x00000002
#define OS_CONSOLE_RETAIL1 0x00000001
#define OS_CONSOLE_DEVHW4 0x10000007
#define OS_CONSOLE_DEVHW3 0x10000006
#define OS_CONSOLE_DEVHW2 0x10000005
#define OS_CONSOLE_DEVHW1 0x10000004
#define OS_CONSOLE_MINNOW 0x10000003
#define OS_CONSOLE_ARTHUR 0x10000002
#define OS_CONSOLE_PC_EMULATOR 0x10000001
#define OS_CONSOLE_EMULATOR 0x10000000
#define OS_CONSOLE_DEVELOPMENT 0x10000000 // bit mask

#define CARD_BLOCK_SIZE 8192
#define CARD_MAX_FILE 127
#define CARD_FILENAME_MAX 32
#define CARD_ICON_MAX 8
#define CARD_COMMENT_SIZE 64 // two 32-byte strings: title + description
#define CARD_WORKAREA_SIZE (5 * 8 * 1024)
#define CARD_READ_SIZE 512

// CARDStat.bannerFormat (low 2 bits)
#define CARD_STAT_BANNER_NONE 0
#define CARD_STAT_BANNER_C8 1     // CI8 indexed + 256-entry RGB5A3 TLUT
#define CARD_STAT_BANNER_RGB5A3 2 // direct color
#define CARD_STAT_BANNER_MASK 3
// CARDStat.iconFormat / iconSpeed are 2-bit-per-frame bitfields, indexed by frame (0..CARD_ICON_MAX-1)
#define CARD_STAT_ICON_NONE 0
#define CARD_STAT_ICON_C8 1     // CI8 indexed, shares one 256-entry RGB5A3 TLUT across all frames
#define CARD_STAT_ICON_RGB5A3 2 // direct color
#define CARD_STAT_ICON_MASK 3
#define CARD_STAT_SPEED_NONE 0 // also marks the end of the animation
#define CARD_STAT_SPEED_FAST 1
#define CARD_STAT_SPEED_MIDDLE 2
#define CARD_STAT_SPEED_SLOW 3
#define CARD_STAT_SPEED_MASK 3
// tile image geometry / byte sizes
#define CARD_BANNER_WIDTH 96
#define CARD_BANNER_HEIGHT 32
#define CARD_ICON_WIDTH 32
#define CARD_ICON_HEIGHT 32
#define CARD_BANNER_SIZE_RGB5A3 (CARD_BANNER_WIDTH * CARD_BANNER_HEIGHT * 2) // 6144
#define CARD_ICON_SIZE_RGB5A3 (CARD_ICON_WIDTH * CARD_ICON_HEIGHT * 2)       // 2048
#define CARD_ICON_SIZE_C8 (CARD_ICON_WIDTH * CARD_ICON_HEIGHT)              // 1024 (per frame, + shared 512B TLUT)
#define CARD_TLUT_SIZE 512                                                  // 256 entries * RGB5A3
#define CARD_RESULT_UNLOCKED 1
#define CARD_RESULT_READY 0
#define CARD_RESULT_BUSY -1
#define CARD_RESULT_WRONGDEVICE -2
#define CARD_RESULT_NOCARD -3
#define CARD_RESULT_NOFILE -4
#define CARD_RESULT_IOERROR -5
#define CARD_RESULT_BROKEN -6
#define CARD_RESULT_EXIST -7
#define CARD_RESULT_NOENT -8
#define CARD_RESULT_INSSPACE -9
#define CARD_RESULT_NOPERM -10
#define CARD_RESULT_LIMIT -11
#define CARD_RESULT_NAMETOOLONG -12
#define CARD_RESULT_ENCODING -13
#define CARD_RESULT_CANCELED -14
#define CARD_RESULT_FATAL_ERROR -128

// PAD
#define PAD_CHAN0_BIT 0x80000000
#define PAD_CHAN1_BIT 0x40000000
#define PAD_CHAN2_BIT 0x20000000
#define PAD_CHAN3_BIT 0x10000000

// courtesy of libogc
#define SI_CHAN0 0
#define SI_CHAN1 1
#define SI_CHAN2 2
#define SI_CHAN3 3

#define SI_ERROR_UNDER_RUN 0x0001
#define SI_ERROR_OVER_RUN 0x0002
#define SI_ERROR_COLLISION 0x0004
#define SI_ERROR_NO_RESPONSE 0x0008
#define SI_ERROR_WRST 0x0010
#define SI_ERROR_RDST 0x0020    // nothing is attached
#define SI_ERROR_UNKNOWN 0x0040 // unknown device is attached
#define SI_ERROR_BUSY 0x0080    // still detecting

//
// CMD_TYPE_AND_STATUS response data
//
#define SI_TYPE_MASK 0x18000000u
#define SI_TYPE_N64 0x00000000u
#define SI_TYPE_DOLPHIN 0x08000000u
#define SI_TYPE_GC SI_TYPE_DOLPHIN

// GameCube specific
#define SI_GC_WIRELESS 0x80000000u
#define SI_GC_NOMOTOR 0x20000000u  // no rumble motor
#define SI_GC_STANDARD 0x01000000u // dolphin standard controller

// WaveBird specific
#define SI_WIRELESS_RECEIVED 0x40000000u // 0: no wireless unit
#define SI_WIRELESS_IR 0x04000000u       // 0: IR  1: RF
#define SI_WIRELESS_STATE 0x02000000u    // 0: variable  1: fixed
#define SI_WIRELESS_ORIGIN 0x00200000u   // 0: invalid  1: valid
#define SI_WIRELESS_FIX_ID 0x00100000u   // 0: not fixed  1: fixed
#define SI_WIRELESS_TYPE 0x000f0000u
#define SI_WIRELESS_LITE_MASK 0x000c0000u // 0: normal 1: lite controller
#define SI_WIRELESS_LITE 0x00040000u      // 0: normal 1: lite controller
#define SI_WIRELESS_CONT_MASK 0x00080000u // 0: non-controller 1: non-controller
#define SI_WIRELESS_CONT 0x00000000u
#define SI_WIRELESS_ID 0x00c0ff00u
#define SI_WIRELESS_TYPE_ID (SI_WIRELESS_TYPE | SI_WIRELESS_ID)

#define SI_N64_CONTROLLER (SI_TYPE_N64 | 0x05000000)
#define SI_N64_MIC (SI_TYPE_N64 | 0x00010000)
#define SI_N64_KEYBOARD (SI_TYPE_N64 | 0x00020000)
#define SI_N64_MOUSE (SI_TYPE_N64 | 0x02000000)
#define SI_GBA (SI_TYPE_N64 | 0x00040000)
#define SI_GC_CONTROLLER (SI_TYPE_GC | SI_GC_STANDARD)
#define SI_GC_RECEIVER (SI_TYPE_GC | SI_GC_WIRELESS)
#define SI_GC_WAVEBIRD (SI_TYPE_GC | SI_GC_WIRELESS | SI_GC_STANDARD | SI_WIRELESS_STATE | SI_WIRELESS_FIX_ID)
#define SI_GC_KEYBOARD (SI_TYPE_GC | 0x00200000)
#define SI_GC_STEERING (SI_TYPE_GC | 0x00000000)

// VI
#define VI_XFBMODE_SF 0
#define VI_XFBMODE_DF 1
#define VI_MAX_WIDTH_NTSC 720
#define VI_MAX_HEIGHT_NTSC 480
#define VI_MAX_WIDTH_PAL 720
#define VI_MAX_HEIGHT_PAL 576
#define VI_MAX_WIDTH_MPAL 720
#define VI_MAX_HEIGHT_MPAL 480
#define VI_MAX_WIDTH_EURGB60 VI_MAX_WIDTH_NTSC
#define VI_MAX_HEIGHT_EURGB60 VI_MAX_HEIGHT_NTSC

#define VI_NTSC 0      /*!< Video standard used in North America and Japan */
#define VI_PAL 1       /*!< Video standard used in Europe */
#define VI_MPAL 2      /*!< Video standard, similar to NTSC, used in Brazil */
#define VI_DEBUG 3     /*!< Video standard, for debugging purpose, used in North America and Japan. Special decoder needed */
#define VI_DEBUG_PAL 4 /*!< Video standard, for debugging purpose, used in Europe. Special decoder needed */
#define VI_EURGB60 5   /*!< RGB 60Hz, 480 lines mode (same timing and aspect ratio as NTSC) used in Europe */

#define VI_INTERLACE 0     /*!< Video mode INTERLACED. */
#define VI_NON_INTERLACE 1 /*!< Video mode NON INTERLACED */
#define VI_PROGRESSIVE 2   /*!< Video mode PROGRESSIVE. Special mode for higher quality */

#define VI_TVMODE(fmt, mode) (((fmt) << 2) + (mode))
#define VI_TVMODE_NTSC_INT VI_TVMODE(VI_NTSC, VI_INTERLACE)
#define VI_TVMODE_NTSC_DS VI_TVMODE(VI_NTSC, VI_NON_INTERLACE)
#define VI_TVMODE_NTSC_PROG VI_TVMODE(VI_NTSC, VI_PROGRESSIVE)
#define VI_TVMODE_PAL_INT VI_TVMODE(VI_PAL, VI_INTERLACE)
#define VI_TVMODE_PAL_DS VI_TVMODE(VI_PAL, VI_NON_INTERLACE)
#define VI_TVMODE_PAL_PROG VI_TVMODE(VI_PAL, VI_PROGRESSIVE)
#define VI_TVMODE_EURGB60_INT VI_TVMODE(VI_EURGB60, VI_INTERLACE)
#define VI_TVMODE_EURGB60_DS VI_TVMODE(VI_EURGB60, VI_NON_INTERLACE)
#define VI_TVMODE_EURGB60_PROG VI_TVMODE(VI_EURGB60, VI_PROGRESSIVE)
#define VI_TVMODE_MPAL_INT VI_TVMODE(VI_MPAL, VI_INTERLACE)
#define VI_TVMODE_MPAL_DS VI_TVMODE(VI_MPAL, VI_NON_INTERLACE)
#define VI_TVMODE_MPAL_PROG VI_TVMODE(VI_MPAL, VI_PROGRESSIVE)
#define VI_TVMODE_DEBUG_INT VI_TVMODE(VI_DEBUG, VI_INTERLACE)
#define VI_TVMODE_DEBUG_PAL_INT VI_TVMODE(VI_DEBUG_PAL, VI_INTERLACE)
#define VI_TVMODE_DEBUG_PAL_DS VI_TVMODE(VI_DEBUG_PAL, VI_NON_INTERLACE)

// credit to libogc
#ifndef ATTRIBUTE_ALIGN
# define ATTRIBUTE_ALIGN(v)					__attribute__((aligned(v)))
#endif
#ifndef ATTRIBUTE_PACKED
# define ATTRIBUTE_PACKED					__attribute__((packed))
#endif

/*** Structs ***/
struct OSInfo
{
    // info obtained from https://www.gc-forever.com/yagcd/chap4.html#sec4.2.1

    char gameName[4];      // 0x80000000
    char company[2];       // 0x80000004
    u8 disk_id;            // 0x80000006
    u8 disk_version;       // 0x80000007
    u8 is_audiostream;     // 0x80000008
    u8 streambuffer_size;  // 0x80000009
    int xc;                // 0x8000000C
    int x10;               // 0x80000010
    int x14;               // 0x80000014
    int x18;               // 0x80000018
    int dvd_magicword;     // 0x8000001C
    int boot_magicword;    // 0x80000020
    int sys_version;       // 0x80000024
    int mem_size;          // 0x80000028
    int console_type;      // 0x8000002C
    int arena_lo;          // 0x80000030
    int arena_hi;          // 0x80000034
    void *fst;             // 0x80000038
    int fst_maxsize;       // 0x8000003C
    int x40;               // 0x80000040
    int x44;               // 0x80000044
    int x48;               // 0x80000048
    int x4C;               // 0x8000004C
    int x50;               // 0x80000050
    int x54;               // 0x80000054
    int x58;               // 0x80000058
    int x5C;               // 0x8000005C
    int x60;               // 0x80000060
    int x64;               // 0x80000064
    int x68;               // 0x80000068
    int x6C;               // 0x8000006C
    int x70;               // 0x80000070
    int x74;               // 0x80000074
    int x78;               // 0x80000078
    int x7C;               // 0x8000007C
    int x80;               // 0x80000080
    int x84;               // 0x80000084
    int x88;               // 0x80000088
    int x8C;               // 0x8000008C
    int x90;               // 0x80000090
    int x94;               // 0x80000094
    int x98;               // 0x80000098
    int x9C;               // 0x8000009C
    int xA0;               // 0x800000A0
    int xA4;               // 0x800000A4
    int xA8;               // 0x800000A8
    int xAC;               // 0x800000AC
    int xB0;               // 0x800000B0
    int xB4;               // 0x800000B4
    int xB8;               // 0x800000B8
    int xBC;               // 0x800000BC
    int xC0;               // 0x800000C0
    int xC4;               // 0x800000C4
    int xC8;               // 0x800000C8
    int tv_mode;           // 0x800000CC
    int aram_size;         // 0x800000D0
    int xD4;               // 0x800000D4
    int xD8;               // 0x800000D8
    int xDC;               // 0x800000DC
    int xE0;               // 0x800000E0
    OSThread *curr_osthread;// 0x800000E4
    int xE8;               // 0x800000E8
    int xEC;               // 0x800000EC
    int simulated_memsize; // 0x800000F0
    void *dvd_BI2;         // 0x800000F4
    int bus_clock;         // 0x800000F8
    int cpu_clock;         // 0x800000FC
};
struct OSCalendarTime
{
    int sec;  // seconds after the minute [0, 61]
    int min;  // minutes after the hour [0, 59]
    int hour; // hours since midnight [0, 23]
    int mday; // day of the month [1, 31]
    int mon;  // month since January [0, 11]
    int year; // years in AD [1, ...]
    int wday; // days since Sunday [0, 6]
    int yday; // days since January 1 [0, 365]

    int msec; // milliseconds after the second [0,999]
    int usec; // microseconds after the millisecond [0,999]
};
struct OSAlarm
{
    void *cb; // 0x0

    OSTime fire;
    OSAlarm *prev;
    OSAlarm *next;

    // Periodic alarm
    OSTime period;
    OSTime start;
};
struct OSContext
{
    u32 gprs[0x20];          // r0-r31
    u32 cr;                  // 0x80
    u32 lr;                  // 0x84
    u32 ctr;                 // 0x88
    u32 xer;                 // 0x8c
    u64 fprs[0x20];          // f0-f31
    u64 fpscr;               // 0x190
    u32 srr0;                // 0x198 - saved PC
    u32 srr1;                // 0x19c - saved MSR
    u16 state;               // 0x1a2; last bit means OSSaveFPUContext was called, second last bit means the GPRs were saved by the exception handler
    u64 gqrs[4];             // 0x1a4
    u64 pairedSingles[0x20]; // starting at 0x1c8
};
struct OSHeapCell
{
    OSHeapCell *prev;
    OSHeapCell *next;
    int size;
};

typedef struct OSHeap
{
    int size;
    OSHeapCell *free;
    OSHeapCell *allocated;
} OSHeap;

struct OSThreadQueue
{
    OSThread *head;
    OSThread *tail;
};

typedef struct OSMutex {
    OSThreadQueue queue;  // at 0x0
    OSThread* thread;     // at 0x8
    s32 lock;             // at 0xC
    struct OSMutex* next; // at 0x10
    struct OSMutex* prev; // at 0x14
} OSMutex;

struct OSMutexQueue
{
    OSMutex *head;
    OSMutex *tail;
};

struct OSThreadLink
{
    OSThread *next;
    OSThread *prev;
};
struct OSThread
{
    OSContext context; // register context

    u16 state;    // OS_THREAD_STATE_*
    u16 attr;     // OS_THREAD_ATTR_*
    s32 suspend;  // suspended if the count is greater than zero
    s32 priority; // effective scheduling priority
    s32 base;     // base scheduling priority
    void *val;    // exit value

    OSThreadQueue *queue; // queue thread is on
    OSThreadLink link;    // queue link

    OSThreadQueue queueJoin; // list of threads waiting for termination (join)

    OSMutex *mutex;          // mutex trying to lock
    OSMutexQueue queueMutex; // list of mutexes owned

    OSThreadLink linkActive; // link of all threads for debugging

    u8 *stackBase; // the thread's designated stack (high address)
    u32 *stackEnd; // last word of stack (low address)

    s32 error;
    void *specific[2]; // thread specific data
};

struct CARDStat
{
    // read-only (Set by CARDGetStatus)
    char fileName[CARD_FILENAME_MAX];
    u32 length;
    u32 time; // seconds since midnight 01/01/2000
    u8 gameName[4];
    u8 company[2];

    // read/write (Set by CARDGetStatus/CARDSetStatus)
    u8 bannerFormat;
    u32 iconAddr;
    u16 iconFormat;
    u16 iconSpeed;
    u32 commentAddr;

    // read-only (Set by CARDGetStatus)
    u32 offsetBanner;
    u32 offsetBannerTlut;
    u32 offsetIcon[CARD_ICON_MAX];
    u32 offsetIconTlut;
    u32 offsetData;
};
struct CARDFileInfo
{
    s32 chan;
    s32 fileNo;

    s32 offset;
    s32 length;
    u16 iBlock;
    u16 __padding;
};
struct RGB565
{
    unsigned short r : 5;
    unsigned short g : 6;
    unsigned short b : 5;
};
struct MTHPlayParam
{
    int on_frame; // frame to apply the below frame rate (offset from last frame rate change)
    int rate;     // in game frames per mth frame
};
struct MTHHeader
{
    char magic[4];      // 0x0, idk why they call this magic
    int x4;             //
    int version;        // 0x8
    int bufSize;        // 0xc
    int xSize;          // 0x10
    int ySize;          // 0x14
    int framerate;      // 0x18
    int numFrames;      // 0x1c
    int firstFrame;     // 0x20
    int frameOffsets;   // 0x24
    int firstFrameSize; // 0x28
    void *x2c;          // 0x2c
    void *x30;          // 0x30
    void *x34;          // 0x34
    void *x38;          // 0x38
    void *x3c;          // 0x3c
};
struct MTHPlayback
{
    MTHHeader header;
    int numFrames;            // 0x40
    int xSize;                // 0x44
    int ySize;                // 0x48
    void **jpeg_lookup;       // 0x4c
    void *decoded_bright;     // 0x50
    void *decoded_chromeb;    // 0x54
    void *decoded_chromer;    // 0x58
    void *x5c;                // 0x5c
    void *x60;                // 0x60
    void *x64;                // 0x64
    int loop;                 // 0x68
    void *x6c;                // 0x6c
    void *x70;                // 0x70
    void *x74;                // 0x74
    int x78;                  // 0x78
    void *x7c;                // 0x7c
    int x80;                  // 0x80
    void *x84;                // 0x84
    void *x88;                // 0x88
    void *x8c;                // 0x8c
    void *x90;                // 0x90
    void *x94;                // 0x94
    void *x98;                // 0x98
    void *x9c;                // 0x9c
    void *xa0;                // 0xa0
    void *xa4;                // 0xa4
    void *xa8;                // 0xa8
    void *xac;                // 0xac
    void *xb0;                // 0xb0
    void *xb4;                // 0xb4
    void *xb8;                // 0xb8
    void *xbc;                // 0xbc
    void *xc0;                // 0xc0
    void *xc4;                // 0xc4
    void *xc8;                // 0xc8
    void *xcc;                // 0xcc
    void *xd0;                // 0xd0
    void *xd4;                // 0xd4
    void *xd8;                // 0xd8
    void *xdc;                // 0xdc
    void *xe0;                // 0xe0
    void *xe4;                // 0xe4
    void *xe8;                // 0xe8
    void *xec;                // 0xec
    void *xf0;                // 0xf0
    void *xf4;                // 0xf4
    void *xf8;                // 0xf8
    void *xfc;                // 0xfc
    int bufSize;              // 0x100
    int jpeg_cache_num;       // 0x104
    void *x108;               // 0x108
    void *x10c;               // 0x10c
    void *is_loading_frame;   // 0x110
    void *x114;               // 0x114
    void *x118;               // 0x118
    int x11c;                 // 0x11c
    int next_jpeg_offset;     // 0x120, next offset to read on disc
    void *x124;               // 0x124
    int entrynum;             // 0x128
    MTHPlayParam *play_param; // 0x12c
    void *x130;               // 0x130
    void *x134;               // 0x134
    void *x138;               // 0x138
    void *x13c;               // 0x13c
    void *x140;               // 0x140
    void *x144;               // 0x144
    void *x148;               // 0x148
    int power;                // 0x14C
    OSAlarm alarm;            // 0x150
};
struct JPEGHeader
{
    int nextSize;  // 0x0
    int prevSize;  // 0x4
    int imageSize; // 0x8
    int audioSize; // 0xc
};

struct FSTEntry
{
    unsigned int is_dir : 8;           // 0x0
    unsigned int filename_offset : 24; // 0x1
    union
    {
        struct
        {
            u32 startAddr; // 0x4
            u32 length;    // 0x8
        } file;
        struct
        {
            u32 x4;            // 0x4
            u32 last_entrynum; // 0x8
        } dir;
    } u;
};
struct DVDDiskID
{
    char gameName[4];
    char company[2];
    u8 diskNumber;
    u8 gameVersion;
    u8 streaming;
    u8 streamingBufSize; // 0 = default
    u8 padding[14];      // 0's are stored
    u32 rvlMagic;        // Revolution disk magic number
    u32 gcMagic;         // GC magic number is here
};
struct DVDCommandBlock
{
    DVDCommandBlock *next; // 0x00
    DVDCommandBlock *prev; // 0x04
    u32 command;           // 0x08
    s32 state;             // 0x0C
    u32 offset;            // 0x10
    u32 length;            // 0x14
    void *addr;            // 0x18
    u32 currTransferSize;  // 0x1C
    u32 transferredSize;   // 0x20
    DVDDiskID *id;         // 0x24
    void *callback;        // 0x28
    void *userData;        // 0x2C
};
struct DVDFileInfo
{
    DVDCommandBlock cb; // 0x0
    u32 startAddr;      // disk address of file, 0x30
    u32 length;         // file size in bytes, 0x34
    void *callback;     // 0x38
    void *file;         // 0x3C
};
struct DVDDir
{
    u32 entryNum;
    u32 location;
    u32 next;
};
struct DVDDirEntry
{
    u32 entryNum;
    int isDir;
    char *name;
};

typedef struct PADStatus
{
    u16 button;      // 0x0, Or-ed PAD_BUTTON_* and PAD_TRIGGER_* bits
    s8 stickX;       // 0x2, -128 <= stickX       <= 127
    s8 stickY;       // 0x3, -128 <= stickY       <= 127
    s8 substickX;    // 0x4, -128 <= substickX    <= 127
    s8 substickY;    // 0x5, -128 <= substickY    <= 127
    u8 triggerLeft;  // 0x6,   0 <= triggerLeft  <= 255
    u8 triggerRight; // 0x7,   0 <= triggerRight <= 255
    u8 analogA;      // 0x8,   0 <= analogA      <= 255
    u8 analogB;      // 0x9,   0 <= analogB      <= 255
    s8 err;          // 0xa, one of PAD_ERR_* number
} PADStatus;

struct FileReadParam
{
    u8 xc0 : 2; // 0xc0
    u8 x38 : 3; // 0x38
    u8 x07 : 3; // 0x07, 0 = unk, 1 = using dram address, 2 = unk, 3 = using aram address, evidenced by 80016708
};

typedef struct SIXYLookup
{
    u16 line;
    u8 cnt;
    u8 x3;
} SIXYLookup;

typedef struct OSReportData
{
    int flags : 8;   // 0x0
    char *text;      // 0x4
    int size;        // 0x8
    int cursor;      // 0xc
    u8 x10;          // 0x10
    u8 x11;          // 0x11
    int x14;         // 0x14
    int newline_num; // 0x18
    int cursor_temp; // 0x1c
} OSReportData;

/*** Static Vars ***/
static OSInfo *os_info = (OSInfo *)0x80000000;
static OSHeap **__OSHeapTable = (OSHeap **)0x805ddeb0;
static int *stc_fst_totalentrynum = (int *)0x805ddd94;
static FSTEntry **stc_fst_entries = (FSTEntry **)0x805ddd8c; // indexed by entrynum (0 is always the root directory)
static char **stc_fst_filenames = (char **)0x805ddd90;       // use FSTEntry.filename_offset to find an entrynums name
static int *stc_si_sampling_rate = (int *)0x804D740C;
static SIXYLookup *stc_si_xy = (SIXYLookup *)0x80402ca0;
static OSReportData *osreport_data = (OSReportData *)0x8058d198;

/*** OS Library ***/
typedef void *(*OSThreadStartFunction)(void *param);
typedef void (*DVDReadCallback)(int result, DVDFileInfo *fileInfo);
typedef void (*FileReadCallback)(int result, void *arg);

int OSGetTick(); // 0x803db530
u64 OSGetTime(); // 0x803db518
void OSTicksToCalendarTime(u64 time, OSCalendarTime *td); // 0x803db790
u64 __cvt_dbl_usll(double num); // 0x803ae0f0
void OSCancelThread(OSThread* thread); // 0x803da39c
long OSCheckActiveThreads(void); // 0x803dad1c
int OSCreateThread(OSThread* thread, OSThreadStartFunction func, void* param, void* stackBase, u32 stackSize, s32 priority, u16 attribute); // 0x803da0d0
void OSExitThread(void* val); // 0x803da2b8
s32 OSResumeThread(OSThread* thread); // 0x803da698
void OSSleepThread(OSThreadQueue* queue); // 0x803daa90
s32 OSSuspendThread(OSThread* thread); // 0x803da920
void OSWakeupThread(OSThreadQueue* queue); // 0x803dab7c
void OSCreateAlarm(OSAlarm *alarm); // 0x803d2e80
void OSSetAlarm(OSAlarm *alarm, OSTime tick, void *handler); // 0x803d30e0
void OSSetPeriodicAlarm(OSAlarm *alarm, OSTime start, OSTime period, void *handler); // 0x803d3148
void OSCancelAlarm(OSAlarm *alarm); // 0x803d31c4
void OSReport(char *, ...); // 0x803d4ce8
void OSPanic(const char *file, int line, const char *msg, ...); // 0x803d4d68
void OSRegisterResetFunction(void *info); // 0x803d8574
void __assert(char *file, int line, char *assert); // 0x804284b8
int OSCreateHeap(void *heap_lo, void *heap_hi); // 0x803d3804
void OSDestroyHeap(int heap_id); // 0x803d3870
void *OSAllocFromHeap(int heap_id, int size); // 0x803d360c
void OSFreeToHeap(int heap_id, void *alloc); // 0x803d3708
int OSCheckHeap(int heap); // 0x803d3884
void *OSAllocFromArenaLo(int size, int align); // 0x803d3c04
int OSGetPhysicalMemSize(); // 0x803d7b30
int OSGetConsoleType(); // 0x803d2470
int OSDisableInterrupts(void); // 0x803d70a8
int OSEnableInterrupts(void); // 0x803d70bc
int OSRestoreInterrupts(int enable); // 0x803d70d0
void OSClearContext(OSContext *ctx); // 0x803d4768
int DVDConvertPathToEntrynum(char *file); // 0x803c4ed4
int DVDFastOpen(s32 entrynum, DVDFileInfo *dvdFileInfo); // 0x803c51c8
int DVDClose(DVDFileInfo *dvdFileInfo); // 0x803c5304
// addr must be 32-byte aligned, length a multiple of 32, offset an absolute disc
// byte offset that is a multiple of 2048; prio 1 is lowest, 2+ is higher.
s32 DVDReadAbsAsyncPrio(DVDCommandBlock *block, void *addr, s32 length, s32 offset, DVDReadCallback callback, s32 prio); // 0x803c7364
int File_Read(int entrynum, int file_offset, void *buffer, int read_size, int flags, int unk_index, FileReadCallback cb, void *cb_arg2); // 0x80446acc, just use 0x21 for flags if dram, 0x23 if aram, 1 for unk_index
// Completion callback for File_Read: sets *stc_file_read_done to 1, asserting first
// that the read reported no error. Clear the flag before the read and spin on
// File_Wait afterwards to turn File_Read into a blocking one - buffer and read_size
// must be 32-byte aligned, file_offset a multiple of 32.
void File_ReadDone(int r3, void *arg); // 0x80058e1c
// Pumps the file task queue (DoTasks) and returns *stc_file_read_done, so a
// `while (File_Wait() == 0);` loop both drives and waits for an outstanding read.
int File_Wait(); // 0x80058e60
static int *stc_file_read_done = (int *)0x805dd5a8;
int File_CopyFromARAMToDRAM(int entrynum, int file_offset, void *buffer, int read_size, int flags, int unk_index);                                          // 0x80059228, just use 0x21 for flags if dram, 0x23 if aram, 1 for unk_index
// Reads the whole file into buffer, writes size to *out_size. ".dat" is appended ONLY when
// file_name has no "_" or "." - a name containing either is taken verbatim (pass its full
// "name.dat"). Panics (assert) if the resolved file does not exist, so gate with a presence
// check first. buffer must be 32-byte aligned.
int File_LoadSync(char *file_name, void *buffer, int *out_size); // 0x80059364
// Loads file_name (extension appended) and Archive_Init's it. The returned archive's
// storage is NOT safe to cache across scene transitions: it comes from a reclaimable
// per-scene heap and is overwritten once another scene loads its archives (the cached
// descriptors then dangle). Reload per scene-context, or for a persistent asset read
// the file into your own buffer (File_LoadSync) and Archive_Init it in place.
int Gm_LoadGameFile(HSD_Archive **out, char *file_name); // 0x80059818
// The matching free. Asserts the archive is non-NULL and carries
// HSD_ARCHIVE_DONT_FREE, then releases the file blob and the archive struct.
void Gm_FreeGameFile(HSD_Archive *archive); // 0x80059798
// Same ".dat"-append rule as File_LoadSync, and likewise panics if the file is missing.
int File_GetSize(char *file_name); // 0x8005915c
// void memcpy(void *dest, void *source, int size);
// void memset(void *dest, int fill, int size);
s32 CARDGetStatus(s32 chan, s32 fileNo, CARDStat *stat); // 0x803e8240
s32 CARDMount(s32 chan, void *workArea, void *detachCallback); // 0x803e64ec
s32 CARDMountAsync(s32 chan, void *workArea, void *detachCallback, void *attachCallback); // 0x803e634c
s32 CARDUnmount(s32 chan); // 0x803e65d0
s32 CARDOpen(s32 chan, char *fileName, CARDFileInfo *fileInfo); // 0x803e70ec
s32 __CARDSync(s32 chan); // 0x803e3024
s32 CARDClose(CARDFileInfo *fileInfo); // 0x803e7264
s32 CARDProbeEx(s32 chan, s32 *memSize, s32 *sectorSize); // 0x803e5c88
s32 CARDCheckExAsync(s32 chan, s32 *xferBytes, void *callback); // 0x803e55d8
s32 CARDFreeBlocks(s32 chan, s32 *byteNotUsed, s32 *filesNotUsed); // 0x803e2ed4
s32 CARDDeleteAsync(s32 chan, char *fileName, void *callback); // 0x803e7ef0
s32 CARDDelete(s32 chan, char *fileName); // 0x803e8000, sync wrapper around CARDDeleteAsync + __CARDSync
s32 CARDCreate(s32 chan, char *fileName, u32 size, CARDFileInfo *fileInfo); // 0x803e7610
s32 CARDCreateAsync(s32 chan, char *fileName, u32 size, CARDFileInfo *fileInfo, void *callback); // 0x803e73f0
s32 CARDSetStatusAsync(s32 chan, s32 fileNo, CARDStat *stat, void *callback); // 0x803e836c
s32 CARDSetStatus(s32 chan, s32 fileNo, CARDStat *stat); // 0x803e84e0, sync wrapper around CARDSetStatusAsync + __CARDSync
s32 CARDRead(CARDFileInfo *fileInfo, void *buf, s32 length, s32 offset); // 0x803e7a88
s32 CARDReadAsync(CARDFileInfo *fileInfo, void *buf, s32 length, s32 offset, void *callback); // 0x803e7940
s32 CARDWrite(CARDFileInfo *fileInfo, void *buf, s32 length, s32 offset); // 0x803e7e04
s32 CARDWriteAsync(CARDFileInfo *fileInfo, void *buf, s32 length, s32 offset, void *callback); // 0x803e7cf0
s32 CARDGetXferredBytes(s32 chan); // 0x803126b4
u32 PADRead(PADStatus *status); // 0x803dc8e8
u32 PADReset(u32 mask); // 0x803dc4cc, use PAD_CHANX_BIT
u32 SISetXY(u16 line, u8 cnt); // 0x803e93c0, returns the new XY register value
void SISetSamplingRate(int msec); // 0x803e9e5c
u32 SIEnablePolling(int mask); // 0x803e942c, returns the resulting poll register value
int SIIsChanBusy(s32 chan); // 0x803e878c
int SIGetStatus(s32 chan); // 0x803e9320
int SIGetType(s32 chan); // 0x803e9b5c
int SIGetResponse(s32 chan, void *out); // 0x803e9608
void DCFlushRange(void *startAddr, u32 nBytes); // 0x803d3f24
void DCInvalidateRange(void *startAddr, u32 nBytes); // 0x803d3ef8
void DCStoreRange(void *startAddr, u32 nBytes); // 0x803d3f54
void TRK_FlushCache(void *startAddr, u32 nBytes); // 0x803bffd8
// int memcmp(void *buf1, void *buf2, u32 nBytes);

/** String Library **/
// #define vsprintf(buffer, format, args) _vsprintf(buffer, -1, format, args)
// int sprintf(char *s, const char *format, ...);
// int _vsprintf(char *str, int unk, const char *format, va_list arg);
// int strlen(char *str);
// char *strchr(char *str, char c); // searches for the first occurrence of the character c (an unsigned char) in the string pointed to by the argument str.
// int strcmp(char *str1, char *str2);
// int strncmp(char *str1, char *str2, int size);
// char *strcpy(char *dest, char *src);            // copies the string pointed to, by src to dest.
// char *strncpy(char *dest, char *src, int size); // copies the string pointed to, by src to dest.
// int tolower(char in);

/** Math **/
/*** Functions ***/
// float fmod(float a, float b);
// float atan(float in);
// float atan2(float y, float x);
// float cos(float x);
void MTXOrtho(Mtx44 m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f); // 0x803d2000
void MTXLookAt(Mtx *dest, Vec3 *eye, Vec3 *up, Vec3 *target); // 0x803d1a78
void MTXRotAxisRad(Mtx *m, Vec3 *axis, float rad); // 0x803d1908 - Sets a matrix for rotation about an arbitrary axis whose ( x, y, z ) components are specified by axis.
void MTXMultVec(Mtx *m, Vec3 *src, Vec3 *dst);     // 0x803d1dec - Post-multiplies a 3D vector or 3D point by a matrix ( m x src = dst ).
void MTXLightPerspective(Mtx *m, float fovy, float aspect, float scaleS, float scaleT, float transS, float transT); // 0x803d1c98
void VECNormalize(Vec3 *src, Vec3 *dest); // 0x803d20fc
void VECScale(Vec3 *src, Vec3 *dst, float scale); // 0x803d20e0
void VECAdd(Vec3 *a, Vec3 *b, Vec3 *ab); // 0x803d2098
void VECSubtract(Vec3 *a, Vec3 *b, Vec3 *a_b); // 0x803d20bc
void PSMTXIdentity(Mtx *dest); // 0x803d13fc
void PSMTXCopy(Mtx *src, Mtx *dest); // 0x803d1428
void PSMTXConcat(Mtx *a, Mtx *b, Mtx *ab); // 0x803d145c
float VECDotProduct(Vec3 *a, Vec3 *b); // 0x803d219c
void VECCrossProduct(Vec3 *a, Vec3 *b, Vec3 *axb); // 0x803d21bc
// void MTXQuat(Mtx *m, Vec4 *dest);           // quat to matrix
void HSD_MkRotationMtx(Mtx *dest, Vec4 *v); // 0x80416ffc, quat to rot matrix
void HSD_MtxGetScale(Mtx *m, Vec3 *dest); // 0x80416b24
void HSD_MtxGetRotation(Mtx *m, Vec3 *dest); // 0x80416048
void HSD_MtxGetTranslate(Mtx *m, Vec3 *dest); // 0x80416b08
void HSD_MtxSRT(Mtx *m, Vec3 *scale, Vec3 *rot, Vec3 *trans, int unk); // 0x80417580
float sqrtf(float num); // 0x803bd490
void MTXRotRad(Mtx m, char axis, f32 rad); // 0x803d1738
void Vec3_RotateAboutUnitAxis(Vec3 *v, Vec3 *axis, float angle); // 0x800638f8

/* GameCube SDK math library */
float VECMag(Vec3 *v);                          // 0x803d2158, |v|
float VECSquareMag(Vec3 *v);                    // 0x803d2140, |v|^2 (no sqrt)
float VECDistance(Vec3 *a, Vec3 *b);            // 0x803d22f4, |a-b|
float VECSquareDistance(Vec3 *a, Vec3 *b);      // 0x803d22cc, |a-b|^2 (no sqrt)
void VECHalfAngle(Vec3 *a, Vec3 *b, Vec3 *half); // 0x803d21f8
void MTXScale(Mtx m, f32 xS, f32 yS, f32 zS);   // 0x803d19ac, build a scale matrix
void MTXTrans(Mtx m, f32 xT, f32 yT, f32 zT);   // 0x803d1978, build a translation matrix
void MTXTranspose(Mtx src, Mtx xPose);          // 0x803d1528
u32 MTXInverse(Mtx src, Mtx inv);               // 0x803d1578, 0 if singular
u32 MTXInvXpose(Mtx src, Mtx invX);             // 0x803d1670, inverse-transpose (normal matrix)
void MTXRotTrig(Mtx m, char axis, f32 sinA, f32 cosA); // 0x803d17a8
void MTXMultVecSR(Mtx m, Vec3 *src, Vec3 *dst); // 0x803d1e40, 3x3 part only (no translation)
void MTXFrustum(Mtx44 m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f); // 0x803d1e94
void MTXPerspective(Mtx44 m, f32 fovY, f32 aspect, f32 n, f32 f); // 0x803d1f30
void MTXLightFrustum(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 scaleS, f32 scaleT, f32 transS, f32 transT); // 0x803d1c04
void MTXLightOrtho(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 scaleS, f32 scaleT, f32 transS, f32 transT); // 0x803d1d64
float sinf(float x); // 0x803bd53c
float cosf(float x); // 0x803bd560
float tanf(float x); // 0x803bd518
double floor(double x); // 0x803bcda4
double pow(double x, double y); // 0x803baafc
double acos(double x); // 0x803b9a0c
double asin(double x); // 0x803b9d1c
double log10(double x);                                    // 0x803ba9ec
float VEC_NormalizeAndSnap(Vec3 *src, Vec3 *dst);          // 0x80062ca4, normalize src->dst, returns |src|
void VEC_CrossNormalizeSnap(Vec3 *a, Vec3 *b, Vec3 *axb);  // 0x80062dd0, axb = normalize(a x b)
float Vec_GetAngleBetween(Vec3 *a, Vec3 *b);               // 0x80062ecc, angle (radians) between a and b

int bp(); // 0x802d4bb0

#endif