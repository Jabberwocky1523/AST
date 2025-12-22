#include "iostream"
#include "../astState.h"
#include "test.h"
#include "stdlib.h"
#include "stdio.h"
#include "../astMem.h"
#include "../astBuffer.h"
#include "../astString.h"
#include "../astMath.h"
#include "../astStack.h"
#include "../astOpcode.h"
#include "../astUtils.h"
#include "../astMap.h"
#include "../astVm.h"
#include "../log.h"
#include "../astGc.h"
#include "../astUser.h"
#include "../astTable.h"
#include "../astError.h"
#include "../auxLib.h"
extern "C"
{
    ast_Integer test(ast_State *L)
    {
        printf("test! 123 123");
        return 0;
    }
    FuncRegs proto = {{"test", test}};
    ast_Integer astOpen(ast_State *L)
    {
        NewLib(L, proto);
        return 1;
    }
}
// auto main() -> int
// {
// }