#include "astGc.h"
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
    }
    prev->gch.next = cur->gch.next;
    return cur->gch.next;
}
int ast_YieldGc(ast_State *L)
{
    int mark = L->G_S->gcmarked;
    L->G_S->gcmarked = GCRUN;
    return mark;
}