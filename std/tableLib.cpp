#include "tableLib.h"
#include "../log.h"
#include "../astString.h"
#include "sstream"
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
ast_Integer remove(ast_State *L)
{
    CheckType(L, 0, AST_TTABLE);
    CheckType(L, 1, AST_TINTEGER);
    ast_StackSetTop(PStack(L), 2);
    TValue tb = ast_StackGetTValue(PStack(L), 0);
    TValue idx = ast_StackGetTValue(PStack(L), 1);
    TValue tmp = tb.value.gc->tb.arr[idx.value.i - 1];
    for (int i = idx.value.i - 1; i < tb.value.gc->tb.arrtop - 1; i++)
    {
        tb.value.gc->tb.arr[i] = tb.value.gc->tb.arr[i + 1];
    }
    tb.value.gc->tb.arrtop--;
    ast_StackPush(PStack(L), tmp);
    return 1;
}
ast_Integer concat(ast_State *L)
{
    std::ostringstream oss;
    CheckType(L, 0, AST_TTABLE);
    ast_StackSetTop(PStack(L), 4);
    TValue tb = ast_StackGetTValue(PStack(L), 0);
    TValue sep = ast_StackGetTValue(PStack(L), 1);
    TValue i = ast_StackGetTValue(PStack(L), 2);
    TValue j = ast_StackGetTValue(PStack(L), 3);
    String s;
    ast_Integer ix, jx;
    if (sep.tt == AST_TNIL)
    {
        s = "";
    }
    else
    {
        s = getstr(&sep.value.gc->ts);
    }
    ix = i.value.i - 1, jx = j.value.i - 1;
    if (jx < 0)
    {
        jx = TableArrLen(&tb.value.gc->tb) - 1;
    }
    if (ix < 0)
    {
        ix = 0;
    }
    for (int i = ix; i <= jx && i < TableArrLen(&tb.value.gc->tb); i++)
    {
        TValue tt = tb.value.gc->tb.arr[i];
        switch (tt.tt)
        {
        case AST_TINTEGER:
            oss << tt.value.i << s.c_str();
            break;
        case AST_TNUMBER:
            oss << tt.value.n << s.c_str();
            break;
        case AST_TSTRING:
            oss << getstr(&tt.value.gc->ts) << s.c_str();
            break;
        }
    }
    ix = s.size();
    s = oss.str().c_str();
    for (int i = 0; i < ix; i++)
    {
        s.erase(s.end() - 1);
    }
    ast_StackPush(PStack(L), Char2Ob(L, s.c_str()));
    return 1;
}
ast_Integer pack(ast_State *L)
{
    ast_Integer num = L->stack->top;
    ast_NewTable(L);
    TValue tb = ast_StackGetTValue(PStack(L), -1);
    for (int i = 0; i < num; i++)
    {
        TValue tt = ast_StackGetTValue(PStack(L), i);
        astTableArr_Push(&tb.value.gc->tb, tt);
    }
    return 1;
}
ast_Integer unpack(ast_State *L)
{
    CheckType(L, 0, AST_TTABLE);
    ast_StackSetTop(PStack(L), 3);
    TValue j = ast_StackPop(PStack(L));
    TValue i = ast_StackPop(PStack(L));
    TValue tb = ast_StackGetTValue(PStack(L), -1);
    ast_Integer ix = i.value.i - 1, jx = j.value.i - 1, len = TableArrLen(&tb.value.gc->tb);
    if (ix < 0)
    {
        ix = 0;
    }
    if (jx < 0)
    {
        jx = len;
    }
    for (int i = ix; i <= jx && i < len; i++)
    {
        ast_StackPush(PStack(L), tb.value.gc->tb.arr[i]);
    }
    return len;
}
ast_Integer free(ast_State *L)
{
    CheckType(L, 0, AST_TTABLE);
    ast_StackSetTop(PStack(L), 1);
    TValue tb = ast_StackGetTValue(PStack(L), 0);
    ast_RemoveTable(L, &tb.value.gc->tb);
    ast_StackPush(PStack(L), Nil2Ob());
    return 1;
}