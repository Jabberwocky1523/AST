#include "stdlib.h"
#include "stdio.h"
#include "astState.h"
#include "astMem.h"
#include "astString.h"
int main()
{
    ast_State *S = (ast_State *)malloc(sizeof(ast_State));
    global_State *g_s = (global_State *)malloc(sizeof(global_State));
    ast_Init(S, g_s);
    // ast_String *td = astString_NewLStr(L, "123", 3);
    // const char *a = getstr(td);
    // printf("%s %d", a, &a);
    // ast_String *td2 = astString_NewLStr(L, "123", 3);
    // const char *b = getstr(td2);
    // printf("\n%s %d", b, &b);
    // printf("\nOK");
    ast_Stack *L = ast_NewStack(8);
    ast_StackCheck(L, 4);
    TValue Ta;
    Ta.tt = AST_TBOOLEAN;
    Ta.value.bo = FALSE;
    TValue Tb;
    Tb.tt = AST_TSTRING;
    ast_String *td = astString_NewLStr(S, "123", 3);
    Tb.value.gc = (GCObject *)td;
    ast_StackPush(L, Ta);
    ast_StackPush(L, Tb);
    ast_StackPush(L, Tb);
    printf("%d %d\n", L->size, L->top);
    TValue *temp = (TValue *)malloc(sizeof(TValue));
    *temp = ast_StackPop(L);
    *temp = ast_StackGetTValue(L, -1);
    printf("%d\n", temp->tt);
    printf("%d", ast_StackIdxIsValid(L, 1));
    ast_StackSetTValue(L, Tb, 8);
    *temp = ast_StackGetTValue(L, 0);
    printf("%d %s\n", temp->tt, getstr((ast_String *)temp->value.gc));
}