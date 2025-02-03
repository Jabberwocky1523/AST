#ifndef STD_TABLE_H
#define STD_TABLE_H
#include "../astUser.h"
#include "../astStr.h"
#include "../astTable.h"
#include "auxLib.h"
#include "../astMap.h"
ast_Integer move(ast_State *L);
ast_Integer insert(ast_State *L);
ast_Integer remove(ast_State *L);
ast_Integer concat(ast_State *L);
ast_Integer pack(ast_State *L);
ast_Integer unpack(ast_State *L);
FuncRegs stdtable = {
    {"move", move},
    {"insert", insert},
    {"remove", remove},
    {"concat", concat},
    {"pack", pack},
    {"unpack", unpack}};
#endif