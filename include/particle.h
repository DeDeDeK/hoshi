#ifndef KAR_H_PARTICLE
#define KAR_H_PARTICLE

#include "structs.h"
#include "datatypes.h"

// The on-disc side of HSD point particles: the three publics a bank exports and
// the bytecode its generators run. No struct is declared for a 148-byte pool
// node, because a particle and a generator alias its offsets differently.

// One texture group inside a bank's <name>_texg symbol - a flipbook a program
// steps the particle's image index (Particle+0x0a) through, all of one format
// and size. images is 0xFFFFFFFF-terminated; for a CI format the image_count
// entries past the terminator are the TLUTs.
struct TexGDesc
{
    u32   image_count; // 0x00
    u32   format;      // 0x04 GX texture format
    u32   x08;         // 0x08
    u32   width;       // 0x0c
    u32   height;      // 0x10
    u32   x14;         // 0x14 its low halfword gates the CI path in psRenderParticles
    void *images[];    // 0x18
};

// The <name>_texg symbol itself: a count and that many TexGDesc pointers,
// terminated by 0xFFFFFFFF. A generator names one by index.
struct TexGBank
{
    u32        count;       // 0x00
    TexGDesc  *graphics[];  // 0x04
};

// The <name>_ptcl symbol: a bank's generator templates. Effect_InitBankGroup
// (0x802350a0) rejects a version below 0x43. Each offset is relative to the
// group's own base and the array is 0xFFFFFFFF-terminated.
struct ParticleGroup
{
    u16 version;    // 0x00
    u16 x02;        // 0x02
    u32 x04;        // 0x04
    u32 count;      // 0x08 generators
    u32 offsets[];  // 0x0c
};

// One generator template. Ptcl_Alloc (0x8043294c) copies the fixed fields into a
// node; everything from +0x3c on is the bytecode program below. The gravity and
// velocity fields are each one vector despite being split here.
struct PtclDesc
{
    u16 flags2;     // 0x00 low nibble is the emit shape
    u16 texgraphic; // 0x02 index into the bank's TexGBank
    u16 x04;        // 0x04 -> node +0x14
    u16 x06;        // 0x06 -> node +0x1e
    u32 flags;      // 0x08 bit 0x100 picks how `rate` seeds the emit accumulator
    f32 grav[2];    // 0x0c x and y of the gravity vector, whose z is at 0x2c
    f32 vel[3];     // 0x14
    f32 x20;        // 0x20 -> node +0x48
    f32 spread;     // 0x24 emission cone half-angle in radians, ramped toward `x30`
                    //      by Ptcl_TickOne, so a burst can open or close as it runs
    f32 rate;       // 0x28 emission rate; seeds the fractional accumulator the
                    //      generator spends down to decide when to spawn
    f32 grav_z;     // 0x2c third component of the vector at 0x0c
    f32 x30;        // 0x30 the value `spread` ramps to; also read by the emit-shape
    f32 x34;        // 0x34 switch, which decides what this pair means
    f32 x38;        // 0x38
    u8  program[];  // 0x3c
};

// Program opcodes, stepped once per frame per particle by Ptcl_TickOne
// (0x8042cce8) when the wait counter at Particle+0x1a reaches zero. A byte below
// 0x80 is a wait; everything above is dispatched through the 128-entry table at
// 0x80504fe8, indexed by `opcode - 0x80` with 0xc? and 0xd? folded down.
#define PTCL_OP_WAIT_MASK  0xc0 // a byte below 0x80 with these bits clear
#define PTCL_OP_SETIMAGE   0x40 // | wait, then one byte -> Particle+0x0a
#define PTCL_OP_RANDIMAGE  0xbc // low and high byte, rolled into Particle+0x0a
#define PTCL_OP_COLOR      0xc0 // | channels, ramps the primary color (+0x12)
#define PTCL_OP_COLOR2     0xd0 // | channels, ramps the secondary color (+0x16)
#define PTCL_OP_END        0xff

// A color operand is read on the frame a particle spawns, so overwriting one in a
// loaded descriptor recolors only the particles born after that.
const u8 *Ptcl_ProgReadVarU16(const u8 *prog, u16 *out); // 0x8042bc10
const u8 *Ptcl_ProgReadF32(const u8 *prog, f32 *out);    // 0x8042bbd8

// The two tables psInitDataBanks (0x8042a734) fills, both indexed by bank: a
// generator is psGeneratorDesc[bank][id]. Both are rebuilt on every scene load
// that installs banks, so re-resolve a descriptor rather than caching it. Which
// archive lands in which bank is a literal at the install site.
static u32 *const psGeneratorCount = (u32 *)0x8058C608;
static u8 ***const psGeneratorDesc = (u8 ***)0x8058C708;

#endif
