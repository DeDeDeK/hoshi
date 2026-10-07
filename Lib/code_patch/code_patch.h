#ifndef LIB_H_CODEPATCH
#define LIB_H_CODEPATCH

void _CodePatch_HookApply(int *dol_addr, void *hook_func);
void _CodePatch_OverwriteApply(int *dol_addr, int instr);
void _CodePatch_ReplaceCall(int *dol_addr, void *func_addr);
void _CodePatch_ReplaceFunc(int *dol_addr, void *func_addr);
void _CodePatch_ReplaceImmediate(int *dol_addr, int imm);
void _CodePatch_ReplaceAddress(int *lis_addr, int *addi_addr, const void *target);

#define PPC_NOP 0x60000000

#define BRANCH_CREATE(src, dst, is_link) (0x48000000 | (is_link ? 1 : 0) | (((int)dst - (int)src) & ~(0xfc000000)));

typedef enum HookKind
{
    HOOKKIND_NORMAL,
    HOOKKIND_CONDITIONAL,
} HookKind;

#define CODEPATCH_HOOKCREATE(_dol_addr, _prologue, _func, _epilogue, _exit_addr)                               \
    void hook_##_dol_addr()                                                                                    \
    {                                                                                                          \
                                                                                                               \
        __asm__ volatile(                                                                                      \
                                                                                                               \
            ".long %2\n\t"              /* type of hook */                                                     \
            "b exit_" #_dol_addr "\n\t" /* glorified pointer to the end of the injection (is not executed) */  \
            _prologue                   /* execute instructions passed into the macro */                       \
            "bl %0\n\t"                 /* call C function */                                                  \
            _epilogue                   /* execute instructions passed into the macro */                       \
            "exit_" #_dol_addr ":\n\t"  /* label indicating the end of the injection */                        \
            ".long 0\n\t"               /* null, replaced with the instruction we clobber to branch here */    \
            ".long %1\n\t"              /* pointer to our return location, relocated to a branch at runtime */ \
            :                                                                                                  \
            : "i"(_func), "i"((_exit_addr) ? _exit_addr : (_dol_addr + 0x4)), "i"(HOOKKIND_NORMAL));           \
    }

#define CODEPATCH_HOOKCONDITIONALCREATE(_dol_addr, _prologue, _func, _epilogue, _exit_addr, _exit_addr_alt)                                                           \
    void hook_##_dol_addr()                                                                                                                                           \
    {                                                                                                                                                                 \
                                                                                                                                                                      \
        __asm__ volatile(                                                                                                                                             \
                                                                                                                                                                      \
            ".long %2\n\t"              /* type of hook */                                                                                                            \
            "b exit_" #_dol_addr "\n\t" /* glorified pointer to the end of the injection (is not executed) */                                                         \
            _prologue                   /* execute instructions passed into the macro */                                                                              \
            "bl %0\n\t"                 /* call C function */                                                                                                         \
            "cmpwi 3, 0\n\t"            /* check return value of our function */                                                                                      \
            _epilogue                   /* execute instructions passed into the macro */                                                                              \
            "exit_" #_dol_addr ":\n\t"  /* label indicating the end of the injection */                                                                               \
            "bne 0xc\n\t"               /* if func returns 1, exit to alt addr */                                                                                     \
            ".long 0\n\t"               /* null, replaced with the instruction we clobber to branch here */                                                           \
            ".long %1\n\t"              /* pointer to our return location, relocated to a branch at runtime */                                                        \
            ".long %3\n\t"              /* pointer to our alternate return location, relocated to a branch at runtime */                                              \
            :                                                                                                                                                         \
            : "i"(_func), "i"((_exit_addr) ? _exit_addr : (_dol_addr + 0x4)), "i"(HOOKKIND_CONDITIONAL), "i"((_exit_addr_alt) ? _exit_addr_alt : (_dol_addr + 0x4))); \
    }

#define CODEPATCH_HOOKAPPLY(_dol_addr) _CodePatch_HookApply((int *)(_dol_addr), hook_##_dol_addr)
#define CODEPATCH_REPLACEINSTRUCTION(_dol_addr, _instr) _CodePatch_OverwriteApply((int *)(_dol_addr), (_instr))
#define CODEPATCH_REPLACECALL(_dol_addr, _func) _CodePatch_ReplaceCall((int *)(_dol_addr), (void *)(_func))
#define CODEPATCH_REPLACEFUNC(_dol_addr, _func) _CodePatch_ReplaceFunc((int *)(_dol_addr), (void *)(_func))
// Rewrites the low 16 bits of the instruction, keeping its opcode and registers.
#define CODEPATCH_REPLACEIMMEDIATE(_dol_addr, _imm) _CodePatch_ReplaceImmediate((int *)(_dol_addr), (_imm))
// Points the lis / addi pair that forms an address at _target instead.
#define CODEPATCH_REPLACEADDRESS(_lis_addr, _addi_addr, _target) _CodePatch_ReplaceAddress((int *)(_lis_addr), (int *)(_addi_addr), (_target))

typedef enum CodePatchKind
{
    CODEPATCHKIND_HOOK,
    CODEPATCHKIND_REPLACEINSTR,
    CODEPATCHKIND_REPLACECALL,
    CODEPATCHKIND_REPLACEFUNC,
} CodePatchKind;

typedef struct PatchDesc
{
    CodePatchKind patch_kind : 4;
    int addr : 28;
    union
    {
        void *func_ptr;
        int instruction;
    };
} PatchDesc;

#define CODEPATCHDESC_HOOK(_dol_addr) {.patch_kind = CODEPATCHKIND_HOOK, .addr = (0x80000000 | (int)_dol_addr), .func_ptr = hook_##_dol_addr}

#endif