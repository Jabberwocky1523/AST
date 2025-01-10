#include "astUser.h"
#include "astStack.h"
#include "astMath.h"
#include "astTable.h"
ast_Integer astPrintTest(ast_State *L)
{
    ast_Integer len = L->stack->top;
    int num;
    for (int i = 0; i < len; i++)
    {
        ast_PrintTValue(L->stack->Value[i]);
    }
    printf("\n");
    return 0;
}
ast_Integer astTest(ast_State *L)
{
    ast_Number sum = 0;
    for (int i = 0; i < L->stack->top; i++)
    {
        sum += ast_ConvertToNumber(L->stack->Value[i]);
    }
    TValue tt = Dou2Ob(sum);
    ast_StackPush(PStack(L), tt);
    return 1;
}
ast_Integer getMetatable(ast_State *L)
{
    ast_Bool ok = ast_GetMetaTableFromIdx(L, 0);
    if (!ok)
    {
        TValue nil = Nil2Ob();
        ast_StackPush(PStack(L), nil);
    }
    return 1;
}
ast_Integer setMetatable(ast_State *L)
{
    ast_SetMetaTableFromIdx(L, 0);
    return 0;
}