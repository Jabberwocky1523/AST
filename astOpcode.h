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
#endif