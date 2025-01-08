#include "astFunc.h"
#include "astStack.h"
ast_Bool ast_PushCFunction(ast_State *L, ast_CFunction func)
{
    TValue tt;
    tt.tt = AST_TUSERFUNCTION;
    GCObject *ob = (GCObject *)malloc(sizeof(GCObject));
    ob->func = func;
    tt.value.gc = ob;
    ob = nullptr;
    free(ob);
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
        return tt.value.gc->func;
    }
    return nullptr;
}