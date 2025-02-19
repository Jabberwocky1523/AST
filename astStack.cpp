#include "astStack.h"
#include "astState.h"
#include "astString.h"
#include "astMath.h"
#include "string.h"
#include "astMap.h"
#include "astTable.h"
#include "log.h"
ast_Stack *ast_NewStack(int size, ast_State *S)
{
    ast_Stack *L = (ast_Stack *)malloc(sizeof(ast_Stack));
    L->Value = (TValue *)malloc(size * sizeof(TValue));
    for (int i = 0; i < size; i++)
    {
        L->Value[i].tt = AST_TNIL;
    }
    L->openuvs = nullptr;
    L->top = 0;
    L->size = size;
    L->pc = 0;
    L->prev = nullptr;
    L->varargs = nullptr;
    L->nPrevFuncResults = 0;
    L->PrevIdx = 0;
    L->varArgsLen = 0;
    L->L = S;
    return L;
}
ast_Bool ast_StackCheck(ast_Stack *L, int n)
{
    int free = L->size - L->top;
    if (free < n)
    {

        TValue *newP = (TValue *)realloc(L->Value, (L->size + n) * sizeof(TValue));
        if (newP != nullptr)
        {
            L->Value = newP;
        }
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
ast_Bool ast_StackPush(ast_Stack *L, TValue &&value)
{
    return ast_StackPush(L, value);
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
    if (idx <= AST_REGISTRYINDEX)
    {
        return idx;
    }
    else if (idx >= 0)
    {
        return idx;
    }
    return idx + L->top;
}
ast_Bool ast_StackIdxIsValid(ast_Stack *L, int idx)
{
    idx = ast_StackAbsIndex(L, idx);
    if (idx == AST_REGISTRYINDEX)
    {
        return TRUE;
    }
    else if (idx < AST_REGISTRYINDEX)
    {
        idx = AST_REGISTRYINDEX - idx - 1;
        if (L->closure != nullptr && idx < L->closure->Uvslen)
        {
            return TRUE;
        }
        return FALSE;
    }
    else if (idx >= 0 && idx < L->top)
    {
        return TRUE;
    }
    return FALSE;
}
TValue ast_StackGetTValue(ast_Stack *L, int idx)
{
    int absIdx = ast_StackAbsIndex(L, idx);
    if (absIdx == AST_REGISTRYINDEX)
    {
        return L->L->Registry;
    }
    else if (absIdx < AST_REGISTRYINDEX)
    {
        absIdx = AST_REGISTRYINDEX - absIdx - 1;
        if (L->closure == nullptr || absIdx >= L->closure->Uvslen)
        {
            return Nil2Ob();
        }
        return L->closure->Upvalues[absIdx];
    }
    else if (absIdx >= 0 && absIdx < L->top)
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
    if (absIdx == AST_REGISTRYINDEX && value.tt == AST_TTABLE)
    {
        L->L->Registry = value;
        return TRUE;
    }
    if (absIdx < AST_REGISTRYINDEX)
    {
        absIdx = AST_REGISTRYINDEX - absIdx - 1;
        if (L->closure != nullptr && absIdx < L->closure->Uvslen)
        {
            L->closure->Upvalues[absIdx] = value;
        }
        return TRUE;
    }
    if (absIdx >= 0 && absIdx < L->top)
    {
        L->Value[absIdx] = value;
        return TRUE;
    }
    else if (absIdx == L->top)
    {
        ast_StackPush(L, value);
        return TRUE;
    }
    PANIC("无效的地址");
}
ast_Bool ast_StackReverse(ast_Stack *L, int idx1, int idx2)
{
    TValue *value = L->Value;
    idx1 = ast_StackAbsIndex(L, idx1);
    idx2 = ast_StackAbsIndex(L, idx2);
    if (idx1 > L->top - 1 || idx2 > L->top - 1)
    {
        PANIC("无效地址");
    }
    for (; idx1 < idx2;)
    {
        std::swap(value[idx1], value[idx2]);
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
    idx = ast_StackAbsIndex(L, idx);
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
    case AST_TNIL:
        return 0;
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
        if (!ast_IsNumeric(getstr(&val.value.gc->ts)))
        {
            PANIC("字符串包含非数字");
        }
        assert(tmp = strtod(getstr(&val.value.gc->ts), nullptr));
        return tmp;
    }
    case AST_TINTEGER:
        return (ast_Number)val.value.i;
    default:
        PANIC("该类型无法转换到Number");
    }
}
ast_Number ast_ConvertToNumberAndGetFlag(TValue val, ast_Integer *flag)
{
    *flag = 0;
    switch (val.tt)
    {
    case AST_TNIL:
        *flag = 1;
        return 0;
    case AST_TBOOLEAN:
    {
        switch (val.value.bo)
        {
        case FALSE:
            *flag = 1;
            return 0;
        case TRUE:
            *flag = 1;
            return 1;
        }
    }
    case AST_TNUMBER:
        *flag = 1;
        return val.value.n;
    case AST_TSTRING:
    {
        ast_Number tmp = 0;
        if (!ast_IsNumeric(getstr(&val.value.gc->ts)))
        {
            PANIC("字符串包含非数字");
        }
        *flag = 1;
        assert(tmp = atof(getstr(&val.value.gc->ts)));
        return tmp;
    }
    case AST_TINTEGER:
        *flag = 1;
        return (ast_Number)val.value.i;
    default:
    {
        *flag = 0;
        return 0;
    }
    }
}
ast_Integer ast_ConvertToInteger(TValue val)
{
    switch (val.tt)
    {
    case AST_TNIL:
        return 0;
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
    {
        ast_Integer flag = 0;
        ast_Integer tmp = ast_DoubleToInteger(val.value.n, &flag);
        if (!flag)
        {
            return 0;
        }
        return (ast_Integer)val.value.n;
    }

    case AST_TSTRING:
    {
        ast_Integer tmp = 0;
        if (!ast_IsNumeric(getstr(&val.value.gc->ts)))
        {
            PANIC("字符串包含非数字");
        }
        assert(tmp = (ast_Integer)atoll(getstr(&val.value.gc->ts)));
        return (ast_Integer)tmp;
    }
    case AST_TINTEGER:
        return val.value.i;
    default:
        PANIC("该类型无法转换为Integer");
    }
}
ast_Integer ast_ConvertToIntegerAndGetFlag(TValue val, ast_Integer *flag)
{
    *flag = 0;
    switch (val.tt)
    {
    case AST_TBOOLEAN:
    {
        *flag = 1;
        switch (val.value.bo)
        {
        case FALSE:
            return 0;
        case TRUE:
            return 1;
        }
    }
    case AST_TNUMBER:
    {
        ast_Integer tmp = ast_DoubleToInteger(val.value.n, flag);
        return tmp;
    }
    case AST_TSTRING:
    {
        ast_Number tmp = 0;
        if (!ast_IsNumeric(getstr(&val.value.gc->ts)))
        {
            PANIC("字符串包含非数字");
        }
        assert(tmp = (ast_Number)atof(getstr(&val.value.gc->ts)));
        tmp = ast_DoubleToInteger(tmp, flag);
        return (ast_Integer)tmp;
    }
    case AST_TINTEGER:
        *flag = 1;
        return val.value.i;
    default:
        *flag = 0;
        return 0;
    }
}
ast_String *ast_ConvertToString(ast_State *L, TValue &val)
{
    switch (val.tt)
    {
    case AST_TBOOLEAN:
    {
        TValue tmp;
        if (val.value.bo == TRUE)
        {
            tmp = Char2Ob(L, "True");
        }
        else
        {
            tmp = Char2Ob(L, "False");
        }
        val.value.gc = tmp.value.gc;
        val.tt = AST_TSTRING;
        return &val.value.gc->ts;
    }
    case AST_TSTRING:
        return &val.value.gc->ts;
    case AST_TNUMBER:
    {
        ast_Number num = val.value.n;
        char *tmp = (char *)malloc(sizeof(char) * 128);
        sprintf(tmp, "%f", num);
        val.tt = AST_TSTRING;
        char *str = (char *)malloc(sizeof(char) * strlen(tmp) + 1);
        memcpy(str, tmp, strlen(tmp));
        str[strlen(tmp)] = '\0';
        val.value.gc = (GCObject *)astString_NewLStr(L, str, strlen(tmp));
        free(tmp);
        free(str);
        return &val.value.gc->ts;
    }
    case AST_TINTEGER:
    {
        ast_Integer num = val.value.i;
        char *tmp = (char *)malloc(sizeof(char) * 128);
        sprintf(tmp, "%lld", num);
        val.tt = AST_TSTRING;
        char *str = (char *)malloc(sizeof(char) * strlen(tmp) + 1);
        memcpy(str, tmp, strlen(tmp));
        str[strlen(tmp)] = '\0';
        val.value.gc = (GCObject *)astString_NewLStr(L, str, strlen(tmp));
        free(tmp);
        free(str);
        return &val.value.gc->ts;
    }
    default:
    {
        printf("无效变量类型,无法转化\n");
        return nullptr;
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
            printf("true");
            return TRUE;
        case FALSE:
            printf("false");
            return FALSE;
        }
    case AST_TSTRING:
    {
        printf("%s", getstr(&val.value.gc->ts));
        return TRUE;
    }
    case AST_TNUMBER:
        // std::cout << val.value.n;
        printf("%.14g", val.value.n);
        return TRUE;
    case AST_TINTEGER:
        printf("%lld", val.value.i);
        return TRUE;
    case AST_TTABLE:
        ast_PrintTable(val.value.gc->tb);
        return TRUE;
    case AST_TFUNCTION:
        printf("[FUNCTION%d-%d] ", val.value.gc->cl.pr->LineDefined, val.value.gc->cl.pr->LastLineDefined);
        return TRUE;
    case AST_TUSERFUNCTION:
        printf("[USERFUNCTION] ");
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
ast_Bool ast_StackPushConstant(ast_State *L, Constant val)
{
    switch (val.tag)
    {
    case CONSTANT_TAG_NIL:
        ast_StackPush(PStack(L), nullptr, AST_TNIL);
        return TRUE;
    case CONSTANT_TAG_BOOLEAN:
        ast_StackPush(PStack(L), (ast_Bool)val.tag_boolean, AST_TBOOLEAN);
        return TRUE;
    case CONSTANT_TAG_NUMBER:
        ast_StackPush(PStack(L), val.tag_number, AST_TNUMBER);
        return TRUE;
    case CONSTANT_TAG_INTEGER:
        ast_StackPush(PStack(L), val.tag_integer, AST_TINTEGER);
        return TRUE;
    case CONSTANT_TAG_STR:
    {
        char *buff = (char *)malloc(astBufferDataSize(val.tag_str) * sizeof(char) + 1);
        memcpy(buff, astBufferData(val.tag_str), astBufferDataSize(val.tag_str));
        buff[astBufferDataSize(val.tag_str)] = '\0';
        TValue str = Char2Ob(L, buff);
        free(buff);
        ast_StackPush(PStack(L), str);
        return TRUE;
    }
        return FALSE;
    }
}
ast_Bool ast_FreeStack(ast_Stack *L)
{
    for (int i = 0; i < L->top; i++)
    {
        ast_RemoveTvaluePoint(L->L, &L->Value[i]);
    }
    free(L);
    return TRUE;
}
ast_Bool ast_StackPushValue(ast_Stack *L, ast_Integer idx)
{
    TValue val = ast_StackGetTValue(L, idx);
    ast_StackPush(L, val);
    return TRUE;
}