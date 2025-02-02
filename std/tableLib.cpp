#include "tableLib.h"
#include "../log.h"
ast_Integer OpenTableLibs(ast_State *L)
{
    NewLib(L, stdtable);
    return 1;
}
ast_Integer move(ast_State *L)
{
    return 1;
}
ast_Bool insertV(TValue tb, TValue val)
{
    return astTableArr_Push(&tb.value.gc->tb, val);
}
ast_Bool insertVFI(TValue tb, TValue idx, TValue val)
{
    Vector<TValue> tmp;
    ast_Integer len = TableArrLen(&tb.value.gc->tb);
    ast_Integer idxx = idx.value.i;
    if (idxx > len || len == 0)
    {
        return insertV(tb, val);
    }
    for (int i = 0; i < len; i++)
    {
        tmp.push_back(tb.value.gc->tb.arr[i]);
    }
    tmp.insert(tmp.begin() + idxx - 1, val);
    tb.value.gc->tb.arrtop = 0;
    for (int i = 0; i < tmp.size(); i++)
    {
        astTableArr_Push(&tb.value.gc->tb, tmp[i]);
    }
    return TRUE;
}
ast_Integer insert(ast_State *L)
{
    ast_Integer num = L->stack->top;
    CheckType(L, 0, AST_TTABLE);
    if (num == 2)
    {
        ast_StackSetTop(PStack(L), 2);
        CheckAny(L, 1);
        TValue val = ast_StackPop(PStack(L));
        TValue tb = ast_StackGetTValue(PStack(L), 0);
        insertV(tb, val);
        return 1;
    }
    ast_StackSetTop(PStack(L), 3);
    TValue val = ast_StackPop(PStack(L));
    TValue idx = ast_StackPop(PStack(L));
    TValue tb = ast_StackGetTValue(PStack(L), 0);
    insertVFI(tb, idx, val);
    return 1;
}