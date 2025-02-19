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
ast_Bool ast_RunGc(ast_State *L);
ast_Bool ast_PrintGcList(ast_State *L);
#endif