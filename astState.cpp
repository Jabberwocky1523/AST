#include "astState.h"
#include "ast.h"
#include "astString.h"
#include "astStack.h"
#include "astObject.h"
ast_Bool ast_Init(ast_State *L, global_State *g_s, Prototype *proto, int pc)
{
    int nRegs = 0;
    if (proto != NULL)
        nRegs = proto->MaxStackSize;
    ast_Stack *Stack = ast_NewStack(nRegs + 8);
    g_s->StringBuff = realloc(NULL, 1024);
    StringTable ts;
    GCObject **hashtable;
    hashtable = (GCObject **)(malloc(sizeof(GCObject *) * 16));
    for (int i = 0; i < 16; i++)
    {
        *(hashtable + i) = NULL;
    }
    ts.hashTable = hashtable;
    ts.Tnum = 0;
    ts.size = 16;
    g_s->stringtable = ts;
    L->G_S = g_s;
    L->stack = Stack;
    L->pc = pc;
    L->proto = proto;
    return TRUE;
}