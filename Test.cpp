#include "stdlib.h"
#include "stdio.h"
#include "astState.h"
#include "astMem.h"
#include "astString.h"
#include "threads.h"
#include "astStack.h"
int main()
{
    ast_Init(S, g_s);
    ast_Stack *L = ast_NewStack(20);
    ast_StackCheck(L, 4);
    TValue Ta;
    Ta.tt = AST_TNUMBER;
    Ta.value.n = 1;
    TValue Tb;
    Tb.tt = AST_TSTRING;
    ast_String *td = astString_NewLStr(S, "123", 3);
    Tb.value.gc = (GCObject *)td;
    // for (int i = 0; i < 5; i++)
    // {
    //     Ta.value.n = i + 1;
    //     ast_StackPush(L, Ta);
    //     printf("%f ", Ta.value.n);
    // }
    ast_StackPush(L, Tb);
    double a = 6;
    ast_StackPush(L, &a, AST_TNUMBER);
    ast_StackPush(L, NULL, AST_TNIL);
    ast_StackPush(L, FALSE, AST_TBOOLEAN);
    char *val = "2345";
    printf("%d ", val);
    ast_StackPush(L, (void *)val, AST_TSTRING);
    ast_PrintStack(L);
}