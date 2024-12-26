#include "stdlib.h"
#include "stdio.h"
#include "astMem.h"
#include "astBuffer.h"
#include "astString.h"
#include "astMath.h"
#include "astStack.h"
#include "astOpcode.h"
#include "astUtils.h"
#include "astVm.h"
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
    ast_Init(L, g_s, proto, 0);
    astack_SetTop(PStack(L), proto->MaxStackSize);
    PrintAst(proto);
    while (1)
    {
        int pc = ast_GetPc(L);
        Instruction ins = ast_Fetch(L);
        if (InstructionOpcode(ins) != OP_RETURN)
        {
            ast_ExecuteOp(L, ins);
            printf("[%d] %s", pc + 1, InstructionOpName(ins));
            ast_PrintStack(PStack(L));
        }
        else
        {
            break;
        }
    }
}