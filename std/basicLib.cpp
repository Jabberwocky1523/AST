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
    return s;
}