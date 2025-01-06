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
#include "astTable.h"
int main(int argc, const char *const *argv)
{
    const char *file_name = "luac.out";
    if (argc >= 2)
    {
        file_name = argv[1];
    }
    astBuffer file_contont = LoadFileToastBuffer(file_name);
    Prototype *proto = astBinaryChunkUnDump(file_contont);
    ast_State *L = (ast_State *)malloc(sizeof(ast_State));
    global_State *g_s = (global_State *)malloc(sizeof(global_State));
    ast_Init(L, g_s);
    PrintAst(proto);
    ast_LoadChunk(L, file_contont, proto, nullptr, 0);
    ast_Call(L, 0, 0);
    return 0;
}