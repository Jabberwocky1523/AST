#include "astMath.h"
#include "stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include "astStack.h"
#include "astString.h"
#include "string.h"
ast_Operator g_operators[14] = {
    {ast_IntegerAdd, ast_NumberAdd},
    {ast_IntegerSub, ast_NumberSub},
    {ast_IntegerMul, ast_NumberMul},
    {ast_IntegerMod, ast_NumberMod},
    {NULL, ast_NumberPow},
    {NULL, ast_NumberDiv},
    {ast_IFloorDiv, ast_FFloorDiv},
    {ast_Band, NULL},
    {ast_Bor, NULL},
    {ast_Bxor, NULL},
    {ast_Shl, NULL},
    {ast_Shr, NULL},
    {ast_IntegerUnm, ast_NumberUnm},
    {ast_Bnot, NULL}};
int ast_IsNumeric(const char *str)
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
        return 1;
    }
    else if (reti == REG_NOMATCH)
    {
        regfree(&regex);
        return 0;
    }
    else
    {
        fprintf(stderr, "Regex match failed: %d\n", reti);
        regfree(&regex);
        exit(1);
    }

    regfree(&regex);
    return 0;
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
ast_Integer ast_DoubleToInteger(double src, int *flag)
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
    return ddivl(a, b);
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
    ast_Operator ope = g_operators[op];
    TValue result = _ast_Arith(a, b, ope);
    if (result.tt != AST_TNIL)
    {
        ast_StackPush(L, result);
    }
    else
    {
        PANIC("Arith Error");
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
    default:
        PANIC("长度计算错误，不是字符串或表");
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
            }
            else
            {
                break;
            }
        }
    }
    return TRUE;
}
