#include "astStack.h"
#include "astState.h"
#include "astString.h"
#include "astMath.h"
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
    temp.tt = AST_TNIL;
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
        for (int i = abs(n); i > 0; i--)
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
ast_Number ast_ConvertToNumber(TValue val)
{
    switch (val.tt)
    {
    case AST_TBOOLEAN:
    {
        switch (val.value.bo)
        {
        case FALSE:
            return 0;
        case TRUE:
            return 1;
        }
    }
    case AST_TNUMBER:
        return val.value.n;
    case AST_TSTRING:
    {
        ast_Number tmp = 0;
        if (ast_IsNumeric(getstr(&val.value.gc->ts)))
            PANIC("字符串包含非数字");
        assert(tmp = (ast_Number)atoll(getstr(&val.value.gc->ts)));
        return tmp;
    }
    case AST_TINTEGER:
        return (ast_Number)val.value.i;
    default:
        return 0;
    }
}
ast_Integer ast_ConvertToInteger(TValue val)
{
    switch (val.tt)
    {
    case AST_TBOOLEAN:
    {
        switch (val.value.bo)
        {
        case FALSE:
            return 0;
        case TRUE:
            return 1;
        }
    }
    case AST_TNUMBER:
        return (ast_Integer)val.value.n;
    case AST_TSTRING:
    {
        ast_Number tmp = 0;
        if (ast_IsNumeric(getstr(&val.value.gc->ts)))
            PANIC("字符串包含非数字");
        assert(tmp = (ast_Number)atoll(getstr(&val.value.gc->ts)));
        return (ast_Integer)tmp;
    }
    case AST_TINTEGER:
        return val.value.i;
    default:
        return 0;
    }
}
ast_String ast_ConvertToString(ast_State *L, TValue &val)
{
    switch (val.tt)
    {
    case AST_TBOOLEAN:
    {
        ast_Bool num = val.value.bo;
        char *tmp = (char *)malloc(sizeof(char) * 2);
        sprintf(tmp, "%d", num);
        ast_String *st = astString_NewLStr(L, tmp, strlen(tmp));
        return *st;
    }
    case AST_TSTRING:
        return val.value.gc->ts;
    case AST_TNUMBER:
    {
        ast_Number num = val.value.n;
        char *tmp = (char *)malloc(sizeof(char) * 32);
        sprintf(tmp, "%f", num);
        val.tt = AST_TSTRING;
        val.value.gc = (GCObject *)astString_NewLStr(L, tmp, strlen(tmp));
        return val.value.gc->ts;
    }
    case AST_TINTEGER:
    {
        ast_Integer num = val.value.i;
        char *tmp = (char *)malloc(sizeof(char) * 32);
        sprintf(tmp, "%lld", num);
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
        printf("无效变量类型,无法转化\n");
        return def;
    }
    }
}
ast_Bool ast_StackPush(ast_Stack *L, void *val, ast_Type type)
{
    TValue tmp;
    switch (type)
    {
    case AST_TNIL:
    {
        tmp.tt = AST_TNIL;
        tmp.value.p = nullptr;
        ast_StackPush(L, tmp);
        return TRUE;
    }
    case AST_TNUMBER:
    {
        tmp.tt = AST_TNUMBER;
        tmp.value.n = *(ast_Number *)val;
        ast_StackPush(L, tmp);
        return TRUE;
    }
    case AST_TINTEGER:
    {
        tmp.tt = AST_TINTEGER;
        tmp.value.i = *(ast_Integer *)val;
        ast_StackPush(L, tmp);
        return TRUE;
    }
    default:
    {
        return FALSE;
    }
    }
}
ast_Bool ast_StackPush(ast_State *L, char *val, ast_Type type)
{
    ast_Stack *Stack = L->stack;
    TValue tmp;
    tmp.tt = AST_TSTRING;
    tmp.value.gc = (GCObject *)astString_NewLStr(L, val, strlen(val));
    ast_StackPush(Stack, tmp);
    return TRUE;
}
ast_Bool ast_StackPush(ast_Stack *L, ast_Bool val, ast_Type type)
{
    TValue tmp;
    tmp.tt = AST_TBOOLEAN;
    tmp.value.bo = val;
    ast_StackPush(L, tmp);
    return TRUE;
}
ast_Bool ast_StackPush(ast_Stack *L, ast_Number val, ast_Type type)
{

    TValue tmp;
    switch (type)
    {
    case AST_TNUMBER:
    {
        tmp.tt = AST_TNUMBER;
        tmp.value.n = val;
        break;
    }
    case AST_TINTEGER:
    {
        tmp.tt = AST_TINTEGER;
        tmp.value.i = val;
        break;
    }
    }
    ast_StackPush(L, tmp);
    return TRUE;
}

ast_Bool ast_PrintTValue(TValue &val)
{
    switch (val.tt)
    {
    case AST_TNIL:
        printf("[NIL] ");
        return TRUE;
    case AST_TBOOLEAN:
        switch (val.value.bo)
        {
        case TRUE:
            printf("[TRUE] ");
            return TRUE;
        case FALSE:
            printf("[FALSE] ");
            return FALSE;
        }
    case AST_TSTRING:
    {
        printf("[\"%s\"] ", getstr(&val.value.gc->ts));
        return TRUE;
    }
    case AST_TNUMBER:
        printf("[%f] ", val.value.n);
        return TRUE;
    case AST_TINTEGER:
        printf("[%lld] ", val.value.i);
        return TRUE;
    default:
        return FALSE;
    }
}
ast_Bool ast_PrintStack(ast_Stack *L)
{
    for (int i = 0; i < L->top; i++)
    {
        ast_PrintTValue(L->Value[i]);
    }
    printf("\n");
    return TRUE;
}
