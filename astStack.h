#ifndef AST_STACK_H
#define AST_STACK_H
#include "astObject.h"
#include "astState.h"
#define PStack(L) (L)->stack // L为ast_State*类型
#define astack_AbsIndex(L, idx) ast_StackAbsIndex((L), (idx))
#define astack_GetTop(L) L->top
#define astack_CheckStack(L, n) ast_StackCheck(L, n)
#define astack_Pop(L) ast_StackPop((L))
#define astack_Copy(L, srcidx, toidx)               \
    {                                               \
        TValue tmp = ast_StackGetTValue(L, srcidx); \
        ast_StackSetTValue(L, tmp, toidx);          \
    }
#define astack_PushValueToTop(L, idx)            \
    {                                            \
        TValue tmp = ast_StackGetTValue(L, idx); \
        ast_StackPush(L, tmp);                   \
    }
/*
    栈顶值移动到指定位置
*/
#define astack_ReplaceToIdx(L, idx)      \
    {                                    \
        TValue tmp = astack_Pop(L);      \
        ast_StackSetTValue(L, tmp, idx); \
    }
/*
    idx1-idx2倒转
*/
#define astack_Reverse(L, idx1, idx2) ast_StackReverse(L, idx1, idx2)
/*
    将idx后到top的值上移()>0)或下移(<0)n个距离
*/
#define astack_Rotate(L, idx, n) ast_StackRotate(L, idx, n)
// 去除指定位置元素
#define astack_Remove(L, idx)        \
    {                                \
        ast_StackRotate(L, idx, -1); \
        ast_StackPop(L);             \
    }
// 栈顶值弹出，插入到指定位置
#define astack_Insert(L, idx)       \
    {                               \
        ast_StackRotate(L, idx, 1); \
    }
#define astack_ToBoolean(L, idx) ast_ConvertToBoolean(L->Value[idx])
#define astack_SetTop(L, idx) ast_StackSetTop(L, idx)
#define astack_DataType(L, idx) ast_StackDataType(L, idx);
#define astack_ToNumber(L, idx, flag) ast_ConvertToNumber(L->Value[idx], flag)
#define astack_ToString(L, idx) ast_ConvertToString(S, L->Value[idx])
ast_Stack *ast_NewStack(int size);
ast_Bool ast_StackCheck(ast_Stack *L, int n);
ast_Bool ast_StackPush(ast_Stack *L, TValue &value);
TValue ast_StackPop(ast_Stack *L);
ast_Bool ast_StackIdxIsValid(ast_Stack *L, int idx);
ast_Bool ast_StackReverse(ast_Stack *L, int idx1, int idx2);
ast_Bool ast_StackRotate(ast_Stack *L, int idx, int n);
TValue ast_StackGetTValue(ast_Stack *L, int idx);
ast_Bool ast_StackSetTop(ast_Stack *L, int idx);
int ast_StackAbsIndex(ast_Stack *L, int idx);
ast_Bool ast_StackSetTValue(ast_Stack *L, TValue &value, int idx);
int ast_StackDataType(ast_Stack *L, int idx);
ast_Bool ast_ConvertToBoolean(TValue val);
ast_Number ast_ConvertToNumber(TValue val, int flag);
ast_String ast_ConvertToString(ast_State *L, TValue &val);
ast_Bool ast_StackPush(ast_Stack *L, void *val, ast_Type type);
ast_Bool ast_StackPush(ast_Stack *L, ast_Bool val,ast_Type type);
ast_Bool ast_PrintStack(ast_Stack *L);
#endif