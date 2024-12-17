/*
    声明所有数据及其宏定义


*/

#ifndef AST_OBJECT_H
#define AST_OBJECT_H
#include <stdarg.h>
#include "ast.h"
#include "astState.h"
typedef double ast_Number;
typedef unsigned char ast_Byte;
typedef unsigned int ast_Hash;
// GC数据类型联合
typedef union GCObject GCObject;
#define GCCommonHearder \
    GCObject *next;     \
    unsigned char tt;   \
    unsigned char marked
// GC数据头
typedef struct GCHeader
{
    GCCommonHearder;
} GCHeader;
// AST_STRING
typedef union ast_TString
{
    ast_UMaxAlign Dummy;
    struct
    {
        GCCommonHearder;
        ast_Byte reserved;
        ast_Hash hash;
        size_t len;
    } Tsv;
} ast_String;

typedef union
{
    GCObject *gc;
    void *p;
    ast_Number n;
    int b;

} Value;
// 判断数据对象是否可回收
#define IsCollectable(o) (ttype(o) >= AST_TSTRING)

// 对common数据的封装
#define TValueFields \
    Value value;     \
    int tt;

typedef struct ast_TValue
{
    TValueFields;
} TValue;

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

#define printnvalue(obj)              \
    {                                 \
        printf("%f", (obj)->value.n); \
    }
#define printbvalue(obj)              \
    {                                 \
        printf("%d", (obj)->value.b); \
    }

#endif