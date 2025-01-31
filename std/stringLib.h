#ifndef STD_STRING_H
#define STD_STRING_H
#include "../astUser.h"
#include "../astStr.h"
#include "../astTable.h"
#include "auxLib.h"
#include "../astMap.h"
#include "unordered_map"
ast_Integer len(ast_State *L);
ast_Integer sub(ast_State *L);
FuncRegs stdstr = {{"len", len},
                   {"sub", sub}};
#endif