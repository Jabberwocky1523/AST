#include "astString.h"
#include <climits>
#include "astMem.h"
// 分配一个新字符串
ast_String *NewLStr(ast_State *L, const char *str, size_t len, ast_Hash hash)
{
    ast_String *ts;
    StringTable *tb;
    ts = (ast_String *)malloc((len + 1) * sizeof(char) + sizeof(ast_String));
    L->G_S->totalbytes += len;
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
    if (tb->Tnum > tb->size && tb->size < (INT_MAX - 2) / 2)
    {
        astString_Resize(L, tb->size * 2);
    }
    return ts;
}
ast_Hash astString_GetHash(const char *str)
{
    ast_Hash h = cast(ast_Hash, strlen(str));
    size_t step = (strlen(str) >> 5) + 1;
    size_t l1;
    for (l1 = strlen(str); l1 >= step; l1 -= step)
    {
        h = h ^ ((h << 5) + (h >> 2) + cast(unsigned char, str[l1 - 1]));
    }
    return h;
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
    return TRUE;
}
ast_Bool ast_IsString(TValue t)
{
    if (t.tt == AST_TSTRING || t.tt == AST_TINTEGER || t.tt == AST_TNUMBER)
    {
        return TRUE;
    }
    return FALSE;
}
ast_String *astString_FindStr(ast_State *L, const char *str)
{
    StringTable st = L->G_S->stringtable;
    ast_Hash h = astString_GetHash(str);
    GCObject *ans = st.hashTable[lmod(h, st.size)];
    while (ans)
    {
        if (strcmp(str, getstr(&ans->ts)) == 0)
        {
            return &ans->ts;
        }
        ans = ans->gch.next;
    }
    return NULL;
}
ast_Bool astString_RemoveStr(ast_State *L, const char *str)
{
    StringTable st = L->G_S->stringtable;
    ast_Hash h = astString_GetHash(str);
    GCObject *ans = st.hashTable[lmod(h, st.size)];
    GCObject *pre = ans;
    while (ans)
    {
        if (strcmp(str, getstr(&ans->ts)) == 0)
        {
            break;
        }
        pre = ans;
        ans = ans->gch.next;
    }
    if (ans == NULL)
    {
        return FALSE;
    }
    else if (pre == ans)
    {
        st.hashTable[lmod(h, st.size)] = ans->gch.next;
    }
    else
    {
        pre->gch.next = ans->gch.next;
    }
    ans->gch.next = nullptr;
    free(ans);
    return TRUE;
}
