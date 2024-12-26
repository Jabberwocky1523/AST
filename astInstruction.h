#ifndef AST_INSTRUCTION_H__
#define AST_INSTRUCTION_H__
#include <inttypes.h>
#include "astOpcode.h"
#define MAXARG_Bx ((1 << 18) - 1)
#define MAXARG_sBx ((MAXARG_Bx) >> 1)
extern ast_OpCode g_ast_opcodes[47];
typedef struct
{
    int a, b, c;
} TABC;

typedef struct
{
    int a, bx;
} TABx;

typedef struct
{
    int a, sbx;
} TAsBx;
typedef struct
{
    int a;
} TAX;
inline int InstructionOpcode(Instruction i)
{
    return i & 0x3f;
}

inline TABC InstructionTABC(Instruction i)
{
    TABC ret = {
        .a = (i >> 6) & 0xff,
        .b = (i >> 23) & 0x1ff,
        .c = (i >> 14) & 0x1ff};
    return ret;
}
inline TABx InstructionTABx(Instruction i)
{
    TABx ret = {
        .a = (i >> 6) & 0xff,
        .bx = (i >> 14)};
    return ret;
}
inline TAsBx InstructionTAsBx(Instruction i)
{
    TABx res = InstructionTABx(i);
    TAsBx ret = {
        .a = res.a,
        .sbx = res.bx - MAXARG_sBx};
    return ret;
}
inline int InstructionTAX(Instruction i)
{
    TAX ret = {
        i >> 6};
    return ret.a;
}

inline const char *InstructionOpName(Instruction i)
{
    return g_ast_opcodes[InstructionOpcode(i)].name;
}

inline unsigned char InstructionOpMode(Instruction i)
{
    return g_ast_opcodes[InstructionOpcode(i)].opMode;
}

inline unsigned char InstructionBMode(Instruction i)
{
    return g_ast_opcodes[InstructionOpcode(i)].argBMode;
}

inline unsigned char InstructionCMode(Instruction i)
{
    return g_ast_opcodes[InstructionOpcode(i)].argCMode;
}

#endif
