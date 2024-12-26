#ifndef AST_VM_H
#define AST_VM_H
#include "astState.h"
#include "astInstruction.h"
ast_Integer ast_GetPc(ast_State *L);
ast_Bool ast_AddPc(ast_State *L, ast_Integer n);
ast_Integer ast_Fetch(ast_State *L);
ast_Bool ast_GetConst(ast_State *L, int idx); // 常量表中推入栈
ast_Bool ast_GetRk(ast_State *L, int rk);     // 常量表或栈推入栈
ast_Bool ast_ExecuteOp(ast_State *L, Instruction i);
#endif