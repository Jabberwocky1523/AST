#include "astState.h"
#include "ast.h"
#include "astString.h"
#include "astStack.h"
#include "astObject.h"
#include "string.h"
#include "log.h"
#include "astBinaryChunk.h"
#include "astInstruction.h"
#include "astVm.h"
ast_Bool ast_Init(ast_State *L, global_State *g_s)
{
    int nRegs = 0;
    ast_Stack *Stack = ast_NewStack(20);
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
    ast_PushStack(L, Stack);
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
ast_Bool ast_PushStack(ast_State *L, ast_Stack *stack)
{
    stack->prev = L->stack;
    L->stack = stack;
    return TRUE;
}
ast_Stack *ast_PopStack(ast_State *L)
{
    ast_Stack *S = L->stack;
    L->stack = S->prev;
    S->prev = nullptr;
    return S;
}
ast_Bool ast_LoadChunk(ast_State *L, astBuffer chunk, ast_String *chunkname, int mode)
{
    Prototype *proto = astBinaryChunkUnDump(chunk);
    TValue tt;
    tt.tt = AST_TFUNCTION;
    tt.value.gc = (GCObject *)proto;
    ast_StackPush(PStack(L), tt);
    return TRUE;
}
TValue *ast_PopN(ast_Stack *L, int size)
{
    TValue *val = (TValue *)malloc(sizeof(TValue) * size);
    for (int i = size - 1; i >= 0; i--)
    {
        val[i] = ast_StackPop(L);
    }
    return val;
}
ast_Bool ast_PushN(ast_Stack *L, TValue *vals, int num)
{
    for (int i = 0; i < abs(num); i++)
    {
        ast_StackPush(L, vals[i]);
    }
    return TRUE;
}
ast_Bool ast_RunAstClosure(ast_State *L)
{
    while (1)
    {
        int pc = ast_GetPc(L);
        Instruction ins = ast_Fetch(L);
        ast_ExecuteOp(L, ins);
        printf("[%d] %s", pc + 1, InstructionOpName(ins));
        ast_PrintStack(PStack(L));
        if (InstructionOpcode(ins) == OP_RETURN)
        {
            return TRUE;
        }
    }
    return TRUE;
}
ast_Bool ast_CallAstClousure(ast_State *L, TValue *clousure, int nArgs, int nResults)
{
    int nRegs = clousure->value.gc->cl.MaxStackSize;
    int nParam = clousure->value.gc->cl.NumParams;
    int IsVarArg = clousure->value.gc->cl.IsVararg == 1;
    ast_Stack *newStack = ast_NewStack(nRegs + 20);
    newStack->closure = clousure;
    TValue *vals = ast_PopN(PStack(L), nArgs);
    if (nParam > nArgs)
    {
        TValue *P = (TValue *)realloc(vals, sizeof(TValue) * nParam);
        if (P != nullptr)
        {
            vals = P;
        }
        P = nullptr;
        free(P);
        for (int i = nArgs; i < nParam; i++)
        {
            TValue tt = Nil2Ob();
            vals[i] = tt;
        }
    }
    ast_PopN(PStack(L), 1);
    ast_PushN(newStack, vals, nParam);
    newStack->top = nRegs;
    if (nArgs > nParam && IsVarArg)
    {
        newStack->varargs = (TValue *)malloc(sizeof(TValue) * (nArgs - nParam));
        for (int i = 0; i < nArgs - nParam; i++)
        {
            newStack->varargs[i] = vals[i + nParam];
        }
        newStack->varArgsLen = nArgs - nParam;
    }
    ast_PushStack(L, newStack);
    ast_RunAstClosure(L);
    ast_PopStack(L);
    if (nResults >= 0)
    {
        L->stack->nPrevFuncResults = nResults;
    }
    free(newStack->varargs);
    free(newStack->Value);
    free(newStack);
    newStack = nullptr;
    return TRUE;
}
ast_Bool ast_Call(ast_State *L, int nArgs, int nResults)
{
    TValue val = ast_StackGetTValue(PStack(L), -(nArgs + 1));
    if (val.tt = AST_TFUNCTION)
    {
        printf("call %s<%d,%d>\n", val.value.gc->cl.Source->data_, val.value.gc->cl.LineDefined, val.value.gc->cl.LastLineDefined);
        ast_CallAstClousure(L, &val, nArgs, nResults);
    }
    else
    {
        PANIC("NOT FUNCTION!");
    }
    return TRUE;
}
ast_Integer ast_RegCount(ast_State *L)
{
    return L->stack->closure->value.gc->cl.MaxStackSize;
}
ast_Bool ast_LoadVararg(ast_State *L, int n)
{
    if (n < 0)
    {
        n = L->stack->varArgsLen;
        L->stack->nPrevFuncResults = L->stack->varArgsLen;
    }
    ast_StackCheck(PStack(L), n);
    ast_PushN(PStack(L), L->stack->varargs, n);
    return TRUE;
}
ast_Bool ast_LoadProto(ast_State *L, int idx)
{
    Prototype *proto = L->stack->closure->value.gc->cl.Protos[idx];
    TValue tt;
    tt.tt = AST_TFUNCTION;
    tt.value.gc = (GCObject *)proto;
    ast_StackPush(PStack(L), tt);
    return TRUE;
}
ast_Bool ast_FreeStack(ast_Stack *L)
{
    TValue *value = L->Value;
}