#include "astState.h"
#include "ast.h"
#include "astString.h"
#include "astObject.h"
ast_Bool ast_Init(ast_State *L, global_State *g_s)
{
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
    return TRUE;
}
// 创建虚拟栈
ast_State *ast_NewState(ast_Alloc func, void *ud)
{
    ast_State *L;
    global_State *g;
}