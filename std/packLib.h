#ifndef STD_PACK_LIB
#define STD_PACK_LIB
#include "../astUser.h"
#include "../astStr.h"
#include "../astTable.h"
#include "auxLib.h"
#include "../astMap.h"
ast_Integer preloadSearcher(ast_State *L);
ast_Integer require(ast_State *L);
ast_Integer astSearcher(ast_State *L);
ast_Integer searchPath(ast_State *L);
FuncRegs stdpack = {{"searchpath", searchPath}};
#endif