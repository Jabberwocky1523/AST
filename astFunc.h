#ifndef AST_FUNC_H
#define AST_FUNC_H
#include "ast.h"
#include "astState.h"
ast_Bool ast_PushCFunction(ast_State *L, ast_CFunction func);
ast_Bool ast_IsCFunction(ast_State *L, ast_Integer idx);
ast_CFunction ast_ToCFunction(ast_State *L, ast_Integer idx);
#endif