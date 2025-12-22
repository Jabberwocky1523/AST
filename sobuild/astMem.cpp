#include "astMem.h"
void *astM_Realloc(ast_State *Local, void *block, size_t oldsize, size_t nsize)
{
    global_State *g = Local->G_S;
    block = realloc(block, nsize);
    g->totalbytes = g->totalbytes + (nsize - oldsize);
    return block;
}