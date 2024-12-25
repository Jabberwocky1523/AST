#ifndef AST_OPCODE_H
#define AST_OPCODE_H
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

enum OperatorNum
{
    OpArgN, // argument is not used
    OpArgU, // argument is used
    OpArgR, // iABC-寄存器索引 iAsBx-跳转偏移
    OpArgK, // 常量或者寄存器 iABx-常量表 iABC:最高位1-常量表
};

typedef struct
{
    unsigned char testFlag; // operator is a test (next instruction muse be a jump)
    unsigned char setAFlag; // instruction set register A
    unsigned char argBMode; // B arg mode
    unsigned char argCMode; // C arg mode
    unsigned char opMode;   // op mode
    const char *name;
} OpCode;

static OpCode g_opcodes[47] = {
    /*T  A     B       C    mode    name */
    {0, 1, OpArgR, OpArgN, IABC, "MOVE    "},
    {0, 1, OpArgK, OpArgN, IABx, "LOADK   "},
    {0, 1, OpArgN, OpArgN, IABx, "LOADKX  "},
    {0, 1, OpArgU, OpArgU, IABC, "LOADBOOL"},
    {0, 1, OpArgU, OpArgN, IABC, "LOADNIL "},
    {0, 1, OpArgU, OpArgN, IABC, "GETUPVAL"},
    {0, 1, OpArgU, OpArgK, IABC, "GETTABUP"},
    {0, 1, OpArgR, OpArgK, IABC, "GETTABLE"},
    {0, 0, OpArgK, OpArgK, IABC, "SETTABUP"},
    {0, 0, OpArgU, OpArgN, IABC, "SETUPVAL"},
    {0, 0, OpArgK, OpArgK, IABC, "SETTABLE"},
    {0, 1, OpArgU, OpArgU, IABC, "NEWTABLE"},
    {0, 1, OpArgR, OpArgK, IABC, "SELF    "},
    {0, 1, OpArgK, OpArgK, IABC, "ADD     "},
    {0, 1, OpArgK, OpArgK, IABC, "SUB     "},
    {0, 1, OpArgK, OpArgK, IABC, "MUL     "},
    {0, 1, OpArgK, OpArgK, IABC, "MOD     "},
    {0, 1, OpArgK, OpArgK, IABC, "POW     "},
    {0, 1, OpArgK, OpArgK, IABC, "DIV     "},
    {0, 1, OpArgK, OpArgK, IABC, "IDIV    "},
    {0, 1, OpArgK, OpArgK, IABC, "BAND    "},
    {0, 1, OpArgK, OpArgK, IABC, "BOR     "},
    {0, 1, OpArgK, OpArgK, IABC, "BXOR    "},
    {0, 1, OpArgK, OpArgK, IABC, "SHL     "},
    {0, 1, OpArgK, OpArgK, IABC, "SHR     "},
    {0, 1, OpArgR, OpArgN, IABC, "UNM     "},
    {0, 1, OpArgR, OpArgN, IABC, "BNOT    "},
    {0, 1, OpArgR, OpArgN, IABC, "NOT     "},
    {0, 1, OpArgR, OpArgN, IABC, "LEN     "},
    {0, 1, OpArgR, OpArgR, IABC, "CONCAT  "},
    {0, 0, OpArgR, OpArgN, IAsBx, "JMP     "},
    {1, 0, OpArgK, OpArgK, IABC, "EQ      "},
    {1, 0, OpArgK, OpArgK, IABC, "LT      "},
    {1, 0, OpArgK, OpArgK, IABC, "LE      "},
    {1, 0, OpArgN, OpArgU, IABC, "TEST    "},
    {1, 1, OpArgR, OpArgU, IABC, "TESTSET "},
    {0, 1, OpArgU, OpArgU, IABC, "CALL    "},
    {0, 1, OpArgU, OpArgU, IABC, "TAILCALL"},
    {0, 0, OpArgU, OpArgN, IABC, "RETURN  "},
    {0, 1, OpArgR, OpArgN, IAsBx, "FORLOOP "},
    {0, 1, OpArgR, OpArgN, IAsBx, "FORPREP "},
    {0, 0, OpArgN, OpArgU, IABC, "TFORCALL"},
    {0, 1, OpArgR, OpArgN, IAsBx, "TFORLOOP"},
    {0, 0, OpArgU, OpArgU, IABC, "SETLIST "},
    {0, 1, OpArgU, OpArgN, IABx, "CLOSURE "},
    {0, 1, OpArgU, OpArgN, IABC, "VARARG  "},
    {0, 0, OpArgU, OpArgU, IAx, "EXTRAARG"},
};

#endif