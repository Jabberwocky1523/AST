/*
声明所有后续数据结构的状态

*/
#ifndef AST_STATE_H
#define AST_STATE_H
#include "ast.h"
#include "astObject.h"
#include "astBinaryChunk.h"
#define Int2Ob(o) ast_ObjectToTValue(NULL, (void *)&o, AST_TINTEGER, 0)
#define Dou2Ob(o) ast_ObjectToTValue(NULL, (void *)&o, AST_TNUMBE, 0)
#define Char2Ob(L, o) ast_ObjectToTValue(L, (void *)o, AST_TSTRING, 0)
#define Str2Ob(L, o) ast_ObjectToTValue(L, (void *)o, AST_TSTRING, 1)
#define Nil2Ob() ast_ObjectToTValue(nullptr, nullptr, AST_TNIL, 0)
#define Bool2Ob(o) ast_ObjectToTValue(nullptr, (void *)&o, AST_TBOOLEAN, 0)
#define Tb2Ob(o) ast_ObjectToTValue(nullptr, (void *)o, AST_TTABLE, 0)
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
    global_State *G_S;
    ast_Stack *stack;
} ast_State;

// 全局定义

ast_Bool ast_Init(ast_State *L, global_State *G_S);
TValue ast_ObjectToTValue(ast_State *L, void *ob, ast_Type type, int flag);
ast_Bool ast_PushStack(ast_State *L, ast_Stack *stack);
ast_Stack *ast_PopStack(ast_State *L);
ast_Bool ast_LoadChunk(ast_State *L, astBuffer chunk, Prototype *proto, ast_String *chunkname, int mode);
ast_Integer ast_RegCount(ast_State *L);
ast_Bool ast_LoadVararg(ast_State *L, int n);
ast_Bool ast_LoadProto(ast_State *L, int idx);
ast_Bool ast_Call(ast_State *L, int nArgs, int nResults);
#endif
