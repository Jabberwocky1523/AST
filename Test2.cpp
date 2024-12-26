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
int main(int argc, const char *const *argv)
{
    ast_State *L = (ast_State *)malloc(sizeof(ast_State));
    global_State *g_s = (global_State *)malloc(sizeof(global_State));
    ast_Init(L, g_s, NULL, 0);
    ast_Map *map = astMap_Init((ast_Integer)8);
    TValue key, val;
    for (int i = 0; i < 30; i++)
    {
        key.tt = AST_TSTRING;
        val.tt = AST_TSTRING;
        char *a = (char *)malloc(sizeof(char));
        sprintf(a, "%d", i);
        ast_String *st = astString_NewLStr(L, a, strlen(a));
        key.value.gc = (GCObject *)st;
        val.value.gc = (GCObject *)st;
        astMap_PushKeyVal(map, key, val);
        printf("%d %d\n", map->size, map->Mnum);
    }
    TValue tt = astMap_GetValFromKey(map, key);
    printf("%s", getstr(&tt.value.gc->ts));
    printf("\n%d", L->G_S->stringtable.size);
}