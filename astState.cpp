#include "astState.h"
#include "ast.h"
#include "astString.h"
#include "astObject.h"
ast_Bool ast_Init(ast_State *L, global_State *g_s)
{
    g_s->StringBuff = realloc(NULL, 1024);
    StringTable ts;
    GCObject **hashtable;
    hashtable = (GCObject **)(malloc(sizeof(GCObject *) * 16));
    for (int i = 0; i < 16; i++)
    {
        *(hashtable + i) = (GCObject *)malloc(sizeof(GCObject));
    }
    ts.hashTable = hashtable;
    ts.Tnum = 0;
    ts.size = 16;
    g_s->stringtable = ts;
    L->G_S = g_s;
    return TRUE;
}
// 创建虚拟栈
ast_State *ast_NewState(ast_Alloc func, void *ud)
{
    ast_State *L;
    global_State *g;
}
ast_Stack *ast_NewStack(int size)
{
    ast_Stack *L = (ast_Stack *)malloc(sizeof(ast_Stack));
    TValue *Value = (TValue *)malloc(size * sizeof(TValue));
    L->top = 0;
    L->size = size;
    L->Value = Value;
    return L;
}
ast_Bool ast_StackCheck(ast_Stack *L, int n)
{
    int free = L->size - L->top;
    if (free < n)
    {
        realloc(L->Value, (L->size + n) * sizeof(TValue));
        L->size = L->size + n;
    }
    for (int i = free; i < n; i++)
    {
        TValue temp;
        temp.tt = AST_TNIL;
        temp.value.b = 0;
        L->Value[L->top++] = temp;
    }
    return TRUE;
}

ast_Bool ast_StackPush(ast_Stack *L, TValue &value)
{
    if (L->top == L->size)
    {
        PANIC("栈溢出");
    }
    L->Value[L->top++] = value;
    return TRUE;
}
TValue ast_StackPop(ast_Stack *L)
{
    if (L->top < 1)
    {
        PANIC("栈为空");
    }
    L->top--;
    TValue temp = L->Value[L->top];
    L->Value[L->top].tt = AST_TNIL;
    L->Value[L->top].value.n = 0;
    return temp;
}
int ast_StackAbsIndex(ast_Stack *L, int idx)
{
    if (idx >= 0)
    {
        return idx;
    }
    return idx + L->top;
}
ast_Bool ast_StackIdxIsValid(ast_Stack *L, int idx)
{
    idx = ast_StackAbsIndex(L, idx);
    if (idx >= 0 && idx < L->top)
    {
        return TRUE;
    }
    return FALSE;
}
TValue ast_StackGetTValue(ast_Stack *L, int idx)
{
    int absIdx = ast_StackAbsIndex(L, idx);
    if (absIdx >= 0 && absIdx < L->top)
    {
        return L->Value[absIdx];
    }
    TValue temp;
    temp.tt = AST_TNONE;
    temp.value.gc = NULL;
    return temp;
}
ast_Bool ast_StackSetTValue(ast_Stack *L, TValue &value, int idx)
{
    int absIdx = ast_StackAbsIndex(L, idx);
    if (absIdx >= 0 && absIdx < L->top)
    {
        L->Value[idx] = value;
        return TRUE;
    }
    PANIC("无效的地址");
}
ast_Bool ast_StackReverse(ast_Stack *L, int idx1, int idx2)
{
    TValue *value = L->Value;
    if (idx1 > L->top - 1 || idx2 > L->top - 1)
    {
        PANIC("无效地址");
    }
    for (; idx1 < idx2;)
    {
        swap(value[idx1], value[idx2]);
        idx1++;
        idx2--;
    }
    return TRUE;
}
ast_Bool ast_StackRotate(ast_Stack *L, int idx, int n)
{
    int top = L->top - 1;
    int p = ast_StackAbsIndex(L, idx);
    int m;
    if (n >= 0)
    {
        m = top - n;
    }
    else
    {
        m = p - n - 1;
    }
    ast_StackReverse(L, p, m);
    ast_StackReverse(L, m + 1, top);
    ast_StackReverse(L, p, top);
    return TRUE;
}
ast_Bool ast_StackSetTop(ast_Stack *L, int idx)
{
    int newtop = ast_StackAbsIndex(L, idx);
    int n = newtop - L->top;
    if (n > 0)
    {
        for (int i = 0; i < n; i++)
        {
            TValue tt;
            tt.tt = AST_TNIL;
            tt.value.n = 0;
            ast_StackPush(L, tt);
        }
    }
    else
    {
        for (int i = n; i > 0; i--)
        {
            ast_StackPop(L);
        }
    }
    return TRUE;
}
int ast_StackDataType(ast_Stack *L, int idx)
{
    if (ast_StackIdxIsValid(L, idx))
    {
        return L->Value[idx].tt;
    }
    return AST_TNONE;
}
ast_Bool ast_ConvertToBoolean(TValue val)
{
    switch (val.tt)
    {
    case AST_TNIL:
        return FALSE;
    case AST_TBOOLEAN:
        return val.value.bo;
    default:
        return TRUE;
    }
}
ast_Number ast_ConvertToNumber(TValue val, int &flag)
{
    switch (val.tt)
    {
    case AST_TNUMBER:
        flag = 1;
        return val.value.n;
    default:
        flag = 0;
        return 0;
    }
}
ast_String ast_ConvertToString(ast_State *L, TValue &val)
{
    switch (val.tt)
    {
    case AST_TSTRING:
        return val.value.gc->ts;
    case AST_TNUMBER:
    {
        ast_Number num = val.value.n;
        char *tmp = (char *)malloc(sizeof(char) * 20);
        sprintf(tmp, "%f", num);
        val.tt = AST_TSTRING;
        val.value.gc = (GCObject *)astString_NewLStr(L, tmp, sizeof(tmp) / sizeof(tmp[0]));
        return val.value.gc->ts;
    }
    default:
    {
        ast_String def;
        def.Tsv.hash = 0;
        def.Tsv.len = 0;
        def.Tsv.marked = 0;
        def.Tsv.next = NULL;
        def.Tsv.reserved = 0;
        def.Tsv.tt = AST_TSTRING;
        return def;
    }
    }
}