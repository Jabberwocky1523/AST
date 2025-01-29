#include "basicLib.h"
#include "../astFunc.h"
#include "../astVm.h"
#include "../astMath.h"
#include "../astString.h"
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
    ast_String fp = ast_ConvertToString(L, tt);
    ast_Load(L, (char *)getstr(&fp));
    
}