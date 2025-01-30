#ifndef BASIC_LIB_H
#define BASIC_LIB_H
#include "../astUser.h"
#include "../astStr.h"
#include "../astTable.h"
#include "auxLib.h"
#include "../astMap.h"
#include "unordered_map"
ast_Integer Len(ast_State *L);
ast_Integer LoadFile(ast_State *L);
ast_Integer doFile(ast_State *L);
ast_Integer select(ast_State *L);
ast_Integer tostring(ast_State *L);
ast_Integer tonumber(ast_State *L);
std::unordered_map<String, ast_CFunction> func{
    {"print", astPrintTest},
    {"getmetatable", getMetatable},
    {"setmetatable", setMetatable},
    {"pairs", pairs},
    {"next", next},
    {"ipairs", ipairs},
    {"pcall", pCall},
    {"len", Len},
    {"dofile", doFile},
    {"loadfile", LoadFile},
    {"select", select},
    {"tostring", tostring},
    {"tonumber", tonumber}};

#endif