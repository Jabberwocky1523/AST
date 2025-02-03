#ifndef STD_IO_LIB
#define STD_IO_LIB
#include "../astUser.h"
#include "../astStr.h"
#include "../astTable.h"
#include "auxLib.h"
#include "../astMap.h"
#include "fcntl.h"
ast_Integer open(ast_State *L);
ast_Integer read(ast_State *L);
FuncRegs stdio = {{"open", open},
                  {"read", read}};
#endif