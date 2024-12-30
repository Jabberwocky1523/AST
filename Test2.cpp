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
int main2(int argc, const char *const *argv)
{
    ast_State *L = (ast_State *)malloc(sizeof(ast_State));
    global_State *g_s = (global_State *)malloc(sizeof(global_State));
    ast_Init(L, g_s);
    ast_Table *tb = astTable_Init(8, 8);
    TValue key, val;
    key.tt = AST_TNUMBER;
    val.tt = AST_TNUMBER;
    key.value.n = 3;
    val.value.n = 2;
    astTable_PushVal(tb, key, val);
    TValue key1 = key;
    for (int i = 0; i < 3; i++)
    {
        key.tt = AST_TNUMBER;
        key.value.n = i;
        val.tt = AST_TINTEGER;
        val.value.i = i + 1;
        astTable_PushVal(tb, key, val);
        printf("%d %d %d %d\n", tb->arrtop, tb->arrSize, tb->HashMap->Mnum, tb->HashMap->size);
    }
    val.tt = AST_TNIL;
    astTable_PushVal(tb, key, val);
}