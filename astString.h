#ifndef AST_STRING_H
#define AST_STRING_H
#include "ast.h"
#include "astObject.h"
#include "astState.h"
/*
param:ts类型为 *ast_String
*/
#define getstr(ts) cast(const char *, (ts) + 1)
ast_Bool astString_Resize(ast_State *L, int newsize);
ast_Bool ast_IsString(TValue t);
ast_String *astString_NewLStr(ast_State *L, const char *str, size_t len);
ast_String *astString_NewLStr(ast_State *L, const char *str, size_t len, int marked);
ast_String *astString_FindStr(ast_State *L, const char *str);
ast_Bool astString_RemoveStr(ast_State *L, const char *str);
ast_Bool astString_Remove(ast_State *L, const ast_String *str);
#endif