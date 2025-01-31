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
}
ast_Integer rawget(ast_State *L)
{
    CheckType(L, 0, AST_TTABLE);
    CheckAny(L, 1);
    ast_StackSetTop(PStack(L), 2);
    TValue key = ast_StackPop(PStack(L));
    TValue tb = ast_StackGetTValue(PStack(L), -1);
    _ast_GetTable(L, tb, key, TRUE);
    return 1;
}
ast_Integer rawset(ast_State *L)
{
    CheckType(L, 0, AST_TTABLE);
    CheckAny(L, 1);
    CheckAny(L, 2);
    ast_StackSetTop(PStack(L), 3);
    TValue val = ast_StackPop(PStack(L));
    TValue key = ast_StackPop(PStack(L));
    TValue tb = ast_StackGetTValue(PStack(L), -1);
    _ast_SetTable(L, tb, key, val, TRUE);
    return 1;
}
ast_Integer type(ast_State *L)
{
    CheckAny(L, 0);
    ast_StackSetTop(PStack(L), 1);
    ast_Type tt = ast_StackDataType(PStack(L), 0);
    TValue k;
    switch (tt)
    {
    case AST_TNIL:
        k = Char2Ob(L, "Nil");
        break;
    case AST_TINTEGER:
        k = Char2Ob(L, "Long Long");
        break;
    case AST_TNUMBER:
        k = Char2Ob(L, "Number");
        break;
    case AST_TSTRING:
        k = Char2Ob(L, "String");
        break;
    case AST_TTABLE:
        k = Char2Ob(L, "Table");
        break;
    case AST_TBOOLEAN:
        k = Char2Ob(L, "Boolean");
        break;
    case AST_TFUNCTION:
    case AST_TUSERFUNCTION:
        k = Char2Ob(L, "Function");
        break;
    default:
        PANIC("cant find type");
    }
    ast_StackPush(PStack(L), k);
    return 1;
}