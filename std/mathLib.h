#ifndef STD_MATH_LIB
#define STD_MATH_LIB
#include "../astUser.h"
#include "../astStr.h"
#include "../astTable.h"
#include "auxLib.h"
#include "../astMap.h"
ast_Integer max(ast_State *L);
FuncRegs stdmath = {{"max", max}};
#endif