#include "stdlib.h"
#include "stdio.h"
#include "astMem.h"
#include "astBuffer.h"
#include "astString.h"
#include "astMath.h"
#include "astStack.h"
#include "astOpcode.h"
#include "astUtils.h"
#include "astMap.h"
#include "astVm.h"
#include "astTable.h"
#include "astLexer.h"
#include "iostream"
#include "astStat.h"
#include "fstream"
int main3(int argc, const char *const *argv)
{
    ast_State *L = (ast_State *)malloc(sizeof(ast_State));
    global_State *g_s = (global_State *)malloc(sizeof(global_State));
    ast_Init(L, g_s);
    ast_Token token = ast_NewToken();
    char *chunk;
    ast_Integer len;
    OPENANDREAD("/Users/tinakurisu/Desktop/AST/lua/test2.lua", chunk, len);
    ast_Lexer *lex = ast_NewLexer(L, chunk, "test");
    ast_TextLexer(lex);
}