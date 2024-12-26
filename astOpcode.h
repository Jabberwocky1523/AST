#ifndef AST_OPCODE_H
#define AST_OPCODE_H
#include "astState.h"
typedef uint32_t Instruction;
enum OPARITHCODE
{
    AST_OPADD = 0, // +
    AST_OPSUB,     // -
    AST_OPMUL,     // *
    AST_OPMOD,     // %
    AST_OPPOW,     // ^
    AST_OPDIV,     // /
    AST_OPIDIV,    // //
    AST_OPBAND,    // &
    AST_OPBOR,     // |
    AST_OPBXOR,    // ~
    AST_OPSHL,     // <<
    AST_OPSHR,     // >>
    AST_OPUNM,     // -
    AST_OPBNOT,    // ~
};
enum OPCOMPARECODE
{
    AST_OPEQ = 0, // ==
    AST_OPLT,     // <
    AST_OPLE,     // <=
};

enum CodecMode
{
    IABC,  // 三操作数 8 9 9
    IABx,  // 二操作数 8 18
    IAsBx, // 二操作数 8 18 sBx-有符号整数
    IAx,   // 一操作数 26
};

enum OperatorCode // 6bit
{
    OP_MOVE = 0,
    OP_LOADK,
    OP_LOADKX,
    OP_LOADBOOL,
    OP_LOADNIL,
    OP_GETUPVAL,
    OP_GETTABUP,
    OP_GETTABLE,
    OP_SETTABUP,
    OP_SETUPVAL,
    OP_SETTABLE,
    OP_NEWTABLE,
    OP_SELF,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_MOD,
    OP_POW,
    OP_DIV,
    OP_IDIV,
    OP_BAND,
    OP_BOR,
    OP_BXOR,
    OP_SHL,
    OP_SHR,
    OP_UNM,
    OP_BNOT,
    OP_NOT,
    OP_LEN,
    OP_CONCAT,
    OP_JMP,
    OP_EQ,
    OP_LT,
    OP_LE,
    OP_TEST,
    OPTESTSET,
    OP_CALL,
    OP_TALLCALL,
    OP_RETURN,
    OP_FORLOOP,
    OP_FORPREP,
    OP_TFORCALL,
    OP_TFORLOOP,
    OP_SSETLIST,
    OP_CLOSURE,
    OP_VARARG,
    OP_EXTRAARG,
};
// 操作种类
enum OperatorNum
{
    OpArgN, // argument is not used
    OpArgU, // argument is used
    OpArgR, // iABC-寄存器索引 iAsBx-跳转偏移
    OpArgK, // 常量或者寄存器 iABx-常量表 iABC:最高位1-常量表
};
typedef struct ast_OpCode
{
    unsigned char testFlag; // operator is a test (next instruction muse be a jump)
    unsigned char setAFlag;
    unsigned char argBMode;
    unsigned char argCMode;
    unsigned char opMode;
    const char *name;
    ast_Bool (*ast_OpAction)(ast_State *L, Instruction i);
} ast_OpCode;

#endif