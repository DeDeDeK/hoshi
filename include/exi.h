#ifndef MEX_H_EXI
#define MEX_H_EXI

#include "datatypes.h"

#define EXI_CHANNEL_0 0
#define EXI_CHANNEL_1 1
#define EXI_CHANNEL_2 2

typedef enum
{
    EXI_READ,
    EXI_WRITE,
} EXIMode;

typedef enum
{
    EXI_BUSY      = -2,
    EXI_ERROR     = -1,
    EXI_OK        = 0,
} EXIStatus;

typedef struct 
{
    u32 CR;   // Control register
    u32 BR;   // Bus register
    u32 DR;   // Data register
    u32 ESR;  // Status register
} EXIChannelReg;

typedef void (*EXICallback)(s32 chan, OSContext* context);

// volatile EXIChannelReg *exi_reg = (EXIChannelReg *)0xCC006800;

s32 EXIInit(void); // 0x803eb1ac - Initialization
s32 EXISelect(s32 chan, s32 device, s32 frequency); // 0x803eabc0 - Select a device on an EXI chan
s32 EXIDeselect(s32 chan); // 0x803eacec - Deselect the currently selected device
s32 EXILock(s32 chan, s32 lockTimeout); // 0x803eb380 - Lock access to an EXI channel
s32 EXIUnlock(s32 chan); // 0x803eb474 - Unlock access to an EXI channel
s32 EXIDma(s32 chan, void *buffer, s32 length, EXIMode direction, EXICallback cb); // 0x803ea354 - Start a DMA transfer
s32 EXIImm(s32 chan, void *buffer, s32 length, EXIMode direction, EXICallback cb); // 0x803ea058 - Immediate transfer (no DMA)
s32 EXIImmEx(s32 chan, void *buffer, s32 length, EXIMode direction); // 0x803ea2b4 - Immediate transfer (no DMA)
s32 EXISync(s32 chan); // 0x803ea440 - Wait for the EXI transfer to complete
s32 EXIProbe(s32 chan); // 0x803ea8c4 - Probe whether a device is present on an EXI chan
s32 EXIGetID(s32 chan, s32 device, u32 *out_id); // 0x803eb590
s32 EXIGetType(s32 chan, s32 device, u32 *type); // 0x803eb940

#endif