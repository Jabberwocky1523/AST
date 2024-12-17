/*
声明所有后续数据结构的状态

*/
#ifndef AST_STATE_H
#define AST_STATE_H

#include "ast.h"
#include "astObject.h"
typedef size_t ast_Mem;
union GCObject
{
    GCHeader gch;
    ast_String ts;
};
// 全局状态机
typedef struct global_State
{
    StringTable stringtable;
    ast_Mem GCthreshold;
    ast_Mem totalbytes;

} global_State;
typedef struct ast_State
{
    GCCommonHearder;
    ast_Byte status;
    TValue global_table;
} ast_State;

typedef struct StringTable
{
    GCObject **hash;
    ast_Uint32 Tnum;
    ast_Uint32 size;
} StringTable;

#endif
