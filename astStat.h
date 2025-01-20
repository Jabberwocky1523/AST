#ifndef AST_STAT_H
#define AST_STAT_H
#include "astState.h"
#include "astStr.h"
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
    astStr::String Name;
} LabelStat;
typedef struct GotoStat
{
    astStr::String Name;
} GotoStat;
typedef struct DoStat
{
    ast_Block *Block;
} DoStat;
typedef struct WhileStat
{
    ast_Exp exp;
    ast_Block *block;
} WhileState;
typedef struct RepeatStat
{
    ast_Block *block;
    ast_Exp exp;
} RepearStat;
typedef struct IfStat
{
    ast_Exp exp;
} IfStat;
#endif
