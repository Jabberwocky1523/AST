#include "astState.h"
#include "ast.h"
// 创建新线程的GS
ast_State *ast_NewState(ast_Alloc func, void *ud)
{
    ast_State *L;
    global_State *g;
    void *l = (*func)(ud, NULL, 0, sizeof(global_State));
    if (l == NULL)
    {
        return NULL;
    }
    L = (ast_State *)l;
}
