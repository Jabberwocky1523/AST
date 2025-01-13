#include "astUser.h"
#include "astStack.h"
#include "astMath.h"
#include "astTable.h"
#include "astFunc.h"
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
ast_Integer next(ast_State *L)
{
    ast_StackSetTop(PStack(L), 2);
    if (ast_Next(L, 0))
    {
        return 2;
    }
    else
    {
        TValue nil = Nil2Ob();
        ast_StackPush(PStack(L), nil);
        return 1;
    }
}
ast_Integer _iPairsAux(ast_State *L)
{
    TValue i = ast_StackGetTValue(PStack(L), 1);
    if (i.tt == AST_TINTEGER)
    {
        i.value.i++;
    }
    TValue tb = ast_StackGetTValue(PStack(L), 0);
    TValue val = astTable_GetVal(cast(ast_Table *, tb.value.gc), i);
    if (val.tt == AST_TNIL)
    {
        i = Nil2Ob();
        ast_StackPush(PStack(L), i);
        return 1;
    }
    ast_StackPush(PStack(L), i);
    ast_StackPush(PStack(L), val);
    return 2;
}
ast_Integer ipairs(ast_State *L)
{
    ast_PushCFunction(L, _iPairsAux);
    TValue tt = ast_StackGetTValue(PStack(L), 0);
    ast_StackPush(PStack(L), tt);
    tt = Int2Ob(0);
    ast_StackPush(PStack(L), tt);
    return 3;
}
ast_Integer pairs(ast_State *L)
{
    ast_PushCFunction(L, next);
    TValue tt = ast_StackGetTValue(PStack(L), 0);
    ast_StackPush(PStack(L), tt);
    tt = Nil2Ob();
    ast_StackPush(PStack(L), tt);
    return 3;
}
ast_Integer pCall(ast_State *L)
{
    ast_Integer nArgs = astack_GetTop(PStack(L)) - 1;
    ast_Integer status = ast_PCall(L, nArgs, -1);
    TValue bo = Bool2Ob((ast_Bool)(status == AST_OK));
    ast_StackPush(PStack(L), bo);
    astack_Insert(PStack(L), 0);
    return astack_GetTop(PStack(L));
}