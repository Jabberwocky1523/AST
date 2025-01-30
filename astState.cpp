#include "astState.h"
#include "ast.h"
#include "astBinaryChunk.h"
#include "astFunc.h"
#include "astInstruction.h"
#include "astLexer.h"
#include "astMap.h"
#include "astObject.h"
#include "astStack.h"
#include "astString.h"
#include "astTable.h"
#include "astUtils.h"
#include "astVm.h"
#include "log.h"
#include "string.h"
ast_Map *kw;
ast_Bool ast_Init(ast_State *L, global_State *g_s)
{
    int nRegs = 0;
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
    TValue reg;
    reg.tt = AST_TTABLE;
    reg.value.gc = (GCObject *)astTable_Init(0, 0);
    ast_Integer g = AST_RIDX_GLOBALS;
    TValue key = Int2Ob(g);
    TValue val = Tb2Ob(astTable_Init(0, 0));
    astTable_PushVal(cast(ast_Table *, reg.value.gc), key, val);
    L->Registry = reg;
    ast_Stack *Stack = ast_NewStack(AST_MINSTACK, L);
    ast_PushStack(L, Stack);
    // kw = ast_KeyWordsInit(L);
    return TRUE;
}
TValue ast_ObjectToTValue(ast_State *L, void *ob, ast_Type type, int flag)
{
    TValue tt;
    if (ob == nullptr)
    {
        type = AST_TNIL;
    }
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
            tt.value.gc = (GCObject *)astString_NewLStr(L, cast(char *, ob),
                                                        strlen(cast(char *, ob)));
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
TValue ast_IntegerToTValue(ast_Integer i)
{
    TValue tt;
    tt.tt = AST_TINTEGER;
    tt.value.i = i;
    return tt;
}
TValue ast_NumberToTValue(ast_Number n)
{
    TValue tt;
    tt.tt = AST_TNUMBER;
    tt.value.n = n;
    return tt;
}
TValue ast_CFunctionToTValue(ast_CFunction func)
{
    TValue tt;
    tt.tt = AST_TUSERFUNCTION;
    ast_Closure *cs = (ast_Closure *)calloc(1, sizeof(ast_Closure));
    cs->func = func;
    cs->Upvalues = nullptr;
    tt.value.gc = cast(GCObject *, cs);
    return tt;
}
TValue ast_BooleanToTValue(ast_Bool bo)
{
    TValue tt;
    tt.tt = AST_TBOOLEAN;
    tt.value.bo = bo;
    return tt;
}
TValue ast_NilToTValue()
{
    TValue tt;
    tt.tt = AST_TNIL;
    tt.value.gc = nullptr;
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
ast_Bool ast_LoadChunk(ast_State *L, astBuffer chunk, Prototype *proto,
                       ast_String *chunkname, int mode)
{
    if (proto == nullptr)
    {
        proto = astBinaryChunkUnDump(chunk);
    }
    TValue tt;
    tt.tt = AST_TFUNCTION;
    ast_Closure *cs = (ast_Closure *)calloc(1, sizeof(ast_Closure));
    cs->Upvalues = nullptr;
    cs->pr = proto;
    tt.value.gc = cast(GCObject *, cs);
    if (proto->Upvalues.len > 0)
    {
        cs->Upvalues = (TValue *)calloc(proto->Upvalues.len, sizeof(TValue));
        cs->Uvslen = proto->Upvalues.len;
        TValue env = astTable_GetVal(cast(ast_Table *, L->Registry.value.gc),
                                     Int2Ob(AST_RIDX_GLOBALS));
        cs->Upvalues[0] = env;
    }
    ast_StackPush(PStack(L), tt);
    return TRUE;
}
TValue *ast_PopN(ast_Stack *L, int size)
{
    if (size < 0)
    {
        size = 0;
    }
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
        printf("\t");
        PrintOperand(ins);
        printf("\t\n");
        ast_PrintStack(PStack(L));
        if (InstructionOpcode(ins) == OP_RETURN)
        {
            return TRUE;
        }
    }
    return TRUE;
}
ast_Bool ast_CallAstClousure(ast_State *L, ast_Closure *closure, int nArgs,
                             int nResults)
{
    int nRegs = closure->pr->MaxStackSize;
    int nParam = closure->pr->NumParams;
    int IsVarArg = closure->pr->IsVararg == 1;
    ast_Stack *newStack = ast_NewStack(nRegs + 20, L);
    newStack->closure = closure;
    if (closure->pr->Upvalues.len > 0 && newStack->closure->Upvalues == nullptr)
    {
        newStack->closure->Upvalues =
            (TValue *)calloc(closure->pr->Upvalues.len, sizeof(TValue));
        newStack->closure->Uvslen = closure->pr->Upvalues.len;
    }
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
    if (nResults > L->stack->nPrevFuncResults)
    {
        TValue tt = Nil2Ob();
        for (int i = L->stack->nPrevFuncResults; i < nResults; i++)
            ast_StackPush(PStack(L), tt);
        astack_Rotate(PStack(L), -nResults, nResults - L->stack->nPrevFuncResults);
    }
    if (nResults >= 0)
    {
        L->stack->nPrevFuncResults = nResults;
    }
    free(newStack->Value);
    if (newStack->varargs != nullptr)
    {
        free(newStack->varargs);
    }
    // else if (newStack->closure->Upvalues != nullptr)
    // {
    //     free(newStack->closure->Upvalues);
    // }
    else if (newStack->openuvs != nullptr)
    {
        free(newStack->openuvs);
    }
    newStack->closure = nullptr;
    free(newStack->closure);
    newStack = nullptr;
    free(newStack);
    return TRUE;
}
ast_Bool ast_CallCFunction(ast_State *L, ast_Closure *func, int nArgs,
                           int nResults)
{
    ast_Stack *newStack = ast_NewStack(nArgs + 20, L);
    newStack->closure = func;
    TValue *vals = ast_PopN(PStack(L), nArgs);
    ast_PopN(PStack(L), 1);
    ast_PushN(newStack, vals, nArgs);
    newStack->top = nArgs;
    ast_PushStack(L, newStack);
    ast_Integer num = newStack->closure->func(L);
    ast_PopStack(L);
    if (num >= 0)
    {
        L->stack->nPrevFuncResults = num;
        TValue *r = ast_PopN(newStack, num);
        for (int i = num - 1; i >= 0; i--)
        {
            ast_StackPush(PStack(L), r[i]);
        }
        r = nullptr;
        free(r);
    }
    if (nResults >= 0)
    {
        L->stack->nPrevFuncResults = nResults;
    }
    free(newStack->Value);
    if (newStack->varargs != nullptr)
    {
        free(newStack->varargs);
    }
    // else if (newStack->closure->Upvalues != nullptr)
    // {
    //     free(newStack->closure->Upvalues);
    // }
    newStack->closure = nullptr;
    free(newStack->closure);
    newStack = nullptr;
    free(newStack);
    return TRUE;
}
ast_Bool ast_Call(ast_State *L, int nArgs, int nResults)
{
    TValue val = ast_StackGetTValue(PStack(L), -(nArgs + 1));
    ast_Bool ok =
        (ast_Bool)((val.tt == AST_TFUNCTION) || (val.tt == AST_TUSERFUNCTION));
    if (!ok)
    {
        TValue mf = ast_GetMetaField(L, val, Char2Ob(L, "__call"));
        if (mf.tt != AST_TNIL)
        {
            if (mf.tt == AST_TFUNCTION || mf.tt == AST_TUSERFUNCTION)
            {
                ast_StackPush(PStack(L), mf);
                astack_Insert(PStack(L), -(nArgs + 2));
                nArgs += 1;
                // printf("call %s<%d,%d>\n", mf.value.gc->cl.pr->Source->data_,
                //        mf.value.gc->cl.pr->LineDefined,
                //        mf.value.gc->cl.pr->LastLineDefined);
                ast_CallAstClousure(L, cast(ast_Closure *, mf.value.gc), nArgs,
                                    nResults);
                return TRUE;
            }
        }
    }
    if (val.tt == AST_TFUNCTION)
    {
        // printf("call %s<%d,%d>\n", val.value.gc->cl.pr->Source->data_,
        //        val.value.gc->cl.pr->LineDefined,
        //        val.value.gc->cl.pr->LastLineDefined);
        ast_CallAstClousure(L, &val.value.gc->cl, nArgs, nResults);
    }
    else if (val.tt == AST_TUSERFUNCTION)
    {
        // printf("call CFunction\n");
        ast_CallCFunction(L, &val.value.gc->cl, nArgs, nResults);
    }
    else
    {
        PANIC("NOT FUNCTION!");
    }
    return TRUE;
}
ast_Integer ast_RegCount(ast_State *L)
{
    return L->stack->closure->pr->MaxStackSize;
}
ast_Integer ast_LoadVararg(ast_State *L, int n)
{
    if (n < 0)
    {
        n = L->stack->varArgsLen;
        L->stack->nPrevFuncResults = L->stack->varArgsLen;
    }
    ast_StackCheck(PStack(L), n);
    ast_PushN(PStack(L), L->stack->varargs, n);
    return n;
}
ast_Bool ast_LoadProto(ast_State *L, int idx)
{
    Prototype *proto = L->stack->closure->pr->Protos[idx];
    TValue tt;
    tt.tt = AST_TFUNCTION;
    ast_Closure *cs = (ast_Closure *)calloc(1, sizeof(ast_Closure));
    cs->pr = proto;
    cs->Upvalues = nullptr;
    tt.value.gc = (GCObject *)cs;
    if (proto->Upvalues.len > 0)
    {
        cs->Upvalues = (TValue *)calloc(proto->Upvalues.len, sizeof(TValue));
        cs->Uvslen = proto->Upvalues.len;
        for (int i = 0; i < proto->Upvalues.len; i++)
        {
            if (proto->Upvalues.data[i].Instack == 1)
            {
                if (L->stack->openuvs == nullptr)
                {
                    L->stack->openuvs = astMap_Init(8);
                }
                ast_Integer k = proto->Upvalues.data[i].Idx;
                TValue res = astMap_GetValFromKey(L->stack->openuvs,
                                                  Int2Ob(proto->Upvalues.data[i].Idx));
                if (res.tt != AST_TNIL)
                {
                    cs->Upvalues[i] = res;
                }
                else
                {
                    cs->Upvalues[i] = L->stack->Value[proto->Upvalues.data[i].Idx];
                    astMap_PushKeyVal(L->stack->openuvs,
                                      Int2Ob(proto->Upvalues.data[i].Idx),
                                      cs->Upvalues[i]);
                }
            }
            else
            {
                if (L->stack->closure->Upvalues != nullptr)
                    cs->Upvalues[i] =
                        L->stack->closure->Upvalues[proto->Upvalues.data->Idx];
            }
        }
    }
    ast_StackPush(PStack(L), tt);
    return TRUE;
}
ast_Bool ast_PushGlobalTable(ast_State *L)
{
    ast_Integer key = AST_RIDX_GLOBALS;
    TValue global =
        astTable_GetVal(cast(ast_Table *, L->Registry.value.gc), Int2Ob(key));
    ast_StackPush(PStack(L), global);
    return TRUE;
}
ast_Bool ast_GetGlobal(ast_State *L, TValue name)
{
    ast_Integer key = AST_RIDX_GLOBALS;
    TValue global =
        astTable_GetVal(cast(ast_Table *, L->Registry.value.gc), Int2Ob(key));
    TValue tt = astTable_GetVal(cast(ast_Table *, global.value.gc), name);
    ast_StackPush(PStack(L), tt);
    return TRUE;
}
ast_Bool ast_SetGlobal(ast_State *L, TValue name)
{
    ast_Integer key = AST_RIDX_GLOBALS;
    TValue global =
        astTable_GetVal(cast(ast_Table *, L->Registry.value.gc), Int2Ob(key));
    TValue val = ast_StackPop(PStack(L));
    astTable_PushVal(cast(ast_Table *, global.value.gc), name, val);
    return TRUE;
}
ast_Bool ast_RegisterPushValue(ast_State *L, ast_CFunction func, TValue name)
{
    ast_PushCFunction(L, func);
    ast_SetGlobal(L, name);
    return TRUE;
}
ast_Bool ast_Load(ast_State *L, char *file_Path)
{
    astBuffer file_content = LoadViaCodePath(file_Path);
    Prototype *proto = astBinaryChunkUnDump(file_content);
    PrintAst(proto);
    ast_LoadChunk(L, file_content, proto, nullptr, 0);
    free(file_content->data_);
    free(file_content);
    return TRUE;
}