#include "astTable.h"
#include "astMap.h"
#include "astStack.h"
#include "astString.h"
ast_Table *astTable_Init(ast_Integer arrSize, ast_Integer MapSize)
{
    ast_Table *tb = (ast_Table *)malloc(sizeof(ast_Table));
    if (arrSize > 0)
    {
        TValue *arr = (TValue *)malloc(sizeof(TValue) * arrSize);
        tb->arrSize = arrSize;
        tb->arr = arr;
    }
    else if (MapSize > 0)
    {
        ast_Map *map = astMap_Init(MapSize);
        tb->HashMap = map;
    }
    tb->next = NULL;
    tb->tt = AST_TTABLE;
    return tb;
}
TValue astTable_GetVal(ast_Table *tb, TValue key)
{
    ast_Integer flag = 0;
    ast_Integer res = ast_ConvertToIntegerAndGetFlag(key, &flag);
    if (flag && res >= 0 && res < tb->arrSize && (IsNum(key)))
    {
        return tb->arr[flag];
    }
    return astMap_GetValFromKey(tb->HashMap, key);
}
TValue astTable_PutVal(ast_Table *tb, TValue key, TValue val)
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
        if (!flag)
        {
            PANIC("无法转换为整数的浮点数无法作为key");
        }
        if (num >= 0 && num < tb->arrSize)
        {
            tb->arr[num] = val;
        }
        else if (num == tb->arrSize - 1 && val.tt == AST_TNIL)
        {

        }
        
    }
}