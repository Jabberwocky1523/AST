#include "astState.h"
#include "ast.h"
#include "astString.h"
#include "astStack.h"
#include "astObject.h"
#include "string.h"
#include "log.h"
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
TValue ast_ObjectToTValue(ast_State *L, void *ob, ast_Type type, int flag)
{
    TValue tt;
    switch (type)
    {
    case AST_TNIL:
    {
        tt.tt = AST_TNIL;
        tt.value.gc = nullptr;
        break;
    }
    case AST_TBOOLEAN:
    {
        tt.tt = AST_TBOOLEAN;
        tt.value.bo = *(ast_Bool *)ob;
        break;
    }
    case AST_TINTEGER:
    {
        tt.tt = AST_TINTEGER;
        tt.value.i = *(ast_Integer *)ob;
        break;
    }
    case AST_TNUMBER:
    {
        tt.tt = AST_TNUMBER;
        tt.value.n = *(ast_Number *)ob;
        break;
    }
    case AST_TSTRING:
    {
        switch (flag)
        {
        case 0:
        {
            tt.tt = AST_TSTRING;
            tt.value.gc = (GCObject *)astString_NewLStr(L, cast(char *, ob), strlen(cast(char *, ob)));
            break;
        }
        case 1:
        {
            tt.tt = AST_TSTRING;
            tt.value.gc = (GCObject *)ob;
            break;
        }
        }
        break;
    }
    case AST_TTABLE:
    {
        tt.tt = AST_TTABLE;
        tt.value.gc = (GCObject *)ob;
        break;
    }
    }
    return tt;
}