/*
声明所有后续数据结构的状态

*/
#ifndef AST_STATE_H
#define AST_STATE_H

#include "ast.h"
#include "astObject.h"
typedef size_t ast_Mem;

typedef struct StringTable
{
    GCObject **hashTable;
    ast_Uint32 Tnum;
    ast_Uint32 size;
} StringTable;

// 全局状态机
typedef struct global_State
{
    StringTable stringtable;
    ast_Mem GCthreshold;
    ast_Mem totalbytes;
    void *StringBuff;
} global_State;
typedef struct ast_State
{
    GCCommonHeader;
    ast_Byte status;
    global_State *G_S;
} ast_State;

typedef struct ast_Stack
{
    TValue *Value;
    int top;
    int size;
} ast_Stack;

ast_Stack *ast_NewStack(int size);
ast_Bool ast_StackCheck(ast_Stack *L, int n);
ast_Bool ast_StackPush(ast_Stack *L, TValue &value);
TValue ast_StackPop(ast_Stack *L);
ast_Bool ast_StackIdxIsValid(ast_Stack *L, int idx);
TValue ast_StackGetTValue(ast_Stack *L, int idx);
ast_Bool ast_StackSetTValue(ast_Stack *L, TValue &value, int idx);
ast_Bool ast_Init(ast_State *L, global_State *G_S);
#endif
