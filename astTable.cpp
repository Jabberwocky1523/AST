#include "astTable.h"
#include "astMap.h"
#include "astMath.h"
#include "astStack.h"
#include "astString.h"
#include "string.h"
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
    tb->MetaTable = nullptr;
    tb->tt = AST_TTABLE;
    tb->IteratorMap = nullptr;
    tb->arrtop = 0;
    tb->arrSize = arrSize;
    return tb;
}
ast_Bool astTableArr_Push(ast_Table *tb, TValue val)
{
    if (tb->arrtop == tb->arrSize)
    {
        TValue *NewArr = (TValue *)realloc(tb->arr, sizeof(TValue) * (tb->arrSize * 2 + 1));
        if (NewArr == nullptr)
        {
            PANIC("栈已满");
        }
        tb->arr = NewArr;
        tb->arrSize = tb->arrSize * 2 + 1;
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
    // printf("do!");
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
ast_Bool ast_SetMetaTable(ast_State *L, TValue mt, TValue val)
{
    if (val.tt == AST_TTABLE && mt.tt == AST_TTABLE)
    {
        val.value.gc->tb.MetaTable = cast(ast_Table *, mt.value.gc);
        return TRUE;
    }
    char *str = (char *)calloc(5, sizeof(char));
    sprintf(str, "_MT%d", val.tt);
    ast_String *key = astString_NewLStr(L, str, strlen(str));
    free(str);
    astTable_PushVal(cast(ast_Table *, L->Registry.value.gc), Str2Ob(L, key), mt);
    return TRUE;
}
TValue ast_GetMetaTable(ast_State *L, TValue val)
{
    if (val.tt == AST_TTABLE)
    {
        return Tb2Ob(val.value.gc->tb.MetaTable);
    }
    char *str = (char *)calloc(5, sizeof(char));
    sprintf(str, "_MT%d", val.tt);
    ast_String *key = astString_NewLStr(L, str, strlen(str));
    free(str);
    return astTable_GetVal(cast(ast_Table *, L->Registry.value.gc), Str2Ob(L, key));
}
TValue ast_GetMetaField(ast_State *L, TValue val, TValue str)
{
    TValue mt = ast_GetMetaTable(L, val);
    if (mt.tt != AST_TNIL)
    {
        return astTable_GetVal(cast(ast_Table *, mt.value.gc), str);
    }
    return Nil2Ob();
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
ast_Type _ast_GetTable(ast_State *L, TValue tb, TValue key, ast_Bool ign)
{
    if (tb.tt == AST_TTABLE)
    {
        TValue val = astTable_GetVal(&tb.value.gc->tb, key);
        TValue hmf = ast_GetMetaField(L, tb, Char2Ob(L, "__index"));
        if (ign || val.tt != AST_TNIL || hmf.tt == AST_TNIL)
        {
            ast_StackPush(PStack(L), val);
            return val.tt;
        }
    }
    if (!ign)
    {
        TValue mf = ast_GetMetaField(L, tb, Char2Ob(L, "__index"));
        if (mf.tt != AST_TNIL)
        {
            switch (mf.tt)
            {
            case AST_TTABLE:
            {
                return _ast_GetTable(L, mf, key, FALSE);
            }
            case AST_TFUNCTION:
            case AST_TUSERFUNCTION:
            {
                ast_StackPush(PStack(L), mf);
                ast_StackPush(PStack(L), tb);
                ast_StackPush(PStack(L), key);
                ast_Call(L, 2, 1);
                TValue val = ast_StackGetTValue(PStack(L), -1);
                return val.tt;
            }
            }
        }
    }
    PANIC("index error");
}
// 从idx获取表，根据栈顶值从表中取值，然后将值push到栈顶
ast_Type ast_GetTableFromIdx(ast_State *L, ast_Integer idx)
{
    TValue tb = ast_StackGetTValue(PStack(L), idx);
    TValue key = astack_Pop(PStack(L));
    return _ast_GetTable(L, tb, key, FALSE);
}
// 与ast_GetTableFromIdx类似，key变为自定义char *(HashMap)
ast_Type ast_GetTableFromString(ast_State *L, ast_Integer idx, TValue str)
{
    TValue tb = ast_StackGetTValue(PStack(L), idx);
    return _ast_GetTable(L, tb, str, FALSE);
}
// 与ast_GetTableFromIdx类似，key变为自定义数值（数组）
ast_Type ast_GetTableFromNum(ast_State *L, ast_Integer idx, TValue num)
{
    TValue tb = ast_StackGetTValue(PStack(L), idx);
    return _ast_GetTable(L, tb, num, FALSE);
}
ast_Bool _ast_SetTable(ast_State *L, TValue tb, TValue key, TValue val, ast_Bool ign)
{
    if (tb.tt == AST_TTABLE)
    {
        TValue kv = astTable_GetVal(cast(ast_Table *, tb.value.gc), key);
        TValue hmf = ast_GetMetaField(L, tb, Char2Ob(L, "__newindex"));
        if (ign || kv.tt != AST_TNIL || hmf.tt == AST_TNIL)
        {
            astTable_PushVal(&(tb.value.gc->tb), key, val);
            return TRUE;
        }
    }
    if (!ign)
    {
        TValue mf = ast_GetMetaField(L, tb, Char2Ob(L, "__newindex"));
        if (mf.tt != AST_TNIL)
        {
            switch (mf.tt)
            {
            case AST_TTABLE:
            {
                return _ast_SetTable(L, mf, key, val, FALSE);
            }
            case AST_TFUNCTION:
            case AST_TUSERFUNCTION:
            {
                ast_StackPush(PStack(L), mf);
                ast_StackPush(PStack(L), tb);
                ast_StackPush(PStack(L), key);
                ast_StackPush(PStack(L), val);
                ast_Call(L, 3, 0);
                return TRUE;
            }
            }
        }
    }
    PANIC("newindex error");
}
// 栈顶弹出两个值分别为val,key(先v后k) 然后传给idx位置的表
ast_Bool ast_SetTableFromIdx(ast_State *L, ast_Integer idx)
{
    TValue tb = ast_StackGetTValue(PStack(L), idx);
    TValue val = ast_StackPop(PStack(L));
    TValue key = ast_StackPop(PStack(L));
    return _ast_SetTable(L, tb, key, val, FALSE);
}
// idx确定表位置，val由stackpop,指定str类型的key
ast_Bool ast_SetTableFromString(ast_State *L, ast_Integer idx, TValue StrKey)
{
    TValue tb = ast_StackGetTValue(PStack(L), idx);
    TValue val = ast_StackPop(PStack(L));
    return _ast_SetTable(L, tb, StrKey, val, FALSE);
}
// idx确定表位置，val由stackpop,指定num类型的key
ast_Bool ast_SetTableFromNum(ast_State *L, ast_Integer idx, TValue NumKey)
{
    TValue tb = ast_StackGetTValue(PStack(L), idx);
    TValue val = ast_StackPop(PStack(L));
    return _ast_SetTable(L, tb, NumKey, val, FALSE);
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
ast_Bool ast_GetMetaTableFromIdx(ast_State *L, ast_Integer idx)
{
    TValue val = ast_StackGetTValue(PStack(L), idx);
    TValue mt = ast_GetMetaTable(L, val);
    if (mt.tt != AST_TNIL)
    {
        ast_StackPush(PStack(L), mt);
        return TRUE;
    }
    return FALSE;
}
// mt从栈中弹出
ast_Bool ast_SetMetaTableFromIdx(ast_State *L, ast_Integer idx)
{
    TValue val = ast_StackGetTValue(PStack(L), idx);
    TValue mt = astack_Pop(PStack(L));
    if (mt.tt == AST_TTABLE)
    {
        ast_SetMetaTable(L, mt, val);
    }
    else
    {
        PANIC("mt不是表");
    }
    return TRUE;
}
ast_Bool ast_InitIteratorMap(ast_Table *tb)
{
    if (tb->IteratorMap == nullptr)
    {
        tb->IteratorMap = astMap_Init(8);
    }
    TValue beginK = Nil2Ob();
    for (int i = 0; i < tb->arrSize; i++)
    {
        astMap_PushKeyVal(tb->IteratorMap, beginK, Int2Ob(i + 1));
        beginK = Int2Ob(i + 1);
    }
    if (tb->HashMap != nullptr)
    {
        for (int i = 0; i < tb->HashMap->size; i++)
        {
            ast_MapNode *cur = tb->HashMap->map[i];
            while (cur)
            {
                TValue key = cur->key;
                astMap_PushKeyVal(tb->IteratorMap, beginK, key);
                cur = cur->next;
                beginK = key;
            }
        }
    }
    astMap_PushKeyVal(tb->IteratorMap, beginK, Nil2Ob());
    return TRUE;
}
TValue ast_NextKey(TValue tb, TValue key)
{
    if (tb.value.gc->tb.IteratorMap == nullptr)
    {
        ast_InitIteratorMap(cast(ast_Table *, tb.value.gc));
    }
    return astMap_GetValFromKey(tb.value.gc->tb.IteratorMap, key);
}
ast_Bool ast_Next(ast_State *L, ast_Integer idx)
{
    TValue tb = ast_StackGetTValue(PStack(L), idx);
    if (tb.tt != AST_TTABLE)
    {
        PANIC("不为表");
    }
    TValue key = ast_StackPop(PStack(L));
    TValue nextkey = ast_NextKey(tb, key);
    if (nextkey.tt != AST_TNIL)
    {
        ast_StackPush(PStack(L), nextkey);
        TValue val = astTable_GetVal(cast(ast_Table *, tb.value.gc), nextkey);
        ast_StackPush(PStack(L), val);
        return TRUE;
    }
    return FALSE;
}