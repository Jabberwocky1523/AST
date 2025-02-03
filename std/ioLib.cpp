#include "ioLib.h"
#include "../astString.h"
#include "iostream"
#include "string"
#include "../astUtils.h"
ast_Integer OpenIoLibs(ast_State *L)
{
    NewLib(L, stdio);
    ast_CreateTableAndPush(L, 0, 1);
    TValue dummy;
    dummy.tt = AST_TFILE;
    ast_StackPush(PStack(L), dummy);
    TValue mt = ast_StackGetTValue(PStack(L), -2);
    ast_StackPush(PStack(L), mt);
    ast_SetMetaTableFromIdx(L, -2);
    ast_StackPop(PStack(L));
    ast_StackPushValue(PStack(L), -2);
    ast_SetTableFromString(L, -2, Char2Ob(L, "__index"));
    ast_StackPop(PStack(L));
    return 1;
}
ast_Integer open(ast_State *L)
{
    CheckType(L, 0, AST_TSTRING);
    ast_StackSetTop(PStack(L), 1);
    TValue filepath = ast_StackPop(PStack(L));
    String s = getstr(&filepath.value.gc->ts);
    TValue f;
    f.tt = AST_TFILE;
    f.value.fh = fopen(s.c_str(), "a+");
    ast_StackPush(PStack(L), f);
    return 1;
}
ast_Integer read(ast_State *L)
{
    ast_Integer num = L->stack->top;
    if (num == 0)
    {
        String tmp;
        std::cin >> tmp;
        ast_StackPush(PStack(L), Char2Ob(L, tmp.c_str()));
        return 1;
    }
    CheckType(L, 0, AST_TFILE);
    TValue f = ast_StackGetTValue(PStack(L), 0);
    char *buffer;
    int len;
    OPENANDREAD(f.value.fh, buffer, len);
    ast_StackPush(PStack(L), Char2Ob(L, buffer));
    free(buffer);
    return 1;
}