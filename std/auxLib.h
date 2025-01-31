#ifndef AUX_LIB_H
#define AUX_LIB_H
#include "../astState.h"
#include "../astStack.h"
#include "../astStr.h"
#include "../astUser.h"
#include "unordered_map"
#define FuncRegs std::unordered_map<String, ast_CFunction>
ast_Bool CheckAny(ast_State *L, ast_Integer idx);
ast_Bool CheckType(ast_State *L, ast_Integer argidx, ast_Type t);
ast_Bool DoFile(ast_State *L, String filename);
ast_Type GetField(ast_State *L, ast_Integer idx, TValue name);
ast_Bool GetSubTable(ast_State *L, ast_Integer idx, String name);
ast_Bool CallMeta(ast_State *L, ast_Integer obj, String name);
ast_Type GetMetaField(ast_State *L, ast_Integer idx, String event);
ast_Integer OpenBaseLibs(ast_State *L);
ast_Bool OpenLibs(ast_State *L);
ast_Integer OpenStringLibs(ast_State *L);
ast_Bool RequireF(ast_State *L, String modname, ast_CFunction func, ast_Bool glb);
ast_Bool SetFuncs(ast_State *L, FuncRegs func);
ast_Bool NewLibTable(ast_State *L, FuncRegs func);
ast_Bool NewLib(ast_State *L, FuncRegs func);
#endif