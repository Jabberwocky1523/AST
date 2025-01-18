#ifndef _AST_LEXER_H
#define _AST_LEXER_H
#include "astState.h"
#include "astToken.h"
#define CClen(lex) strlen(lex->curchunk)
#define CopyToken(token, lex, str, T)                                                 \
    {                                                                                 \
        token.line = lex->Line;                                                       \
        token.kind = T;                                                               \
        if (token.size - 1 < strlen(str))                                             \
        {                                                                             \
            char *n = (char *)realloc(token.token, sizeof(char) * (strlen(str) + 1)); \
            token.size = strlen(str) + 1;                                             \
            if (n != nullptr)                                                         \
            {                                                                         \
                token.token = n;                                                      \
            }                                                                         \
        }                                                                             \
        memcpy(token.token, str, strlen(str));                                        \
        token.token[strlen(str)] = '\0';                                              \
    }
ast_Map *ast_KeyWordsInit(ast_State *L);
ast_Lexer *ast_NewLexer(ast_State *L, char *chunk, char *chunkName);
ast_Bool ast_SkipWhiteSpaces(ast_Lexer *lex);
TValue ast_ScanStr(ast_State *L, char *chunk);
ast_Token ast_NewToken();
TValue ast_ScanNumber(ast_State *L, char *chunk);
ast_Bool ast_NextToken(ast_Lexer *lex, ast_Token &token);
#endif