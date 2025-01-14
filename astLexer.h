#ifndef _AST_LEXER_H
#define _AST_LEXER_H
#include "astState.h"
#include "astToken.h"
ast_Map *ast_KeyWordsInit(ast_State *L);
ast_Lexer *ast_NewLexer(char *chunk, char *chunkName);
ast_Bool ast_SkipWhiteSpaces(ast_Lexer *lex);
#endif