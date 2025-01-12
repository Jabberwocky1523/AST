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
int main2()
{
    TValue t1 = Nil2Ob();
    TValue t2 = Dou2Ob(1);
    ast_State *L = (ast_State *)malloc(sizeof(ast_State));
    global_State *g_s = (global_State *)malloc(sizeof(global_State));
    ast_Init(L, g_s);
    ast_Map *map = astMap_Init(8);
    astMap_PushKeyVal(map, t1, t2);
    ast_PrintMap(map);
    astMap_PushKeyVal(map, t1, Int2Ob(2));
    ast_PrintMap(map);
}