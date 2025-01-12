#ifndef AST_USER_H
#define AST_USER_H
#include "astState.h"
ast_Integer astPrintTest(ast_State *L);
ast_Integer astTest(ast_State *L);
ast_Integer getMetatable(ast_State *L);
ast_Integer setMetatable(ast_State *L);
ast_Integer pairs(ast_State *L);
ast_Integer next(ast_State *L);
ast_Integer ipairs(ast_State *L);
#endif