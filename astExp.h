#ifndef AST_EXP_H
#define AST_EXP_H
#include "astStr.h"
#include "astVector.h"
#include "astState.h"
enum
{
    NILEXP = 0,
    TRUEEXP,
    FALSEEXP,
    INTEGEREXP,
    FLOATEXP,
    STRINGEXP,
    NAMEEXP,
    UNOPEXP,
    BINOPEXP,
    CONCATEXP,
    TABLECONSTRUCTOREXP,
    FUNCDEFEXP,
    PARENSEXP,
    TABLEACCESSEXP,
    FUNCCALLEXP,
};
typedef struct NilExp
{
    ast_Integer Line;
} NilExp;
typedef struct TrueExp
{
    ast_Integer Line;
} TrueExp;
typedef struct FalseExp
{
    ast_Integer Line;
} FalseExp;
typedef struct IntegerExp
{
    ast_Integer Line;
    ast_Integer val;
} IntegerExp;
typedef struct FloatExp
{
    ast_Integer Line;
    ast_Number val;
} FloatExp;
typedef struct StringExp
{
    ast_Integer Line;
    String str;
} StringExp;
typedef struct NameExp
{
    ast_Integer Line;
    String Name;
} NameExp;
typedef struct UnopExp
{
    ast_Integer Line;
    ast_Integer Op;
    ast_Exp exp;
} UnopExp;
typedef struct BinopExp
{
    ast_Integer Line;
    ast_Integer Op;
    ast_Exp exp1;
    ast_Exp exp2;
} BinopExp;
typedef struct ConcatExp
{
    ast_Integer Line;
    Vector<ast_Exp> exps;
} ConcatExp;
typedef struct TableConstructOrExp
{
    ast_Integer Line;
    ast_Integer LastLine;
    Vector<ast_Exp> keyexps;
    Vector<ast_Exp> valexps;
} TableConstructOrExp;
typedef struct FuncDefExp
{
    ast_Integer Line;
    ast_Integer LastLine;
    Vector<String> ParList;
    ast_Block *block;
} FuncDefExp;
typedef struct ParensExp
{
    ast_Exp exp;
} ParensExp;
typedef struct TableAccessExp
{
    ast_Integer LastLine;
    ast_Exp PrefixExp;
    ast_Exp KeyExp;
} TableAccessExp;
typedef struct FuncCallExp
{
    ast_Integer Line;
    ast_Integer LastLine;
    ast_Exp PrefixExp;
    StringExp *NameExp;
    Vector<ast_Exp> args;
} FuncCallExp;
typedef union ExpObject
{
    NilExp nilexp;
    TrueExp trueexp;
    FalseExp falseexp;
    IntegerExp integerexp;
    FloatExp floatexp;
    StringExp stringexp;
    NameExp nameexp;
    UnopExp unopexp;
    BinopExp binopexp;
    ConcatExp concatexp;
    TableConstructOrExp tableexp;
    FuncDefExp funcdefexp;
    ParensExp parensexp;
    TableAccessExp tableaccessexp;
    FuncCallExp funccallexp;
} ExpObject;
ast_Bool ast_FreeExp(ast_Exp exp);
ast_Bool ast_FreeExpPointer(ast_Exp *exp);

#endif