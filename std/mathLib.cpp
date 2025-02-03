#include "mathLib.h"
ast_Integer OpenMathLibs(ast_State *L)
{
    NewLib(L, stdmath);
    return 1;
}
ast_Integer max(ast_State *L)
{
    ast_Integer num = L->stack->top;
    ast_Number max = LONG_MIN;
    ast_Integer flag = 0;
    for (int i = 0; i < num; i++)
    {
        ast_Number tmp = ast_ConvertToNumber(ast_StackGetTValue(PStack(L), i));
        if (max < tmp)
        {
            max = tmp;
        }
    }
    ast_StackPush(PStack(L), Dou2Ob(max));
    return 1;
}