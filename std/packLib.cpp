#include "packLib.h"
#include "../astString.h"
#include "iostream"
#include "string"
#include "../astUtils.h"
#include "../log.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

std::optional<fs::path> findFile(const fs::path &directory, const std::string &filename)
{
    for (const auto &entry : fs::recursive_directory_iterator(directory))
    {
        if (entry.is_regular_file() && entry.path().string().find(filename) != std::string::npos)
        {
            return fs::absolute(entry.path());
        }
    }
    return std::nullopt;
}
ast_Integer preloadSearcher(ast_State *L)
{
    CheckType(L, 0, AST_TSTRING);
    TValue name = ast_StackGetTValue(PStack(L), 0);
    GetField(L, AST_REGISTRYINDEX, Char2Ob(L, "_PRELOADED"));
    GetField(L, -1, name);
    TValue f = ast_StackGetTValue(PStack(L), 0);
    if (f.tt == AST_TNIL)
    {
        ast_StackPush(PStack(L), Char2Ob(L, "error preload"));
    }
    return 1;
}
ast_Integer astSearcher(ast_State *L)
{
    CheckType(L, 0, AST_TSTRING);
    TValue name = ast_StackGetTValue(PStack(L), 0);
    fs::path path = "./";
    std::string str = getstr(&name.value.gc->ts);
    while (str.find(".") != std::string::npos)
    {
        std::string sep = "/";
        str = str.replace(str.find("."), 1, sep);
    }
    if (!str.ends_with(".lua"))
    {
        str = str.append(".lua");
    }
    auto result = findFile(path, str.c_str());
    if (!result.has_value())
    {
        String errmsg = "dont find ";
        errmsg.append(str.c_str());
        ast_StackPush(PStack(L), Char2Ob(L, errmsg.c_str()));
        return 1;
    }
    LoadFile(L, result.value().c_str());
    return 1;
}
ast_Integer OpenPackLibs(ast_State *L)
{
    NewLib(L, stdpack);
    GetSubTable(L, AST_REGISTRYINDEX, "_LOADED");
    ast_SetTableFromString(L, -2, Char2Ob(L, "loaded"));
    GetSubTable(L, AST_REGISTRYINDEX, "_PRELOAD");
    ast_SetTableFromString(L, -2, Char2Ob(L, "preload"));
    ast_StackPush(PStack(L), CFunc2Ob(require));
    ast_SetGlobal(L, Char2Ob(L, "require"));
    return 1;
}
ast_Bool _findLoader(ast_State *L, TValue name)
{
    ast_StackPush(PStack(L), CFunc2Ob(astSearcher));
    ast_StackPush(PStack(L), name);
    ast_Call(L, 1, 1);
    return TRUE;
}
ast_Integer require(ast_State *L)
{
    CheckType(L, 0, AST_TSTRING);
    TValue name = ast_StackGetTValue(PStack(L), 0);
    ast_StackSetTop(PStack(L), 1);
    GetField(L, AST_REGISTRYINDEX, Char2Ob(L, "_LOADED"));
    GetField(L, -1, name);
    TValue mod = ast_StackGetTValue(PStack(L), -1);
    if (mod.tt != AST_TNIL)
    {
        return 1;
    }
    ast_StackPop(PStack(L));
    _findLoader(L, name);
    TValue re = ast_StackGetTValue(PStack(L), -1);
    if (re.tt == AST_TSTRING)
    {
        return 1;
    }
    ast_Call(L, 0, 1);
    ast_StackPushValue(PStack(L), -1);
    ast_SetTableFromString(L, -3, name);
    return 1;
}
ast_Integer searchPath(ast_State *L)
{
    CheckType(L, 0, AST_TSTRING);
    ast_StackSetTop(PStack(L), 2);
    TValue path = ast_StackPop(PStack(L));
    fs::path p;
    if (path.tt == AST_TNIL)
    {
        p = "./";
    }
    else
    {
        p = getstr(&path.value.gc->ts);
    }
    TValue name = ast_StackPop(PStack(L));
    std::string n = getstr(&name.value.gc->ts);
    while (n.find(".") != std::string::npos)
    {
        std::string sep = "/";
        n = n.replace(n.find("."), 1, sep);
    }
    if (!n.ends_with(".lua"))
    {
        n = n.append(".lua");
    }
    auto re = findFile(p, n.c_str());
    if (!re.has_value())
    {
        ast_StackPush(PStack(L), Char2Ob(L, "cant find dir"));
    }
    else
    {
        ast_StackPush(PStack(L), Char2Ob(L, re.value().c_str()));
    }
    return 1;
}