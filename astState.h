/*
声明所有后续数据结构的状态

*/
#ifndef AST_STATE_H
#define AST_STATE_H
#include "ast.h"
#include "astObject.h"
#include "astBinaryChunk.h"
#define Int2Ob(o) ast_IntegerToTValue(o)
#define Dou2Ob(o) ast_NumberToTValue(o)
#define Char2Ob(L, o) ast_ObjectToTValue(L, (void *)o, AST_TSTRING, 0)
#define Str2Ob(L, o) ast_ObjectToTValue(L, (void *)o, AST_TSTRING, 1)
#define Nil2Ob() ast_NilToTValue()
#define Bool2Ob(o) ast_BooleanToTValue(o)
#define Tb2Ob(o) ast_ObjectToTValue(nullptr, (void *)o, AST_TTABLE, 0)
#define CFunc2Ob(o) ast_CFunctionToTValue(o)
typedef size_t ast_Mem;
typedef struct StringTable
{
    GCObject **hashTable;
    ast_Uint32 Tnum;
    ast_Uint32 size;
} StringTable;

typedef struct global_State
{
    StringTable stringtable;
    ast_Mem GCthreshold;
    ast_Mem totalbytes;
    GCObject *rootgc;
    int gcmarked;
    int borrow;
    int gcnum;
} global_State;

typedef struct ast_State
{
    GCCommonHeader;
    global_State *G_S;
    ast_Stack *stack;
    TValue Registry;
} ast_State;
// 全局定义

ast_Bool ast_Init(ast_State *L, global_State *G_S);
ast_Bool ast_Load(ast_State *L, char *file_Path);
TValue ast_ObjectToTValue(ast_State *L, void *ob, ast_Type type, int flag);
TValue ast_IntegerToTValue(ast_Integer i);
TValue ast_NumberToTValue(ast_Number n);
TValue ast_BooleanToTValue(ast_Bool bo);
TValue ast_CFunctionToTValue(ast_CFunction func);
TValue ast_NilToTValue();
ast_Bool ast_PushStack(ast_State *L, ast_Stack *stack);
ast_Stack *ast_PopStack(ast_State *L);
ast_Bool ast_LoadChunk(ast_State *L, astBuffer chunk, Prototype *proto, ast_String *chunkname, int mode);
ast_Integer ast_RegCount(ast_State *L);
ast_Integer ast_LoadVararg(ast_State *L, int n);
ast_Bool ast_LoadProto(ast_State *L, int idx);
ast_Bool ast_Call(ast_State *L, int nArgs, int nResults);
ast_Bool ast_PushGlobalTable(ast_State *L);
ast_Bool ast_RegisterPushValue(ast_State *L, ast_CFunction func, TValue name);
ast_Bool ast_SetGlobal(ast_State *L, TValue name);
ast_Bool ast_GetGlobal(ast_State *L, TValue name);
TValue *ast_PopN(ast_Stack *L, int size);
ast_Bool ast_PushN(ast_Stack *L, TValue *vals, int num);
#endif
