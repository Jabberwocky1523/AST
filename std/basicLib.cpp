#include "basicLib.h"
#include "../astFunc.h"
ast_Integer OpenBaseLibs(ast_State *L)
{
    ast_PushGlobalTable(L);
    SetFuncs(L, func);
    return 1;
}