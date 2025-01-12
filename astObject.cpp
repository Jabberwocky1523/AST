#include "astObject.h"
#include "string.h"
#include "ast.h"
#include "astString.h"
#include "astState.h"
#include "astStack.h"
#include "astTable.h"
#include <functional>
ast_Bool ast_TValueCmp(TValue val1, TValue val2)
{
    if (IsNum(val1) && IsNum(val2))
    {
        return (ast_Bool)(ast_ConvertToNumber(val1) == ast_ConvertToNumber(val2));
    }
    if ((val1.tt != val2.tt))
    {
        return FALSE;
    }
    switch (val1.tt)
    {
    case AST_TNIL:
        return (ast_Bool)(val1.tt == val2.tt);
    case AST_TBOOLEAN:
        return (ast_Bool)(val1.value.bo == val2.value.bo);
    case AST_TSTRING:
    {
        return (ast_Bool)(val1.value.gc == val2.value.gc);
    }
    case AST_TTABLE:
        return (ast_Bool)(val1.value.gc == val2.value.gc);
    default:
        PANIC("有不支持比较的类型");
    }
}
ast_Integer ast_GetTValueHash(TValue val1)
{
    if (val1.tt == AST_TNIL)
    {
        return 0;
    }
    switch (val1.tt)
    {
    case AST_TBOOLEAN:
        return std::hash<ast_Bool>()(val1.value.bo);
    case AST_TINTEGER:
        return std::hash<long long>()(val1.value.i);
    case AST_TNUMBER:
        return std::hash<long long>()((ast_Integer)val1.value.n);
    case AST_TSTRING:
    case AST_TFUNCTION:
    case AST_TUSERFUNCTION:
    case AST_TTABLE:
        return std::hash<GCObject *>()(val1.value.gc);
    }
    return 0;
}
TValue *ast_NewClosure(int size)
{
    TValue *tt;
    tt->tt = AST_TFUNCTION;
    Prototype *proto = (Prototype *)malloc(sizeof(Prototype) * size);
    tt->value.gc = (GCObject *)proto;
    return tt;
}
ast_Bool ast_FreeTvaluePoint(TValue *t)
{
    switch (t->tt)
    {
    case AST_TTABLE:
        ast_FreeTable(&t->value.gc->tb);
        break;
    }
    return TRUE;
}