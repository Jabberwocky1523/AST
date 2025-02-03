#include "osLib.h"
#include "../astString.h"
#include "sstream"
ast_Integer OpenOsLibs(ast_State *L)
{
    NewLib(L, stdos);
    return 1;
}