#include "astError.h"
#include "astStack.h"
#include <exception>
#include <stdexcept>
ast_Bool ast_processException(std::exception_ptr eptr)
{
    try
    {
        if (eptr)
        {
            std::rethrow_exception(eptr);
        }
    }
    catch (const std::exception &e)
    {
        printf("error:%s\n", e.what());
    }
    return TRUE;
}
ast_Integer ast_PCall(ast_State *L, ast_Integer nArgs, ast_Integer nResults)
{
    std::exception_ptr eptr;
    ast_Stack *caller = L->stack;
    ast_Integer status = AST_ERRRUN;
    try
    {
        ast_Call(L, nArgs, nResults);
    }
    catch (...)
    {
        if (caller != L->stack)
        {
            ast_PopStack(L);
        }
        // TValue fa = Bool2Ob(FALSE);
        // ast_StackPush(PStack(L), fa);
        ast_processException(eptr);
        return status;
    }
    status = AST_OK;
    return status;
}