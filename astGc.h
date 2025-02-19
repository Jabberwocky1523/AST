#ifndef AST_GC_H
#define AST_GC_H
#include "astState.h"
enum
{
    GCPAUSE = 0,
    GCRUN
};
ast_Bool ast_LinkGc(ast_State *L, GCObject *gc);
int ast_YieldGc(ast_State *L);
GCObject *ast_RemoveGc(ast_State *L, GCObject *prev, GCObject *cur);
#endif