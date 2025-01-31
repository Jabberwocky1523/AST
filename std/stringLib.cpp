#include "stringLib.h"
#include "../astMath.h"
#include "../astString.h"
ast_Integer OpenStringLibs(ast_State *L)
{
    NewLib(L, stdstr);
    return 1;
}
ast_Integer len(ast_State *L)
{
    CheckType(L, 0, AST_TSTRING);
    ast_Len(L, 0);
    return 1;
}
ast_Integer sub(ast_State *L)
{
    CheckType(L, 0, AST_TSTRING);
    CheckAny(L, 1);
    CheckAny(L, 2);
    ast_StackSetTop(PStack(L), 3);
    TValue len = ast_StackPop(PStack(L));
    TValue begin = ast_StackPop(PStack(L));
    if (begin.value.i == 0)
    {
        return 1;
    }
    TValue str = ast_StackGetTValue(PStack(L), -1);
    String s = getstr(&str.value.gc->ts);
    s = s.substr(begin.value.i - 1, len.value.i);
    ast_StackPush(PStack(L), Char2Ob(L, s.c_str()));
    return 1;
}