/*
    声明所有数据及其宏定义


*/

#ifndef AST_OBJECT_H
#define AST_OBJECT_H
#include <stdarg.h>
#include "astBinaryChunk.h"
#include "ast.h"
#include "astVector.h"
#define cast(t, exp) ((t)(exp)) // 类型转换
#define lmod(s, size) cast(ast_Hash, (s) & ((size) - 1))
typedef double ast_Number;
typedef unsigned char ast_Byte;
typedef unsigned int ast_Hash;
typedef long long ast_Integer;
typedef struct ast_State ast_State;
typedef ast_Integer (*ast_CFunction)(ast_State *L);
// GC数据类型联合
typedef union GCObject GCObject;
typedef union StatObject StatObject;
typedef struct ast_Stat ast_Stat;
typedef union ExpObject ExpObject;
typedef struct ast_Exp
{
    ast_Integer tt;
    ExpObject *exp;
} ast_Exp;
typedef struct ast_Stat
{
    ast_Integer tt;
    StatObject *stat;
} ast_Stat;
typedef struct ast_Block
{
    ast_Integer LastLine;
    Vector<ast_Stat> stats;
    Vector<ast_Exp> exps;
} ast_Block;
#define GCCommonHeader \
    GCObject *next;    \
    unsigned char tt;  \
    unsigned char marked
// GC数据头
typedef struct GCHeader
{
    GCCommonHeader;
} GCHeader;
// AST_STRING
typedef union
{
    union GCObject *gc;
    void *p;
    ast_Number n;
    ast_Integer i;
    ast_Bool bo;
    int b;
} Value;

#define TValueFields \
    Value value;     \
    int tt;

typedef struct ast_TValue
{
    TValueFields;
} TValue;
typedef union ast_TString
{
    ast_UMaxAlign Dummy;
    struct
    {
        GCCommonHeader;
        ast_Byte reserved;
        ast_Hash hash;
        size_t len;
    } Tsv;
} ast_String;
typedef struct ast_Lexer
{
    ast_State *L;
    char *chunk;
    char *chunkName;
    ast_Integer Line;
    char *curchunk;
    ast_Integer chunkSize;
} ast_Lexer;
// 哈希表
typedef struct ast_MapNode
{
    TValue key;
    TValue val;
    struct ast_MapNode *next;
} ast_MapNode;

typedef struct ast_Map
{
    ast_MapNode **map;
    ast_Integer size;
    ast_Integer Mnum;
} ast_Map;
typedef struct ast_Table
{
    GCCommonHeader;
    ast_Map *HashMap;
    ast_Map *IteratorMap;
    TValue *arr;
    ast_Table *MetaTable;
    ast_Integer arrSize;
    ast_Integer arrtop;
} ast_Table;
#define TableArrLen(o) ((o)->arrtop)
typedef struct ast_Closure
{
    union
    {
        ast_UMaxAlign align;
        Prototype *pr;
        ast_CFunction func;
    };
    ast_Integer Uvslen;
    TValue *Upvalues;
} ast_Closure;
typedef union GCObject
{
    GCHeader gch;
    ast_String ts;
    ast_Table tb;
    ast_Closure cl;
} GCObject;
// 栈作为虚拟寄存器
typedef struct ast_Token
{
    ast_Integer line;
    ast_Integer kind;
    char *token;
    ast_Integer size;
} ast_Token;
typedef struct ast_Stack
{
    TValue *Value;
    int top;
    int size;
    int pc;
    ast_Closure *closure;
    ast_Map *openuvs;
    TValue *varargs;
    int varArgsLen = 0;
    int PrevIdx = 0;
    int nPrevFuncResults = 0;
    ast_Stack *prev;
    ast_State *L;
} ast_Stack;
// 判断数据对象是否可回收
#define IsCollectable(o) (ttype(o) >= AST_TSTRING)
#define IsNum(o) ((o.tt == AST_TINTEGER || o.tt == AST_TNUMBER))
// 对common数据的封装

#define setnvalue(obj, x)        \
    {                            \
        (obj)->value.n = (x);    \
        (obj)->tt = AST_TNUMBER; \
    }

#define setbvalue(obj, x)         \
    {                             \
        (obj)->value.b = x;       \
        (obj)->tt = AST_TBOOLEAN; \
    }
ast_Bool ast_TValueCmp(TValue val1, TValue val2);
ast_Integer ast_GetTValueHash(TValue val1);
ast_Bool ast_FreeTvaluePoint(TValue *t);
TValue *ast_NewClosure(int size);
#endif