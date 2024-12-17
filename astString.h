#ifndef AST_STRING_H
#define AST_STRING_H
#include "ast.h"
#include "astObject.h"
#include "astState.h"
ast_Bool astString_Resize(int newsize);
ast_String *astString_NewLStr(ast_String *List, const char *str, size_t len);
#endif