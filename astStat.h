#ifndef AST_STAT_H
#define AST_STAT_H
#include "astState.h"
#include "string.h"
#include "string"
// ";"
typedef struct EmptyStat
{
} EmptyStat;
// break
typedef struct BreakStat
{
    ast_Integer line;
} BreakStat;
typedef struct LabelStat
{
    std::string Name;
} LabelStat;
#endif