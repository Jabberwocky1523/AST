#ifndef AST_H
#define AST_H

#include <stdarg.h>
#include <stddef.h>
#define ttype(o) ((o)->tt)
#define AST_TNONE (-1)

#define AST_TBOOLEAN 1
#define AST_TUSERDATA 2
#define AST_TNUMBER 3
#endif