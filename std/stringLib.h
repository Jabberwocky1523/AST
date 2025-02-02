#ifndef STD_STRING_H
#define STD_STRING_H
#include "../astUser.h"
#include "../astStr.h"
#include "../astTable.h"
#include "auxLib.h"
#include "../astMap.h"
#include "../astVector.h"
#include "unordered_map"
ast_Integer len(ast_State *L);
ast_Integer sub(ast_State *L);
ast_Integer reverse(ast_State *L);
ast_Integer lower(ast_State *L);
ast_Integer upper(ast_State *L);
ast_Integer format(ast_State *L);
ast_Integer find(ast_State *L);
ast_Integer gsub(ast_State *L);
FuncRegs stdstr = {{"len", len},
                   {"sub", sub},
                   {"reverse", reverse},
                   {"lower", lower},
                   {"upper", upper},
                   {"format", format},
                   {"find", find},
                   {"gsub", gsub}};
#endif