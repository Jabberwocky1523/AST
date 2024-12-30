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
int main3(int argc, const char *const *argv)
{
    ast_State *L = (ast_State *)malloc(sizeof(ast_State));
    global_State *g_s = (global_State *)malloc(sizeof(global_State));
    ast_Init(L, g_s);
    ast_String *a = astString_NewLStr(L, "123", 3);
    TValue tt = Str2Ob(L, a);
    char *b = "234";
    TValue tt2 = Char2Ob(L, b);
    ast_PrintTValue(tt);
    ast_PrintTValue(tt2);
    TValue tt3 = Nil2Ob();
    ast_PrintTValue(tt3);
    ast_Bool bo = TRUE;
    TValue tt4 = Bool2Ob(bo);
    ast_PrintTValue(tt4);
    ast_Table *tb = astTable_Init(1, 1);
    TValue tt5 = Tb2Ob(tb);
    ast_PrintTValue(tt5);
}