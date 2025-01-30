#include "basicLib.h"
#include "../astFunc.h"
#include "../astVm.h"
#include "../astMath.h"
#include "../astString.h"
#include "../log.h"
ast_Integer OpenBaseLibs(ast_State *L)
{
    ast_PushGlobalTable(L);
    SetFuncs(L, func);
    return 1;
}

ast_Integer Len(ast_State *L)
{
    ast_Integer nl = L->stack->top;
    for (int i = 0; i < nl; i++)
    {
        ast_Len(L, i);
    }
    return nl;
}
ast_Integer LoadFile(ast_State *L)
{
    TValue tt = ast_StackGetTValue(PStack(L), -1);
    ast_String *fp = ast_ConvertToString(L, tt);
    ast_Load(L, (char *)getstr(fp));
    return 1;
}
ast_Integer doFile(ast_State *L)
{
    TValue tt = ast_StackGetTValue(PStack(L), -1);
    ast_String *fp = ast_ConvertToString(L, tt);
    if (!DoFile(L, getstr(fp)))
    {
        PANIC("Error dofile!");
    }
    return 0;
}
ast_Integer select(ast_State *L)
{
    ast_Integer num = astack_GetTop(PStack(L));
    TValue i = ast_StackGetTValue(PStack(L), 0);
    if (i.tt == AST_TSTRING && strcmp(getstr(&i.value.gc->ts), "#") == 0)
    {
        num -= 1;
        ast_StackPush(PStack(L), Int2Ob(num));
        return 1;
    }
    ast_Integer s = ast_ConvertToInteger(i);
    if (s < 0)
    {
        s = s + num;
    }
    else if (s > num - 1)
    {
        s = num - 1;
    }
    if (s < 0)
    {
        PANIC("index out of range!");
    }
<<<<<<< HEAD
    // for (int j = s; j <= num - 1; j++)
    // {
    //     TValue tmp = ast_StackGetTValue(PStack(L), j);
    //     ast_StackPush(PStack(L), tmp);
    // }
    return num - s;
}
ast_Integer tostring(ast_State *L)
{
    ast_Integer num = astack_GetTop(PStack(L));
    for (int i = 0; i < num; i++)
    {
        astack_ToString(L, i);
    }
    return num;
}
ast_Integer tonumber(ast_State *L)
{
    ast_Integer num = astack_GetTop(PStack(L));
    for (int i = 0; i < num; i++)
    {
        ast_Integer flag = 0;
        TValue val = ast_StackGetTValue(PStack(L), i);
        ast_Number n = ast_ConvertToNumberAndGetFlag(val, &flag);
        if (flag == 0)
        {
            PANIC("some str cant convert to number!");
        }
        flag = 0;
        ast_Integer in = ast_DoubleToInteger(n, &flag);
        if (flag)
        {
            ast_StackPush(PStack(L), Int2Ob(in));
        }
        else
        {
            ast_StackPush(PStack(L), Dou2Ob(n));
        }
    }
    return num;
=======
    return s;
>>>>>>> 8f56bee6108f7249811fab839f38dba00b875e02
}