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
    const char *file_name = "../lua/pcall.lua";
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
    TValue r = ast_StackGetTValue(PStack(L), AST_REGISTRYINDEX);
    ast_PrintTable(r.value.gc->tb);
    TValue g = astTable_GetVal(cast(ast_Table *, r.value.gc), Int2Ob(AST_RIDX_GLOBALS));
    ast_PrintTable(g.value.gc->tb);
    return 0;
}