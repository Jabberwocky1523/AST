#include "stdlib.h"
#include "stdio.h"
#include "astState.h"
#include "astMem.h"
#include "astString.h"
#include "astMath.h"
#include "astStack.h"
#include "astOpcode.h"
int main()
{
    ast_State *L = (ast_State *)malloc(sizeof(ast_State));
    global_State *g_s = (global_State *)malloc(sizeof(global_State));
    ast_Init(L, g_s);
    printf("%d %d\n", L->G_S->stringtable.size);
    ast_StackPush(PStack(L), (ast_Number)2.0, AST_TINTEGER);
    ast_StackPush(PStack(L), -3.1, AST_TNUMBER);

    // ast_StackPush(PStack(L), 122, AST_TINTEGER);
    // ast_StackPush(PStack(L), 5, AST_TNUMBER);
    // ast_Len(L, 0);
    ast_PrintStack(PStack(L));
    // ast_Arith(L, AST_OPADD);
    ast_StackPush(L, "123", AST_TSTRING);
    ast_StackPush(L, "123", AST_TSTRING);

    ast_PrintStack(PStack(L));
    printf("%d", ast_Compare(L, 2, 3, AST_OPLE));
    ast_Concat(L, 5);
    ast_PrintStack(PStack(L));
    printf("%s ", getstr(astString_FindStr(L, "123")));
    printf("%d", astString_RemoveStr(L, "123"));
    printf("%d ", (bool)astString_FindStr(L, "123"));
}