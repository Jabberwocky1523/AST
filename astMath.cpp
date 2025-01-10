#include "astMath.h"
#include "stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include "astStack.h"
#include "astString.h"
#include "string.h"
#include "astTable.h"
#include "astOpcode.h"
#include "log.h"
ast_Operator g_Arith_operators[14] = {
    {"__add", ast_IntegerAdd, ast_NumberAdd},
    {"__sub", ast_IntegerSub, ast_NumberSub},
    {"__mul", ast_IntegerMul, ast_NumberMul},
    {"__mod", ast_IntegerMod, ast_NumberMod},
    {"__pow", NULL, ast_NumberPow},
    {"__div", NULL, ast_NumberDiv},
    {"__idiv", ast_IFloorDiv, ast_FFloorDiv},
    {"__band", ast_Band, NULL},
    {"__bor", ast_Bor, NULL},
    {"__bxor", ast_Bxor, NULL},
    {"__shl", ast_Shl, NULL},
    {"shr", ast_Shr, NULL},
    {"__unm", ast_IntegerUnm, ast_NumberUnm},
    {"__bnot", ast_Bnot, NULL}};
ast_Bool ast_IsNumeric(const char *str)
{
    regex_t regex;
    int reti;

    reti = regcomp(&regex, "^[+-]?[0-9]+(\\.[0-9]+)?$|^0x[0-9a-fA-F]+$|^0[0-7]+$|^-?[0-9]+$", REG_EXTENDED);
    if (reti)
    {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

    reti = regexec(&regex, str, 0, NULL, 0);
    if (!reti)
    {
        regfree(&regex);
        return TRUE;
    }
    else if (reti == REG_NOMATCH)
    {
        regfree(&regex);
        return FALSE;
    }
    else
    {
        fprintf(stderr, "Regex match failed: %d\n", reti);
        regfree(&regex);
        exit(1);
    }

    regfree(&regex);
    return FALSE;
}
ast_Integer ast_IFloorDiv(ast_Integer a, ast_Integer b)
{
    if (a > 0 && b > 0 || a < 0 && b < 0 || a % b == 0)
    {
        return a / b;
    }
    else
    {
        return a / b - 1;
    }
}
ast_Number ast_FFloorDiv(ast_Number a, ast_Number b)
{
    return ast_IFloorDiv(a, b);
}
ast_Integer ast_DoubleToInteger(ast_Number src, ast_Integer *flag)
{
    ast_Number tmp = src;
    src = (ast_Integer)src;
    ast_Integer ans = src;
    src = (ast_Number)src;
    if (src == tmp)
    {
        *flag = 1;
    }
    return ans;
}
ast_Integer ast_StringToInteger(const char *str)
{
    if (!ast_IsNumeric(str))
    {
        PANIC("字符串包含非数字");
    }
    return atoll(str);
}
ast_Number ast_StringToDouble(const char *str)
{
    if (!ast_IsNumeric(str))
    {
        PANIC("字符串包含非数字");
    }
    return atof(str);
}
ast_Integer ast_IntegerAdd(ast_Integer a, ast_Integer b)
{
    return a + b;
}
ast_Number ast_NumberAdd(ast_Number a, ast_Number b)
{
    return a + b;
}
ast_Integer ast_IntegerSub(ast_Integer a, ast_Integer b)
{
    return a - b;
}
ast_Number ast_NumberSub(ast_Number a, ast_Number b)
{
    return a - b;
}
ast_Integer ast_IntegerMul(ast_Integer a, ast_Integer b)
{
    return a * b;
}
ast_Number ast_NumberMul(ast_Number a, ast_Number b)
{
    return a * b;
}
ast_Integer ast_IntegerMod(ast_Integer a, ast_Integer b)
{
    return a % b;
}
ast_Number ast_NumberMod(ast_Number a, ast_Number b)
{
    return fmod(a, b);
}
ast_Number ast_NumberDiv(ast_Number a, ast_Number b)
{
#ifdef __APPLE__
    return a / b;
#endif
#ifdef __linux__
    return ddivl(a, b);
#endif
}
ast_Number ast_NumberPow(ast_Number a, ast_Number b)
{
    return pow(a, b);
}
ast_Integer ast_Band(ast_Integer a, ast_Integer b)
{
    return a & b;
}
ast_Integer ast_Bor(ast_Integer a, ast_Integer b)
{
    return a | b;
}
ast_Integer ast_Bxor(ast_Integer a, ast_Integer b)
{
    return a ^ b;
}
ast_Integer ast_Shl(ast_Integer a, ast_Integer b)
{
    return a << b;
}
ast_Integer ast_Shr(ast_Integer a, ast_Integer b)
{
    return a >> b;
}
ast_Integer ast_IntegerUnm(ast_Integer a, ast_Integer nil)
{
    return -a;
}
ast_Number ast_NumberUnm(ast_Number a, ast_Number nil)
{
    return -a;
}
ast_Integer ast_Bnot(ast_Integer a, ast_Integer nil)
{
    return !a;
}
const char *ast_CTS(ast_State *L, TValue val)
{
    switch (val.tt)
    {
    case AST_TBOOLEAN:
    {
        ast_Bool num = val.value.bo;
        char *tmp = (char *)malloc(sizeof(char) * 2);
        sprintf(tmp, "%d", num);
        ast_String *st = astString_NewLStr(L, tmp, strlen(tmp));
        return getstr(st);
    }
    case AST_TSTRING:
        return getstr(&val.value.gc->ts);
    case AST_TNUMBER:
    {
        ast_Number num = val.value.n;
        char *tmp = (char *)malloc(sizeof(char) * 32);
        sprintf(tmp, "%f", num);
        val.tt = AST_TSTRING;
        val.value.gc = (GCObject *)astString_NewLStr(L, tmp, strlen(tmp));
        return getstr(&val.value.gc->ts);
    }
    case AST_TINTEGER:
    {
        ast_Integer num = val.value.i;
        char *tmp = (char *)malloc(sizeof(char) * 32);
        sprintf(tmp, "%lld", num);
        val.tt = AST_TSTRING;
        val.value.gc = (GCObject *)astString_NewLStr(L, tmp, sizeof(tmp) / sizeof(tmp[0]));
        return getstr(&val.value.gc->ts);
    }
    default:
    {
        return "";
    }
    }
}
TValue _ast_Arith(TValue a, TValue b, ast_Operator op)
{
    TValue tt;
    if (op.DoubleFunc == NULL)
    {

        tt.tt = AST_TINTEGER;
        tt.value.i = op.IntegerFunc(ast_ConvertToInteger(a), ast_ConvertToInteger(b));
        return tt;
    }
    else if (op.IntegerFunc == NULL)
    {
        tt.tt = AST_TNUMBER;
        tt.value.n = op.DoubleFunc(ast_ConvertToNumber(a), ast_ConvertToNumber(b));
        return tt;
    }
    else
    {
        if (a.tt == AST_TINTEGER && b.tt == AST_TINTEGER)
        {
            tt.tt = AST_TINTEGER;
            tt.value.i = op.IntegerFunc(ast_ConvertToInteger(a), ast_ConvertToInteger(b));
            return tt;
        }
        else
        {
            tt.tt = AST_TNUMBER;
            tt.value.n = op.DoubleFunc(ast_ConvertToNumber(a), ast_ConvertToNumber(b));
            return tt;
        }
    }
    tt.tt = AST_TNIL;
    tt.value.gc = NULL;
    return tt;
}
ast_Bool ast_CallMetaMethod(ast_State *L, TValue a, TValue b, TValue str)
{
    TValue m1 = ast_GetMetaField(L, a, str);
    if (m1.tt == AST_TNIL)
    {
        m1 = ast_GetMetaField(L, b, str);
        if (m1.tt == AST_TNIL)
        {
            return FALSE;
        }
    }
    ast_StackCheck(PStack(L), 4);
    ast_StackPush(PStack(L), m1);
    ast_StackPush(PStack(L), a);
    ast_StackPush(PStack(L), b);
    ast_Call(L, 2, 1);
    return TRUE;
}
ast_Bool ast_Arith(ast_State *S, int op)
{
    ast_Stack *L = S->stack;
    TValue a, b;
    b = astack_Pop(L);
    if (op != AST_OPUNM && op != AST_OPBNOT)
    {
        a = astack_Pop(L);
    }
    else
    {
        a = b;
    }
    ast_Operator ope = g_Arith_operators[op];
    TValue result = _ast_Arith(a, b, ope);
    if (result.tt != AST_TNIL)
    {
        ast_StackPush(L, result);
        return TRUE;
    }
    char *mn = ope.MetaMethod;
    ast_Bool ok = ast_CallMetaMethod(S, a, b, Char2Ob(S, mn));
    if (ok == FALSE)
    {
        PANIC("arith error");
    }
    return TRUE;
}
int _ast_Eq(ast_State *L, TValue a, TValue b)
{
    if (a.tt == AST_TNIL ^ b.tt == AST_TNIL)
    {
        return 0;
    }
    switch (a.tt)
    {
    case AST_TNIL:
        return b.tt == AST_TNIL;
    case AST_TBOOLEAN:
    {
        return a.value.bo == ast_ConvertToBoolean(b);
    }
    case AST_TSTRING:
    {

        if (strcmp(getstr(&a.value.gc->ts), ast_CTS(L, b)) == 0)
        {
            return 1;
        }
        return 0;
    }
    case AST_TINTEGER:
    {
        switch (b.tt)
        {
        case AST_TINTEGER:
            return a.value.i == b.value.i;
        case AST_TNUMBER:
            return ((ast_Number)a.value.i) == b.value.n;
        default:
            return (ast_Number)a.value.i == ast_ConvertToNumber(b);
        }
    }
    case AST_TNUMBER:
    {
        return a.value.n == ast_ConvertToNumber(b);
    }
    case AST_TTABLE:
    {
        if (b.tt == AST_TTABLE && L != nullptr)
        {
            ast_Bool ok = ast_CallMetaMethod(L, a, b, Char2Ob(L, "__eq"));
            if (ok == TRUE)
            {
                TValue res = ast_StackPop(PStack(L));
                return ast_ConvertToBoolean(res);
            }
        }
        return 0;
    }
    default:
        return 0;
    }
}
int _ast_Lt(ast_State *L, TValue a, TValue b)
{
    switch (a.tt)
    {
    case AST_TSTRING:
        switch (b.tt)
        {
        case AST_TINTEGER:
        case AST_TNUMBER:
        case AST_TSTRING:
            if (strcmp(getstr(&a.value.gc->ts), ast_CTS(L, b)) < 0)
            {
                return 1;
            }
            return 0;
        }
        break;
    case AST_TINTEGER:
    {
        switch (b.tt)
        {
        case AST_TINTEGER:
            return a.value.i < b.value.i;
        case AST_TNUMBER:
            return (ast_Number)a.value.i < b.value.n;
        case AST_TSTRING:
            if (strcmp(ast_CTS(L, a), getstr(&b.value.gc->ts)) < 0)
            {
                return 1;
            }
            return 0;
        }
    }
    break;
    case AST_TNUMBER:
    {
        switch (b.tt)
        {
        case AST_TINTEGER:
            return a.value.n < (ast_Number)b.value.i;
        case AST_TNUMBER:
            return a.value.n < b.value.n;
        case AST_TSTRING:
            if (strcmp(ast_CTS(L, a), getstr(&b.value.gc->ts)) < 0)
            {
                return 1;
            }
            return 0;
        }
    }
    break;
    }
    ast_Bool ok = ast_CallMetaMethod(L, a, b, Char2Ob(L, "__lt"));
    if (ok == TRUE)
    {
        TValue result = astack_Pop(PStack(L));
        return ast_ConvertToBoolean(result);
    }
    PANIC("无效的比较指令");
}

int _ast_Le(ast_State *L, TValue a, TValue b)
{
    switch (a.tt)
    {
    case AST_TSTRING:
        switch (b.tt)
        {
        case AST_TINTEGER:
        case AST_TNUMBER:
        case AST_TSTRING:
            if (strcmp(getstr(&a.value.gc->ts), ast_CTS(L, b)) <= 0)
            {
                return 1;
            }
            return 0;
        }
        break;
    case AST_TINTEGER:
    {
        switch (b.tt)
        {
        case AST_TINTEGER:
            return a.value.i <= b.value.i;
        case AST_TNUMBER:
            return (ast_Number)a.value.i <= b.value.n;
        case AST_TSTRING:
            if (strcmp(ast_CTS(L, a), getstr(&b.value.gc->ts)) <= 0)
            {
                return 1;
            }
            return 0;
        }
    }
    break;
    case AST_TNUMBER:
    {
        switch (b.tt)
        {
        case AST_TINTEGER:
            return a.value.n <= (ast_Number)b.value.i;
        case AST_TNUMBER:
            return a.value.n <= b.value.n;
        case AST_TSTRING:
            if (strcmp(ast_CTS(L, a), getstr(&b.value.gc->ts)) <= 0)
            {
                return 1;
            }
            return 0;
        }
    }
    break;
    }
    ast_Bool ok = ast_CallMetaMethod(L, a, b, Char2Ob(L, "__le"));
    if (ok == TRUE)
    {
        TValue result = astack_Pop(PStack(L));
        return ast_ConvertToBoolean(result);
    }
    ok = ast_CallMetaMethod(L, b, a, Char2Ob(L, "__lt"));
    if (ok == TRUE)
    {
        TValue result = astack_Pop(PStack(L));
        return !ast_ConvertToBoolean(result);
    }
    PANIC("比较错误");
}
ast_Bool ast_Compare(ast_State *L, int idx1, int idx2, int op)
{
    TValue a = ast_StackGetTValue(PStack(L), idx1);
    TValue b = ast_StackGetTValue(PStack(L), idx2);
    switch (op)
    {
    case AST_OPEQ:
        return (ast_Bool)_ast_Eq(L, a, b);
    case AST_OPLT:
        return (ast_Bool)_ast_Lt(L, a, b);
    case AST_OPLE:
        return (ast_Bool)_ast_Le(L, a, b);
    default:
        PANIC("无效比较指令");
    }
    return TRUE;
}
ast_Bool ast_Len(ast_State *L, int idx)
{
    TValue tmp = ast_StackGetTValue(PStack(L), idx);
    switch (tmp.tt)
    {
    case AST_TSTRING:
    {
        ast_Integer len = tmp.value.gc->ts.Tsv.len;
        ast_StackPush(PStack(L), &len, AST_TINTEGER);
        return TRUE;
    }
    case AST_TTABLE:
    {
        ast_Integer len = TableArrLen(&tmp.value.gc->tb);
        ast_StackPush(PStack(L), &len, AST_TINTEGER);
        return TRUE;
    }
    default:
    {
        ast_Bool bo = ast_CallMetaMethod(L, tmp, tmp, Char2Ob(L, "__len"));
        if (bo == FALSE)
        {
            PANIC("长度计算有误！");
        }
        return TRUE;
    }
    }
    return TRUE;
}
ast_Bool ast_Concat(ast_State *L, int n)
{
    if (n == 0)
    {
        ast_StackPush(L, "", AST_TSTRING);
    }
    else if (n >= 2)
    {
        for (int i = 1; i < n; i++)
        {
            if (ast_IsString(ast_StackGetTValue(PStack(L), -1)) && ast_IsString(ast_StackGetTValue(PStack(L), -2)))
            {
                ast_ConvertToString(L, PStack(L)->Value[ast_StackAbsIndex(PStack(L), -1)]);
                ast_ConvertToString(L, PStack(L)->Value[ast_StackAbsIndex(PStack(L), -2)]);
                TValue t1 = astack_Pop(PStack(L));
                TValue t2 = astack_Pop(PStack(L));
                char *a = (char *)malloc(sizeof(char) * (t1.value.gc->ts.Tsv.len + t2.value.gc->ts.Tsv.len) + 1);
                memcpy(a, getstr(&t2.value.gc->ts), t2.value.gc->ts.Tsv.len);
                memcpy(a + t2.value.gc->ts.Tsv.len, getstr(&t1.value.gc->ts), t1.value.gc->ts.Tsv.len);
                memcpy(a + t2.value.gc->ts.Tsv.len + t1.value.gc->ts.Tsv.len, "\0", 1);
                ast_String *s = astString_NewLStr(L, a, strlen(a));
                TValue tt;
                tt.value.gc = (GCObject *)s;
                tt.tt = AST_TSTRING;
                ast_StackPush(PStack(L), tt);
                free(a);
            }
            else
            {
                TValue a = astack_Pop(PStack(L));
                TValue b = astack_Pop(PStack(L));
                ast_Bool ok = ast_CallMetaMethod(L, a, b, Char2Ob(L, "__concat"));
                if (ok == FALSE)
                {
                    PANIC("concat error");
                }
            }
        }
    }
    return TRUE;
}
int IntToFb(int x)
{
    int e = 0;
    if (x < 8)
    {
        return x;
    }
    for (; x >= (8 << 4);)
    {
        x = (x + 0xf) >> 4;
        e += 4;
    }
    for (; x >= (8 << 1);)
    {
        x = (x + 1) >> 1;
        e++;
    }
    return ((e + 1) << 3) | (x - 8);
}
int FbToInt(int x)
{
    if (x < 8)
    {
        return x;
    }
    else
    {
        return ((x & 7) + 8) << uint((x >> 3) - 1);
    }
}