#ifndef AST_MAP_H
#define AST_MAP_H
#include "astObject.h"
ast_Map *astMap_Init(ast_Integer size);

ast_Bool astMap_PushKeyVal(ast_Map *map, TValue key, TValue val);
TValue astMap_GetValFromKey(ast_Map *map, TValue key);
ast_Bool astMap_RemoveFromKey(ast_Map *map, TValue key);
ast_Bool ast_PrintMap(ast_Map *map);
ast_Bool ast_RemoveMap(ast_State *L, ast_Map *map);
ast_MapNode *astMap_FindNodeFromKey(ast_Map *map, TValue key);
#endif