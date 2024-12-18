#ifndef AST_STRING_H
#define AST_STRING_H
#include "ast.h"
#include "astObject.h"
#include "astState.h"
#include "astMem.h"
/*
param:ts类型为 *ast_String
*/
#define getstr(ts) cast(const char *, (ts) + 1)
ast_Bool astString_Resize(int newsize);
ast_String *astString_NewLStr(ast_State *L, const char *str, size_t len);
#endif