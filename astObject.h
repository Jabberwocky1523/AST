#ifndef AST_OBJECT_H
#define AST_OBJECT_H
#include <stdarg.h>
#include "ast.h"
#define ast_Number double

typedef union
{

    void *p;
    ast_Number n;
    int b;

} Value;

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