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
#include "astGc.h"
int main(int argc, const char *const *argv)
{
    ast_State *L = (ast_State *)malloc(sizeof(ast_State));
    global_State *g_s = (global_State *)malloc(sizeof(global_State));
    ast_Init(L, g_s);
    ast_YieldGc(L);
    TValue str = Char2Ob(L, "123");
    L->stack = ast_NewStack(1, L);
    str = Char2Ob(L, "234");
    L->stack = ast_NewStack(1, L);
    ast_Stack *mainstack = L->stack;
    str = Char2Ob(L, "345");

    // TValue tb = Tb2Ob(astTable_Init(L, 1, 1));
    L->stack = mainstack;
    ast_RunGc(L);
    ast_PrintGcList(L);
    // ast_Token token = ast_NewToken();
    // char *chunk;
    // ast_Integer len;
    // FILE *fp = fopen(argv[1], "rb");
    // OPENANDREAD(fp, chunk, len);
    // ast_Lexer *lex = ast_NewLexer(L, chunk, "test");
    // // ast_TextLexer(lex);
    // printf("%d ", lex->curchunk);
    // ast_LookAhead(lex, token);
    // printf("%s", token.token);
    // printf("%d", lex->curchunk);
}