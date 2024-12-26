/*
    声明所有数据及其宏定义


*/

#ifndef AST_OBJECT_H
#define AST_OBJECT_H
#include <stdarg.h>
#include "ast.h"
#define cast(t, exp) ((t)(exp)) // 类型转换
#define lmod(s, size) cast(int, (s) & ((size) - 1))
typedef double ast_Number;
typedef unsigned char ast_Byte;
typedef unsigned int ast_Hash;
typedef long long ast_Integer;
// GC数据类型联合
typedef union GCObject GCObject;
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
    TValue *arr;
} ast_Table;

typedef union GCObject
{
    GCHeader gch;
    ast_String ts;
    ast_Table tb;
} GCObject;
// 栈作为虚拟寄存器
typedef struct ast_Stack
{
    TValue *Value;
    int top;
    int size;
} ast_Stack;
// 判断数据对象是否可回收
#define IsCollectable(o) (ttype(o) >= AST_TSTRING)

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
#endif