#ifndef BASIC_LIB_H
#define BASIC_LIB_H
#include "../astUser.h"
#include "../astStr.h"
#include "../astTable.h"
#include "auxLib.h"
#include "../astMap.h"
#include "unordered_map"
std::unordered_map<String, ast_CFunction> func{
    {"print", astPrintTest},
    {"getmetatable", getMetatable},
    {"setmetatable", setMetatable},
    {"pairs", pairs},
    {"next", next},
    {"ipairs", ipairs},
    {"pcall", pCall}};
#endif