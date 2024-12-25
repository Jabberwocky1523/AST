#ifndef AST_VM_H
#define AST_VM_H
#include "astOpcode.h"
#include "astState.h"
#include "astInstruction.h"
typedef struct
{
    unsigned char testFlag; // operator is a test (next instruction muse be a jump)
    unsigned char setAFlag;
    unsigned char argBMode;
    unsigned char argCMode;
    unsigned char opMode;
    const char *name;
    ast_Bool (*ast_OpAction)(ast_State *L, Instruction i);
} ast_OpCode;
ast_Integer ast_GetPc(ast_State *L);
ast_Bool ast_AddPc(ast_State *L, ast_Integer n);
ast_Integer ast_Fetch(ast_State *L);
ast_Bool ast_GetConst(ast_State *L, int idx); // 常量表中推入栈
ast_Bool ast_GetRk(ast_State *L, int rk);     // 常量表或栈推入栈
// MOVE AND JMP

ast_Bool _ast_Move(ast_State *L, Instruction i);
ast_Bool _ast_Jmp(ast_State *L, Instruction i);
// END

// LOAD
ast_Bool _ast_LoadNil(ast_State *L, Instruction i);
// END
ast_Bool ast_ExecuteOp(ast_State *L, Instruction i);
#endif