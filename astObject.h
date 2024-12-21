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

typedef union GCObject
{
    GCHeader gch;
    ast_String ts;
} GCObject;
// 判断数据对象是否可回收
#define IsCollectable(o) (ttype(o) >= AST_TSTRING)

#define ast_CopyTValue(obj1, obj2)     \
    {                                  \
        (obj1)->value = (obj2)->value; \
        (obj1)->tt = (obj2)->tt;       \
    }

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

#endif