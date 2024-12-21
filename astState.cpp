#include "astState.h"
#include "ast.h"

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
    printf("%d\n", L->Value[L->top - 1].tt);
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
