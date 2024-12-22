#include "stdlib.h"
#include "stdio.h"
#include "astState.h"
#include "astMem.h"
#include "astString.h"
#include "threads.h"

int main()
{
    ast_Init(S, g_s);
    // ast_String *td = astString_NewLStr(L, "123", 3);
    // const char *a = getstr(td);
    // printf("%s %d", a, &a);
    // ast_String *td2 = astString_NewLStr(L, "123", 3);
    // const char *b = getstr(td2);
    // printf("\n%s %d", b, &b);
    // printf("\nOK");
    ast_Stack *L = ast_NewStack(20);
    ast_StackCheck(L, 4);
    TValue Ta;
    Ta.tt = AST_TNUMBER;
    Ta.value.n = 1;
    TValue Tb;
    Tb.tt = AST_TSTRING;
    ast_String *td = astString_NewLStr(S, "123", 3);
    Tb.value.gc = (GCObject *)td;
    for (int i = 0; i < 5; i++)
    {
        Ta.value.n = i + 1;
        ast_StackPush(L, Ta);
        printf("%f ", Ta.value.n);
    }
    printf("\n");
    ast_StackRotate(L, 0, 1);
}