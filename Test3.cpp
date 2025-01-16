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
#include "astTable.h"
#include "astLexer.h"
int main(int argc, const char *const *argv)
{
    ast_State *L = (ast_State *)malloc(sizeof(ast_State));
    global_State *g_s = (global_State *)malloc(sizeof(global_State));
    ast_Init(L, g_s);
    char *a = "1234z";
    TValue str = ast_ScanNumber(L, a);
    a = "123213'";
    ast_PrintTValue(str);
    str = ast_ScanStr(L, a);
    ast_PrintTValue(str);
}