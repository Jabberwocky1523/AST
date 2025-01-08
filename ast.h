/*
    目前是宏定义以及后续文件的公共头
*/
#ifndef AST_H
#define AST_H
#include "vector"
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#define ast_Type int
#define FPF 50
#define PANIC(fmt, ...)                                              \
  printf("file:[%s], line:[%d] panic:" fmt "\n", __FILE__, __LINE__, \
         ##__VA_ARGS__);                                             \
  exit(-1);
// 重定义数据
typedef unsigned int ast_Uint32;
typedef enum ast_Bool
{
  FALSE = 0,
  TRUE
} ast_Bool;
#define AST_USER_ALINMENT \
  union                   \
  {                       \
    double u;             \
    void *s;              \
    long l;               \
  }
typedef AST_USER_ALINMENT ast_UMaxAlign;
typedef void *(*ast_Alloc)(void *ud, void *ptr, size_t oldSize, size_t newSize);

#define ttype(o) ((o)->tt)
#define AST_TNONE (-1)
#define AST_TNIL 0
#define AST_TBOOLEAN 1

#define AST_TLIGHTUSERDATA 2
#define AST_TNUMBER 3
#define AST_TINTEGER 4
#define AST_TSTRING 5
#define AST_TTABLE 6
#define AST_TFUNCTION 7
#define AST_TUSERFUNCTION 10
#define AST_TUSERDATA 8
#define AST_TTHREAD 9

#define AST_MINSTACK 20
#define AST_MAXSTACK 1000000
#define AST_REGISTRYINDEX -AST_MAXSTACK - 1000
#define AST_RIDX_GLOBALS 2

#endif