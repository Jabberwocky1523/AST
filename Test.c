#include "stdlib.h"
#include "stdio.h"
#include "astState.h"
#include "astMem.h"
#include "astString.h"
int main()
{
    ast_State *L = (ast_State *)malloc(sizeof(ast_State));
    global_State *g_s = (global_State *)malloc(sizeof(global_State));
    g_s->StringBuff = realloc(NULL, 1024);
    StringTable ts;
    GCObject **hashtable;
    hashtable = (GCObject **)(malloc(sizeof(GCObject *) * 16));
    for (int i = 0; i < 16; i++)
    {
        *(hashtable + i) = (GCObject *)malloc(sizeof(GCObject));
    }
    ts.hashTable = hashtable;
    ts.Tnum = 0;
    ts.size = 16;
    g_s->stringtable = ts;
    L->G_S = g_s;
    ast_String *td = astString_NewLStr(L, "123", 3);
    const char *a = getstr(td);
    printf("%s %d", a, &a);
    ast_String *td2 = astString_NewLStr(L, "123", 3);
    const char *b = getstr(td2);
    printf("\n%s %d", b, &b);
    printf("\nOK");
}