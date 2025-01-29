#include "stdlib.h"
#include "stdio.h"
#include "astMem.h"
#include "astBuffer.h"
#include "astString.h"
#include "astMath.h"
#include "astStack.h"
#include "astOpcode.h"
#include "astUtils.h"
#include "astMap.h"
#include "astVm.h"
#include "log.h"
#include "astUser.h"
#include "astTable.h"
#include "astError.h"
#include "std/auxLib.h"
int main(int argc, const char *const *argv)
{
    const char *file_name = "/home/kurisu/桌面/AST/lua/test2.lua";
    if (argc >= 2)
    {
        file_name = argv[1];
    }
    ast_State *L = (ast_State *)malloc(sizeof(ast_State));
    global_State *g_s = (global_State *)malloc(sizeof(global_State));
    ast_Init(L, g_s);
    OpenLibs(L);
    ast_Load(L, (char *)file_name);
    ast_Call(L, 0, 0);
    return 0;
}