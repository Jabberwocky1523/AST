#include "stringLib.h"
#include "../astMath.h"
#include "../astString.h"
#include "../log.h"
#include "sstream"
ast_Integer OpenStringLibs(ast_State *L)
{
    NewLib(L, stdstr);
    ast_CreateTableAndPush(L, 0, 1);
    TValue dummy = Char2Ob(L, "dummy");
    ast_StackPush(PStack(L), dummy);
    TValue mt = ast_StackGetTValue(PStack(L), -2);
    ast_StackPush(PStack(L), mt);
    ast_SetMetaTableFromIdx(L, -2);
    ast_StackPop(PStack(L));
    astString_RemoveStr(L, "dummy");
    ast_StackPushValue(PStack(L), -2);
    ast_SetTableFromString(L, -2, Char2Ob(L, "__index"));
    ast_StackPop(PStack(L));
    return 1;
}
ast_Integer len(ast_State *L)
{
    CheckType(L, 0, AST_TSTRING);
    ast_Len(L, 0);
    return 1;
}
ast_Integer sub(ast_State *L)
{
    CheckType(L, 0, AST_TSTRING);
    CheckAny(L, 1);
    CheckAny(L, 2);
    ast_StackSetTop(PStack(L), 3);
    TValue len = ast_StackPop(PStack(L));
    TValue begin = ast_StackPop(PStack(L));
    if (begin.value.i == 0)
    {
        return 1;
    }
    TValue str = ast_StackGetTValue(PStack(L), -1);
    String s = getstr(&str.value.gc->ts);
    s = s.substr(begin.value.i - 1, len.value.i);
    ast_StackPush(PStack(L), Char2Ob(L, s.c_str()));
    return 1;
}
ast_Integer reverse(ast_State *L)
{
    CheckType(L, -1, AST_TSTRING);
    ast_StackSetTop(PStack(L), 1);
    TValue str = ast_StackGetTValue(PStack(L), -1);
    String tmp = getstr(&str.value.gc->ts);
    std::reverse(tmp.begin(), tmp.end());
    ast_StackPush(PStack(L), Char2Ob(L, tmp.c_str()));
    return 1;
}
ast_Integer lower(ast_State *L)
{
    CheckType(L, -1, AST_TSTRING);
    ast_StackSetTop(PStack(L), 1);
    TValue str = ast_StackGetTValue(PStack(L), -1);
    String tmp = getstr(&str.value.gc->ts);
    std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
    ast_StackPush(PStack(L), Char2Ob(L, tmp.c_str()));
    return 1;
}
ast_Integer upper(ast_State *L)
{
    CheckType(L, -1, AST_TSTRING);
    ast_StackSetTop(PStack(L), 1);
    TValue str = ast_StackGetTValue(PStack(L), -1);
    String tmp = getstr(&str.value.gc->ts);
    std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
    ast_StackPush(PStack(L), Char2Ob(L, tmp.c_str()));
    return 1;
}
String _format(String str, Vector<TValue> val)
{
    int i = 0;
    std::ostringstream oss;
    while (str.find("{}") != str.end() && i < val.size())
    {
        switch (val[i].tt)
        {
        case AST_TBOOLEAN:
        {
            if (val[i].value.bo == TRUE)
            {
                oss << str.substr(0, str.find("{}") - str.begin()) << "true";
            }
            else
            {
                oss << str.substr(0, str.find("{}") - str.begin()) << "false";
            }
            break;
        }
        case AST_TINTEGER:
        {
            oss << str.substr(0, str.find("{}") - str.begin()) << val[i].value.i;
            break;
        }
        case AST_TNUMBER:
        {
            oss << str.substr(0, str.find("{}") - str.begin()) << val[i].value.n;
            break;
        }
        case AST_TSTRING:
        {
            oss << str.substr(0, str.find("{}") - str.begin()) << getstr(&val[i].value.gc->ts);
            break;
        }
        default:
            PANIC("connot format!");
        }
        str = str.substr(str.find("{}") - str.begin() + 2, str.length());
        i++;
    }
    oss << str;
    return oss.str().c_str();
}
ast_Integer format(ast_State *L)
{
    CheckType(L, 0, AST_TSTRING);
    ast_Integer num = L->stack->top;
    TValue str = ast_StackGetTValue(PStack(L), 0);
    String tmp = getstr(&str.value.gc->ts);
    Vector<TValue> vals;
    for (int i = 1; i < num; i++)
    {
        vals.push_back(ast_StackGetTValue(PStack(L), i));
    }
    tmp = _format(tmp, vals);
    ast_StackPush(PStack(L), Char2Ob(L, tmp.c_str()));
    return 1;
}
ast_Integer find(ast_State *L)
{
    CheckType(L, 0, AST_TSTRING);
    CheckType(L, 1, AST_TSTRING);
    TValue str = ast_StackGetTValue(PStack(L), 0);
    String tmp = getstr(&str.value.gc->ts);
    TValue p = ast_StackGetTValue(PStack(L), 1);
    String pattern = getstr(&p.value.gc->ts);
    String::Iterator it = tmp.find(pattern);
    ast_Integer begin = it - tmp.begin() + 1;
    if (it == tmp.end())
    {
        ast_StackPush(PStack(L), Nil2Ob());
        ast_StackPush(PStack(L), Nil2Ob());
        return 2;
    }
    ast_StackPush(PStack(L), Int2Ob(begin));
    ast_StackPush(PStack(L), Int2Ob(begin + pattern.size()));
    return 2;
}
String _gsub(String tmp, String src, String des)
{
    std::ostringstream oss;
    while (tmp.find(src) != tmp.end())
    {
        oss << tmp.substr(0, tmp.find(src) - tmp.begin()) << des;
        tmp = tmp.substr(tmp.find(src) - tmp.begin() + des.length(), tmp.length());
    }
    oss << tmp;
    return oss.str().c_str();
}
ast_Integer gsub(ast_State *L)
{
    CheckType(L, 0, AST_TSTRING);
    CheckType(L, 1, AST_TSTRING);
    CheckType(L, 2, AST_TSTRING);
    TValue str = ast_StackGetTValue(PStack(L), 0);
    String tmp = getstr(&str.value.gc->ts);
    str = ast_StackGetTValue(PStack(L), 1);
    String src = getstr(&str.value.gc->ts);
    str = ast_StackGetTValue(PStack(L), 2);
    String des = getstr(&str.value.gc->ts);
    tmp = _gsub(tmp, src, des);
    ast_StackPush(PStack(L), Char2Ob(L, tmp.c_str()));
    return 1;
}