#include "astFunc.h"
#include "astStack.h"
ast_Bool ast_PushCFunction(ast_State *L, ast_CFunction func)
{
    TValue tt;
    tt.tt = AST_TUSERFUNCTION;
    ast_Closure *cs = (ast_Closure *)calloc(1, sizeof(ast_Closure));
    cs->func = func;
    cs->Upvalues = nullptr;
    tt.value.gc = cast(GCObject *, cs);
    ast_StackPush(PStack(L), tt);
    return TRUE;
}
ast_Bool ast_PushCClosure(ast_State *L, ast_CFunction func, ast_Integer num)
{
    TValue tt;
    tt.tt = AST_TUSERFUNCTION;
    ast_Closure *cs = (ast_Closure *)calloc(1, sizeof(ast_Closure));
    cs->func = func;
    cs->Upvalues = (TValue *)calloc(num, sizeof(TValue));
    for (int i = num - 1; i >= 0; i--)
    {
        cs->Upvalues[i] = astack_Pop(PStack(L));
    }
    cs->Uvslen = num;
    tt.value.gc = cast(GCObject *, cs);
    ast_StackPush(PStack(L), tt);
    return TRUE;
}
ast_Bool ast_IsCFunction(ast_State *L, ast_Integer idx)
{
    TValue tt = ast_StackGetTValue(PStack(L), idx);
    return tt.tt == AST_TUSERFUNCTION ? TRUE : FALSE;
}
ast_CFunction ast_ToCFunction(ast_State *L, ast_Integer idx)
{
    TValue tt = ast_StackGetTValue(PStack(L), idx);
    if (ast_IsCFunction(L, idx))
    {
        return tt.value.gc->cl.func;
    }
    return nullptr;
}