#ifndef AST_TABLE_H
#define AST_TABLE_H
#include "astState.h"
ast_Table *astTable_Init(ast_Integer arrSize, ast_Integer MapSize);
TValue astTable_GetVal(ast_Table *tb, TValue key);
ast_Bool astTable_PushVal(ast_Table *tb, TValue key, TValue val);

ast_Bool ast_CreateTableAndPush(ast_State *L, ast_Integer arrSize, ast_Integer mapsize);

ast_Bool ast_NewTable(ast_State *L);
// 从idx获取表，根据栈顶值从表中取值，然后将值push到栈顶
ast_Type ast_GetTableFromIdx(ast_State *L, ast_Integer idx);
// 与ast_GetTableFromIdx类似，值变为自定义str(用于HashMap)
ast_Type ast_GetTableFromString(ast_State *L, ast_Integer idx, TValue str);
// 与ast_GetTableFromIdx类似，值变为自定义数值（用于Arr）
ast_Type ast_GetTableFromNum(ast_State *L, ast_Integer idx, TValue num);
// 栈顶弹出两个值分别为val,key(先v后k) 然后传给idx位置的表
ast_Bool ast_SetTableFromIdx(ast_State *L, ast_Integer idx);
// idx确定表位置，val由stackpop,指定str类型的key
ast_Bool ast_SetTableFromString(ast_State *L, ast_Integer idx, TValue StrKey);
// idx确定表位置，val由stackpop,指定num类型的key
ast_Bool ast_SetTableFromNum(ast_State *L, ast_Integer idx, TValue NumKey);
ast_Bool ast_PrintTable(ast_Table tb);
ast_Bool ast_FreeTable(ast_Table *tb);
// MetaTable
ast_Bool ast_SetMetaTable(ast_State *L, TValue mt, TValue val);
TValue ast_GetMetaTable(ast_State *L, TValue val);
TValue ast_GetMetaField(ast_State *L, TValue val, TValue str);
ast_Bool ast_GetMetaTableFromIdx(ast_State *L, ast_Integer idx);
ast_Bool ast_SetMetaTableFromIdx(ast_State *L, ast_Integer idx);

#endif