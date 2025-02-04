#include "auxLib.h"
#include "../astTable.h"
#include "../astFunc.h"
ast_Bool CheckAny(ast_State *L, ast_Integer idx)
{
    if (ast_StackDataType(PStack(L), idx) == AST_TNONE)
    {
        PANIC("%d value expected", idx);
        return FALSE;
    }
    return TRUE;
}
ast_Bool CheckType(ast_State *L, ast_Integer argidx, ast_Type t)
{
    return (ast_Bool)(ast_StackDataType(PStack(L), argidx) == t);
}
ast_Bool LoadFile(ast_State *L, String filename)
{
    return ast_Load(L, (char *)filename.c_str());
}
ast_Bool DoFile(ast_State *L, String filename)
{
    if (LoadFile(L, filename))
    {
        ast_Call(L, 0, 0);
    }
    return TRUE;
}
ast_Type GetField(ast_State *L, ast_Integer idx, TValue name)
{
    TValue tb = ast_StackGetTValue(PStack(L), idx);
    if (tb.tt != AST_TTABLE)
    {
        PANIC("not table!");
        return AST_TNIL;
    }
    TValue val = astTable_GetVal(&tb.value.gc->tb, name);
    ast_StackPush(PStack(L), val);
    return val.tt;
}
ast_Bool GetSubTable(ast_State *L, ast_Integer idx, String name)
{
    TValue key = Char2Ob(L, name.c_str());
    if (GetField(L, idx, key) == AST_TTABLE)
    {
        return TRUE;
    }
    ast_StackPop(PStack(L));
    idx = ast_StackAbsIndex(PStack(L), idx);
    ast_NewTable(L);
    TValue tb = ast_StackGetTValue(PStack(L), -1);
    ast_StackPush(PStack(L), tb);
    ast_SetTableFromString(L, idx, key);
    return FALSE;
}
ast_Type GetMetaField(ast_State *L, ast_Integer idx, String event)
{
    TValue mt = ast_GetMetaTable(L, ast_StackGetTValue(PStack(L), idx));
    if (mt.tt == AST_TNIL)
    {
        return AST_TNIL;
    }
    ast_StackPush(PStack(L), mt);
    TValue e = Char2Ob(L, event.c_str());
    ast_StackPush(PStack(L), e);
    ast_GetTableFromIdx(L, -2);
    int type = ast_StackGetTValue(PStack(L), -1).tt;
    if (type == AST_TNIL)
    {
        ast_StackPop(PStack(L));
        ast_StackPop(PStack(L));
    }
    else
    {
        astack_Remove(PStack(L), -2);
    }
    return type;
}
ast_Bool CallMeta(ast_State *L, ast_Integer obj, String name)
{
    obj = ast_StackAbsIndex(PStack(L), obj);
    if (GetMetaField(L, obj, name) == AST_TNIL)
    {
        return FALSE;
    }
    TValue val = ast_StackGetTValue(PStack(L), obj);
    ast_StackPush(PStack(L), val);
    ast_Call(L, 1, 1);
    return TRUE;
}
ast_Bool SetFuncs(ast_State *L, FuncRegs func)
{
    FuncRegs::iterator it = func.begin();
    for (; it != func.end(); it++)
    {
        ast_PushCFunction(L, it->second);
        ast_SetTableFromString(L, -2, Char2Ob(L, it->first.c_str()));
    }
    return TRUE;
}
ast_Bool OpenLibs(ast_State *L)
{
    FuncRegs lib = {{"_G", OpenBaseLibs},
                    {"string", OpenStringLibs},
                    {"table", OpenTableLibs},
                    {"math", OpenMathLibs},
                    {"os", OpenOsLibs},
                    {"io", OpenIoLibs},
                    {"package", OpenPackLibs}};
    FuncRegs::iterator it = lib.begin();
    for (; it != lib.end(); it++)
    {
        RequireF(L, it->first, it->second, TRUE);
        astack_Pop(PStack(L));
    }
    return TRUE;
}
ast_Bool RequireF(ast_State *L, String modname, ast_CFunction func, ast_Bool glb)
{
    GetSubTable(L, AST_REGISTRYINDEX, "_LOADED");
    GetField(L, -1, Char2Ob(L, modname.c_str()));
    TValue field = ast_StackGetTValue(PStack(L), -1);
    if (!ast_ConvertToBoolean(field))
    {
        ast_StackPop(PStack(L));
        TValue f = CFunc2Ob(func);
        ast_StackPush(PStack(L), f);
        TValue s = Char2Ob(L, modname.c_str());
        ast_StackPush(PStack(L), s);
        ast_Call(L, 1, 1);
        TValue r = ast_StackGetTValue(PStack(L), -1);
        ast_StackPush(PStack(L), r);
        ast_SetTableFromString(L, -3, s);
    }
    astack_Remove(PStack(L), -2);
    if (glb)
    {
        TValue r = ast_StackGetTValue(PStack(L), -1);
        ast_StackPush(PStack(L), r);
        ast_SetGlobal(L, Char2Ob(L, modname.c_str()));
    }
    return TRUE;
}
ast_Bool NewLibTable(ast_State *L, FuncRegs func)
{
    ast_CreateTableAndPush(L, 0, func.size());
    return TRUE;
}
ast_Bool NewLib(ast_State *L, FuncRegs func)
{
    NewLibTable(L, func);
    SetFuncs(L, func);
    return TRUE;
}