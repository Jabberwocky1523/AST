#ifndef AST_TABLE_H
#define AST_TABLE_H
#include "astObject.h"
ast_Table *astTable_Init(ast_Integer arrSize, ast_Integer MapSize);
TValue astTable_GetVal(ast_Table *tb, TValue key);
TValue astTable_PutVal(ast_Table *tb, TValue key, TValue val);
ast_Bool _astTable_ShrinkTableNil(ast_Table *tb);
#endif