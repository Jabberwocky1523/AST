#include "astString.h"

// 分配一个新字符串
ast_String *NewLStr(ast_State *L, const char *str, size_t len, ast_Hash hash)
{
    ast_String *ts;
    StringTable *tb;
    ts = (ast_String *)ast_Malloc(L, (len + 1) * sizeof(char) + sizeof(ast_String));
    ts->Tsv.len = len;
    ts->Tsv.hash = hash;
    ts->Tsv.marked = 0;
    ts->Tsv.tt = AST_TSTRING;
    ts->Tsv.reserved = 0;
    ast_Memcpy(ts + 1, str, len * sizeof(char));
    ((char *)(ts + 1))[len] = '\0'; // 字符串末尾
    tb = &L->G_S->stringtable;
    hash = lmod(hash, tb->size);
    ts->Tsv.next = tb->hashTable[hash];
    tb->hashTable[hash] = cast(GCObject *, ts);
    tb->Tnum++;
    if (tb->Tnum > tb->size && tb->Tnum < (INT_MAX - 2) / 2)
    {
        astString_Resize(L, tb->size * 2);
    }
    return ts;
}
ast_String *astString_NewLStr(ast_State *L, const char *str, size_t len)
{
    GCObject *Object;
    ast_Hash h = cast(ast_Hash, len);
    size_t step = (len >> 5) + 1;
    size_t l1;
    for (l1 = len; l1 >= step; l1 -= step)
    {
        h = h ^ ((h << 5) + (h >> 2) + cast(unsigned char, str[l1 - 1]));
    }
    for (Object = L->G_S->stringtable.hashTable[lmod(h, L->G_S->stringtable.size)]; Object != NULL; Object = Object->gch.next)
    {
        ast_String *ts = &(Object->ts);
        if (ts->Tsv.len == len && (ast_Memcmp(str, getstr(ts), len) == 0))
        {
            return ts;
        }
    }
    return NewLStr(L, str, len, h);
}
ast_Bool astString_Resize(ast_State *L, int newsize)
{
    StringTable *ts = &L->G_S->stringtable;
    GCObject **newhashtable;
    newhashtable = (GCObject **)(malloc(sizeof(GCObject *) * newsize));
    for (int i = 0; i < newsize; i++)
    {
        *(newhashtable + i) = NULL;
    }
    for (int i = 0; i < ts->size; i++)
    {
        GCObject *p = ts->hashTable[i];
        while (p)
        {
            GCObject *next = p->gch.next;
            ast_Hash hash = p->ts.Tsv.hash;
            int h1 = lmod(hash, newsize);
            p->gch.next = newhashtable[h1];
            newhashtable[h1] = p;
            p = next;
        }
    }
    for (int i = 0; i < ts->size; i++)
    {
        ts->hashTable[i] = NULL;
        free(ts->hashTable[i]);
    }
    ts->hashTable = newhashtable;
    ts->size = newsize;
}