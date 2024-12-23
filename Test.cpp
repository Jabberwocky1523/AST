#include "stdlib.h"
#include "stdio.h"
#include "astState.h"
#include "astMem.h"
#include "astString.h"
#include "astStack.h"
int main()
{
    ast_State *L = (ast_State *)malloc(sizeof(ast_State));
    global_State *g_s = (global_State *)malloc(sizeof(global_State));
    ast_Init(L, g_s);
    printf("%d\n", L->G_S->stringtable.size);
    for (int i = 0; i < 17; i++)
    {
        char *tmp = (char *)ast_Malloc(L, 32);
        sprintf(tmp, "%f", (double)i);

        ast_String *st = astString_NewLStr(L, tmp, strlen(tmp));
        printf("%s\n", getstr(st));
    }
    printf("%d\n", L->G_S->stringtable.size);
    // for (int i = 0; i < 5; i++)
    // {
    //     Ta.value.n = i + 1;
    //     ast_StackPush(L, Ta);
    //     printf("%f ", Ta.value.n);
    // }
    // ast_StackPush(PStack(L), Tb);
    // double a = 6;
    // ast_StackPush(PStack(L), &a, AST_TNUMBER);
    // ast_StackPush(PStack(L), NULL, AST_TNIL);
    // ast_StackPush(PStack(L), FALSE, AST_TBOOLEAN);
    // char *val = "2345";
    // ast_StackPush(L, val, AST_TSTRING);
    // ast_StackPush(L, "12321323r", AST_TSTRING);
    // ast_PrintStack(PStack(L));
    // astack_ToString(L, 1);
    // ast_PrintStack(PStack(L));
    // printf("%s\n", getstr(&L->stack->Value[1].value.gc->ts));
    // ast_Number b = astack_ToNumber(PStack(L), 5, nullptr);
    // printf("%f", b);
}