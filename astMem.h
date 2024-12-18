#ifndef AST_MEM_H
#define AST_MEM_H
#include "ast.h"
#include "astObject.h"
#include "stdlib.h"
#include "string.h"
#include "astState.h"
#define ast_Malloc(L, S) astM_Realloc(L, NULL, 0, (S))
#define ast_Memcpy(des, src, len) memcpy((des), (src), (len))
#define ast_Memcmp(s1, s2, len) memcmp((s1), (s2), (len))
void *astM_Realloc(ast_State *L, void *block, size_t oldsize, size_t size);

#endif
