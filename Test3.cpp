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
int main(int argc, const char *const *argv)
{
    ast_State *L = (ast_State *)malloc(sizeof(ast_State));
    global_State *g_s = (global_State *)malloc(sizeof(global_State));
    ast_Init(L, g_s);
    ast_Token token = ast_NewToken();
    ast_Lexer *lex = ast_NewLexer(L, "()\"234567\"\n\"0x13123123123123\"", "test");
    // printf("%s %s", lex->chunk, lex->chunkName);
    while (ast_NextToken(lex, token))
    {

        printf("%d\n", token.size);
        printf("%s %d %d \n", token.token, token.kind, token.line);
    }
}