#include "astUser.h"
#include "astStack.h"
#include "astMath.h"
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