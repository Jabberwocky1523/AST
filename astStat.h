#ifndef AST_STAT_H
#define AST_STAT_H
#include "astStr.h"
#include "astVector.h"
#include "astState.h"
enum
{
    EMPTYSTAT = 0,
    BREAKSTAT,
    LABELSTAT,
    GOTOSTAT,
    DOSTAT,
    WHILESTAT,
    REPEATSTAT,
    IFSTAT,
    FORNUMSTAT,
    FORINSTAT,
    LOCALVARDELCSTAT,
    ASSIGNSTAT,
    LOCALFUNDEFSTAT
};
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
    Vector<ast_Exp> exps;
    Vector<ast_Block *> blocks;
} IfStat;
typedef struct ForNumStat
{
    ast_Integer LineOfFor;
    ast_Integer LineOfDo;
    String varname;
    ast_Exp InitExp;
    ast_Exp LimitExp;
    ast_Exp StepExp;
    ast_Block *block;
} ForNumStat;
typedef struct ForInStat
{
    ast_Integer LineOfDo;
    Vector<String> NameList;
    Vector<ast_Exp> ExpList;
    ast_Block *Block;
} ForInStat;
typedef struct LocalVarDelcStat
{
    ast_Integer LastLine;
    Vector<String> NameList;
    Vector<ast_Exp> ExpList;
} LocalVarDelcStat;
typedef struct AssignStat
{
    ast_Integer LastLine;
    Vector<ast_Exp> VarList;
    Vector<ast_Exp> ExpList;
} AssignStat;
typedef struct LocalFuncDefStat
{
    String Name;
    ast_Exp *FuncDefExp;
} LocalFuncDefStat;
typedef union StatObject
{
    EmptyStat emptystat;
    BreakStat breakstat;
    LabelStat labelstat;
    GotoStat gotostat;
    DoStat dostat;
    WhileStat whilestat;
    RepeatStat repeatstat;
    IfStat ifstat;
    ForNumStat fornumstat;
    ForInStat forinstat;
    LocalVarDelcStat localvardelcstat;
    AssignStat assignstat;
    LocalFuncDefStat localfundefstat;
} StatObject;
#endif
