#include "astTable.h"
#include "astMap.h"
#include "astMath.h"
#include "astStack.h"
#include "astString.h"
#include "log.h"
ast_Table *astTable_Init(ast_Integer arrSize, ast_Integer MapSize)
{
    ast_Table *tb = (ast_Table *)malloc(sizeof(ast_Table));
    tb->HashMap = nullptr;
    tb->arr = nullptr;
    if (arrSize > 0)
    {
        TValue *arr = (TValue *)malloc(sizeof(TValue) * arrSize);
        tb->arrSize = arrSize;
        tb->arr = arr;
    }
    if (MapSize > 0)
    {
        ast_Map *map = astMap_Init(MapSize);
        tb->HashMap = map;
    }
    tb->next = NULL;
    tb->tt = AST_TTABLE;
    tb->arrtop = 0;
    tb->arrSize = arrSize;
    return tb;
}
ast_Bool astTableArr_Push(ast_Table *tb, TValue val)
{
    if (tb->arrtop == tb->arrSize)
    {
        TValue *NewArr = (TValue *)realloc(tb->arr, sizeof(TValue) * (tb->arrSize * 2));
        if (NewArr == nullptr)
        {
            PANIC("栈已满");
        }
        tb->arr = NewArr;
        tb->arrSize *= 2;
    }
    tb->arr[tb->arrtop++] = val;
    return TRUE;
}
TValue astTable_GetVal(ast_Table *tb, TValue key)
{
    ast_Integer flag = 0;
    if (IsNum(key))
    {
        ast_Integer res = ast_ConvertToIntegerAndGetFlag(key, &flag);
        res--;
        if (flag && res >= 0 && res < TableArrLen(tb))
        {
            return tb->arr[res];
        }
    }
    return astMap_GetValFromKey(tb->HashMap, key);
}
ast_Bool _astTable_ShrinkTableNil(ast_Table *tb)
{
    printf("do!");
    int i = TableArrLen(tb) - 1;
    for (; i >= 0; i--)
    {
        if (tb[i].tt != AST_TNIL)
        {
            break;
        }
    }
    tb->arrtop = i + 1;
    return TRUE;
}
ast_Bool _astTable_Expand(ast_Table *tb)
{
    int sum = 0;
    for (int i = TableArrLen(tb); true; i++)
    {
        TValue tt;
        tt.tt = AST_TINTEGER;
        tt.value.i = (ast_Integer)i;
        if (!astMap_FindNodeFromKey(tb->HashMap, tt))
        {
            return FALSE;
        }
        TValue tmp = tt;
        tt = astMap_GetValFromKey(tb->HashMap, tt);
        astMap_RemoveFromKey(tb->HashMap, tmp);
        astTableArr_Push(tb, tt);
    }
    return TRUE;
}
ast_Bool astTable_PushVal(ast_Table *tb, TValue key, TValue val)
{
    ast_Integer num = 0;
    if (key.tt == AST_TNIL)
    {
        PANIC("NIL无法作为key");
    }
    else if (IsNum(key))
    {
        ast_Integer flag = 0;
        num = ast_ConvertToIntegerAndGetFlag(key, &flag);
        num--;
        if (!flag && key.tt == AST_TNUMBER)
        {
            PANIC("无法转换为整数的浮点数无法作为key");
        }
        if (num >= 0 && num < TableArrLen(tb))
        {
            tb->arr[num] = val;
            if (num == TableArrLen(tb) - 1 && val.tt == AST_TNIL)
            {
                _astTable_ShrinkTableNil(tb);
            }
            return TRUE;
        }
        else if (num == TableArrLen(tb))
        {
            if (tb->HashMap != nullptr)
            {
                astMap_RemoveFromKey(tb->HashMap, key);
            }
            if (val.tt != AST_TNIL)
            {
                astTableArr_Push(tb, val);
                _astTable_Expand(tb);
            }
            return TRUE;
        }
    }
    if (val.tt != AST_TNIL)
    {
        if (tb->HashMap == nullptr)
        {
            tb->HashMap = astMap_Init(8);
        }
        astMap_PushKeyVal(tb->HashMap, key, val);
    }
    else
    {
        return astMap_RemoveFromKey(tb->HashMap, key);
    }
    return TRUE;
}
ast_Bool ast_CreateTableAndPush(ast_State *L, ast_Integer arrSize, ast_Integer mapsize)
{
    ast_Table *tb = astTable_Init(arrSize, mapsize);
    TValue tt;
    tt.value.gc = (GCObject *)tb;
    tt.tt = AST_TTABLE;
    ast_StackPush(PStack(L), tt);
    return TRUE;
}
ast_Bool ast_NewTable(ast_State *L)
{
    return ast_CreateTableAndPush(L, 0, 0);
}
// 将表中key对应val压入stack
ast_Type _ast_GetTable(ast_State *L, TValue tb, TValue key)
{
    if (tb.tt != AST_TTABLE)
    {
        PANIC("取到的不是表");
    }
    TValue val = astTable_GetVal(&tb.value.gc->tb, key);
    ast_StackPush(PStack(L), val);
    return val.tt;
}
// 从idx获取表，根据栈顶值从表中取值，然后将值push到栈顶
ast_Type ast_GetTableFromIdx(ast_State *L, ast_Integer idx)
{
    TValue tb = ast_StackGetTValue(PStack(L), idx);
    TValue key = astack_Pop(PStack(L));
    return _ast_GetTable(L, tb, key);
}
// 与ast_GetTableFromIdx类似，值变为自定义char *(HashMap)
ast_Type ast_GetTableFromString(ast_State *L, ast_Integer idx, TValue str)
{
    TValue tb = ast_StackGetTValue(PStack(L), idx);
    return _ast_GetTable(L, tb, str);
}
// 与ast_GetTableFromIdx类似，值变为自定义数值（数组）
ast_Type ast_GetTableFromNum(ast_State *L, ast_Integer idx, TValue num)
{
    TValue tb = ast_StackGetTValue(PStack(L), idx);
    return _ast_GetTable(L, tb, num);
}
ast_Bool _ast_SetTable(TValue tb, TValue key, TValue val)
{
    if (tb.tt != AST_TTABLE)
    {
        PANIC("取到的不是表");
    }
    return astTable_PushVal(&(tb.value.gc->tb), key, val);
}
// 栈顶弹出两个值分别为val,key(先v后k) 然后传给idx位置的表
ast_Bool ast_SetTableFromIdx(ast_State *L, ast_Integer idx)
{
    TValue tb = ast_StackGetTValue(PStack(L), idx);
    TValue val = ast_StackPop(PStack(L));
    TValue key = ast_StackPop(PStack(L));
    return _ast_SetTable(tb, key, val);
}
// idx确定表位置，val由stackpop,指定str类型的key
ast_Bool ast_SetTableFromString(ast_State *L, ast_Integer idx, TValue StrKey)
{
    TValue tb = ast_StackGetTValue(PStack(L), idx);
    TValue val = ast_StackPop(PStack(L));
    return _ast_SetTable(tb, StrKey, val);
}
// idx确定表位置，val由stackpop,指定num类型的key
ast_Bool ast_SetTableFromNum(ast_State *L, ast_Integer idx, TValue NumKey)
{
    TValue tb = ast_StackGetTValue(PStack(L), idx);
    TValue val = ast_StackPop(PStack(L));
    return _ast_SetTable(tb, NumKey, val);
}
ast_Bool ast_PrintTableArr(ast_Table tb)
{
    if (tb.arr == nullptr)
    {
        return FALSE;
    }
    printf("Arr: ");
    for (int i = 0; i < tb.arrtop; i++)
    {
        ast_PrintTValue(tb.arr[i]);
    }
    printf("\n");
    return TRUE;
}
ast_Bool ast_PrintTable(ast_Table tb)
{
    ast_PrintTableArr(tb);
    ast_PrintMap(tb.HashMap);
    return TRUE;
}
ast_Bool ast_FreeTable(ast_Table *tb)
{
    for (int i = 0; i < tb->arrSize; i++)
    {
        ast_FreeTvaluePoint(&tb->arr[i]);
    }
    free(tb);
    return TRUE;
}