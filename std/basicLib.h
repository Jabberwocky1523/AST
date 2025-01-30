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
<<<<<<< HEAD
ast_Integer tostring(ast_State *L);
ast_Integer tonumber(ast_State *L);
=======
>>>>>>> 8f56bee6108f7249811fab839f38dba00b875e02
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
<<<<<<< HEAD
    {"select", select},
    {"tostring", tostring},
    {"tonumber", tonumber}};
=======
    {"select", select}};
>>>>>>> 8f56bee6108f7249811fab839f38dba00b875e02

#endif