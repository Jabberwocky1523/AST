#ifndef _AST_ERROR_H
#define _AST_ERROR_H
#define AST_OK 0
#define AST_YIELD 1
#define AST_ERRRUN 2
#define AST_ERRSYNTAX 3
#define AST_ERRMEM 4
#define AST_ERRGCMM 5
#define AST_ERRERR 6
#define AST_ERRFILE 7
#include "astState.h"
ast_Integer ast_PCall(ast_State *L,ast_Integer nArgs, ast_Integer nResults);
#endif