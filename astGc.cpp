#include "astGc.h"
#include "astStack.h"
#include "astString.h"
ast_Type ast_FreeGcOb(ast_State *L, GCObject *ob)
{
    ast_Type type = ob->gch.tt;
    // if (ob->gch.tt >= AST_TSTRING)
    // {
    //     switch (ob->gch.tt)
    //     {
    //     case AST_TSTRING:
    //     {
    //         astString_Remove(L, &ob->ts);
    //         break;
    //     }
    //     default:
    //     {
    //         PANIC("can not free!");
    //     }
    //     }
    // }
    TValue tt;
    tt.tt = ob->gch.tt;
    tt.value.gc = ob;
    ast_RemoveTvalue(L, tt);
    return type;
}
ast_Bool ast_LinkGc(ast_State *L, GCObject *gc)
{
    global_State *gs = L->G_S;
    if (gs->gcmarked == GCPAUSE)
    {
        return FALSE;
    }
    gc->gch.next = gs->rootgc;
    gs->rootgc = gc;
    gs->gcnum++;
    return TRUE;
}
GCObject *ast_RemoveGc(ast_State *L, GCObject *prev, GCObject *cur)
{
    if (prev == cur)
    {
        L->G_S->rootgc = cur->gch.next;
        cur->gch.next = nullptr;
        ast_FreeGcOb(L, cur);
        L->G_S->gcnum--;
        return L->G_S->rootgc;
    }
    prev->gch.next = cur->gch.next;
    GCObject *tmp = cur->gch.next;
    cur->gch.next = nullptr;
    ast_FreeGcOb(L, cur);
    L->G_S->gcnum--;
    return tmp;
}
int ast_YieldGc(ast_State *L)
{
    int mark = L->G_S->gcmarked;
    L->G_S->gcmarked = GCRUN;
    return mark;
}
ast_Bool ast_RunGc(ast_State *L)
{
    global_State *gs = L->G_S;
    GCObject *cur = gs->rootgc;
    GCObject *prev = gs->rootgc;
    while (cur)
    {
        if (cur->gch.master == L->stack)
        {
            cur = ast_RemoveGc(L, prev, cur);
            break;
        }
        prev = cur;
        cur = cur->gch.next;
    }
    return TRUE;
}
ast_Bool ast_PrintGcList(ast_State *L)
{
    GCObject *cur = L->G_S->rootgc;
    while (cur)
    {
        TValue tt;
        tt.tt = cur->gch.tt;
        tt.value.gc = cur;
        ast_PrintTValue(tt);
        cur = cur->gch.next;
    }
    return TRUE;
}