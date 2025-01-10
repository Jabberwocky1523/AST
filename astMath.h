#ifndef AST_MATH_H
#define AST_MATH_H
#include "ast.h"
#include "math.h"
#include "astState.h"
ast_Integer ast_IFloorDiv(ast_Integer a, ast_Integer b);
ast_Number ast_FFloorDiv(ast_Number a, ast_Number b);
ast_Integer ast_DoubleToInteger(ast_Number src, ast_Integer *flag);
ast_Integer ast_StringToInteger(const char *str);
ast_Number ast_StringToDouble(const char *str);
ast_Bool ast_IsNumeric(const char *str);
ast_Integer ast_IntegerAdd(ast_Integer a, ast_Integer b);
ast_Number ast_NumberAdd(ast_Number a, ast_Number b);
ast_Integer ast_IntegerSub(ast_Integer a, ast_Integer b);
ast_Number ast_NumberSub(ast_Number a, ast_Number b);
ast_Integer ast_IntegerMul(ast_Integer a, ast_Integer b);
ast_Number ast_NumberMul(ast_Number a, ast_Number b);
ast_Integer ast_IntegerMod(ast_Integer a, ast_Integer b);
ast_Number ast_NumberMod(ast_Number a, ast_Number b);
ast_Number ast_NumberDiv(ast_Number a, ast_Number b);
ast_Number ast_NumberPow(ast_Number a, ast_Number b);
ast_Integer ast_Band(ast_Integer a, ast_Integer b);
ast_Integer ast_Bor(ast_Integer a, ast_Integer b);
ast_Integer ast_Bxor(ast_Integer a, ast_Integer b);
ast_Integer ast_Shl(ast_Integer a, ast_Integer b);
ast_Integer ast_Shr(ast_Integer a, ast_Integer b);
ast_Integer ast_IntegerUnm(ast_Integer a, ast_Integer nil);
ast_Number ast_NumberUnm(ast_Number a, ast_Number nil);
ast_Integer ast_Bnot(ast_Integer a, ast_Integer nil);

typedef struct ast_Operator
{
    char *MetaMethod;
    ast_Integer (*IntegerFunc)(ast_Integer a, ast_Integer b);
    ast_Number (*DoubleFunc)(ast_Number a, ast_Number b);
} ast_Operator;
ast_Bool ast_Arith(ast_State *L, int op);
ast_Bool ast_Compare(ast_State *L, int idx1, int idx2, int op);
ast_Bool ast_Len(ast_State *L, int idx);
ast_Bool ast_Concat(ast_State *L, int n);
int IntToFb(int x);
int FbToInt(int x);
#endif