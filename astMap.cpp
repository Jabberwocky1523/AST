#include "astMap.h"
#include "astObject.h"
#include "astStack.h"
#include "log.h"
ast_Map *astMap_Init(ast_Integer size)
{
  ast_Map *map = (ast_Map *)malloc(sizeof(ast_Map));
  ast_MapNode **nodelist = (ast_MapNode **)malloc(sizeof(ast_MapNode *) * size);
  map->Mnum = 0;
  map->size = size;
  for (int i = 0; i < size; i++)
  {
    *(nodelist + i) = NULL;
  }
  map->map = nodelist;
  return map;
}
ast_MapNode *astMap_FindNodeFromKey(ast_Map *map, TValue key)
{
  if (map == nullptr)
  {
    return nullptr;
  }
  ast_Hash h = ast_GetTValueHash(key);
  ast_MapNode *cur = map->map[lmod(h, map->size)];
  while (cur)
  {
    if (ast_TValueCmp(cur->key, key))
    {
      return cur;
    }
    cur = cur->next;
  }
  return nullptr;
}
ast_Bool astMap_Resize(ast_Map *map, ast_Integer newsize)
{
  ast_MapNode **newnodelist =
      (ast_MapNode **)malloc(sizeof(ast_MapNode *) * newsize);
  for (int i = 0; i < newsize; i++)
  {
    *(newnodelist + i) = NULL;
  }
  for (int i = 0; i < map->size; i++)
  {
    ast_MapNode *cur = map->map[i];
    while (cur)
    {
      ast_MapNode *next = cur->next;
      ast_Hash h = ast_GetTValueHash(cur->key);
      int h1 = lmod(h, newsize);
      cur->next = newnodelist[h1];
      newnodelist[h1] = cur;
      cur = next;
    }
  }
  for (int i = 0; i < map->size; i++)
  {
    map->map[i] = nullptr;
    free(map->map[i]);
  }
  map->map = newnodelist;
  map->size = newsize;
  return TRUE;
}
ast_Bool astMap_PushKeyVal(ast_Map *map, TValue key, TValue val)
{
  if (astMap_FindNodeFromKey(map, key) != nullptr)
  {
    return FALSE;
  }
  else if (map == NULL)
  {
    return FALSE;
  }
  ast_Hash h = ast_GetTValueHash(key);
  ast_MapNode *newnode = (ast_MapNode *)malloc(sizeof(ast_MapNode));
  newnode->key = key;
  newnode->val = val;
  newnode->next = map->map[lmod(h, map->size)];
  map->map[lmod(h, map->size)] = newnode;
  map->Mnum++;
  if (map->Mnum > map->size && map->size < (__INT_MAX__ - 2) / 2)
  {
    astMap_Resize(map, 2 * map->size);
  }
  return TRUE;
}
TValue astMap_GetValFromKey(ast_Map *map, TValue key)
{
  if (astMap_FindNodeFromKey(map, key) == nullptr)
  {
    printf("找不到对应val\n");
    TValue tt;
    tt.tt = AST_TNIL;
    tt.value.gc = NULL;
    return tt;
  }
  return astMap_FindNodeFromKey(map, key)->val;
}
ast_Bool astMap_RemoveFromKey(ast_Map *map, TValue key)
{
  if (map == nullptr)
  {
    return FALSE;
  }
  ast_Hash h = ast_GetTValueHash(key);
  ast_MapNode *cur = map->map[lmod(h, map->size)];
  ast_MapNode *pre = cur;
  while (cur)
  {
    if (ast_TValueCmp(cur->key, key))
    {
      break;
    }
    pre = cur;
    cur = cur->next;
  }
  if (cur == NULL)
  {
    return FALSE;
  }
  else if (pre == cur)
  {
    map->map[lmod(h, map->size)] = cur->next;
  }
  else
  {
    pre->next = cur->next;
  }
  cur->next = nullptr;
  cur = nullptr;
  map->Mnum--;
  free(cur);
  return TRUE;
}
ast_Bool ast_PrintMap(ast_Map *map)
{
  if (map == nullptr)
  {
    return FALSE;
  }
  printf("Map: ");
  for (int i = 0; i < map->size; i++)
  {
    ast_MapNode *cur = map->map[i];
    while (cur)
    {
      ast_PrintTValue(cur->key);
      printf(":");
      ast_PrintTValue(cur->val);
      cur = cur->next;
    }
  }
  printf("\n");
  return TRUE;
}