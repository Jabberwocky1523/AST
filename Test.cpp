#include "stdlib.h"
#include "stdio.h"
#include "astState.h"
#include "astMem.h"
#include "astString.h"
int main()
{
    // ast_State *L = (ast_State *)malloc(sizeof(ast_State));
    // global_State *g_s = (global_State *)malloc(sizeof(global_State));
    // ast_Init(L, g_s);
    // ast_String *td = astString_NewLStr(L, "123", 3);
    // const char *a = getstr(td);
    // printf("%s %d", a, &a);
    // ast_String *td2 = astString_NewLStr(L, "123", 3);
    // const char *b = getstr(td2);
    // printf("\n%s %d", b, &b);
    // printf("\nOK");
    ast_Stack *L = ast_NewStack(8);
    ast_StackCheck(L, 4);
}