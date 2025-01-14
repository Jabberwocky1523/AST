#include "astLexer.h"
#include "string.h"
#include "astMap.h"
#include "string"
ast_Lexer *ast_NewLexer(char *chunk, char *chunkName)
{
    ast_Lexer *lexer = (ast_Lexer *)malloc(sizeof(ast_Lexer));
    lexer->chunk = chunk;
    lexer->chunkName = chunkName;
    lexer->Line = 1;
    lexer->curchunk = lexer->chunk;
    lexer->chunkSize = strlen(lexer->chunk);
    return lexer;
}
ast_Map *ast_KeyWordsInit(ast_State *L)
{
    ast_Map *kw = astMap_Init(8);
    std::string key[22] = {"and", "break", "do", "else", "elseif", "end", "false",
                           "for", "function", "goto", "if", "in", "local", "nil",
                           "not", "or", "repeat", "return", "then", "true", "until",
                           "while"};
    ast_Integer val[22] = {
        TOKEN_OP_AND, TOKEN_KW_BREAK, TOKEN_KW_DO, TOKEN_KW_ELSE, TOKEN_KW_ELSEIF, TOKEN_KW_END, TOKEN_KW_FALSE,
        TOKEN_KW_FOR, TOKEN_KW_FUNCTION, TOKEN_KW_GOTO, TOKEN_KW_IF, TOKEN_KW_IN, TOKEN_KW_LOCAL, TOKEN_KW_NIL,
        TOKEN_OP_NOT, TOKEN_OP_OR, TOKEN_KW_REPEAT, TOKEN_KW_RETURN, TOKEN_KW_THEN, TOKEN_KW_TRUE, TOKEN_KW_UNTIL,
        TOKEN_KW_WHILE};
    for (int i = 0; i < 22; i++)
    {
        TValue k = Char2Ob(L, key[i].c_str());
        TValue v = Int2Ob(val[i]);
        astMap_PushKeyVal(kw, k, v);
    }
    return kw;
}
ast_Bool ast_StrHasPrefix(ast_Lexer *lex, char *s)
{
    ast_Integer size = strlen(s);
    for (int i = 0; i < size; i++)
    {
        if (lex->curchunk[i] != s[i])
        {
            return FALSE;
        }
    }
    return TRUE;
}
ast_Bool ast_LexerNext(ast_Lexer *lex, ast_Integer num)
{
    lex->curchunk += num;
    // if (strlen(lex->curchunk) * 2 > lex->chunkSize && lex->chunkSize > 50)
    // {
    //     char *newChunk = (char *)malloc(sizeof(char) * (strlen(lex->curchunk) + 1));
    //     memcpy(newChunk, lex->curchunk, strlen(lex->curchunk));
    //     newChunk[lex->chunkSize] = '\0';
    //     char *old = lex->chunk;
    //     lex->chunk = newChunk;
    //     lex->curchunk = lex->chunk;
    //     lex->chunkSize = strlen(lex->curchunk);
    //     free(old);
    // }
    return TRUE;
}
ast_Bool ast_IsWhiteSpace(char c)
{
    switch (c)
    {
    case '\t':
    case '\n':
    case '\v':
    case '\f':
    case '\r':
    case ' ':
        return TRUE;
    }
    return FALSE;
}
ast_Bool ast_IsNewLine(char c)
{
    return (ast_Bool)((c == '\r') || (c == '\n'));
}
ast_Bool ast_SkipComment(ast_Lexer *lex)
{
    ast_LexerNext(lex, 2);
    // if (ast_StrHasPrefix(lex, "["))
    // {
    // }
    while (strlen(lex->curchunk) > 0 && ast_IsNewLine(lex->curchunk[0]) == 0)
    {
        ast_LexerNext(lex, 1);
    }
    return TRUE;
}
ast_Bool ast_SkipWhiteSpaces(ast_Lexer *lex)
{
    while (strlen(lex->curchunk) > 0)
    {
        if (ast_StrHasPrefix(lex, "--"))
        {
            ast_SkipComment(lex);
        }
        else if (ast_StrHasPrefix(lex, "\r\n") || ast_StrHasPrefix(lex, "\n\r"))
        {
            ast_LexerNext(lex, 2);
            lex->Line += 1;
        }
        else if (ast_IsNewLine(lex->curchunk[0]))
        {
            ast_LexerNext(lex, 1);
            lex->Line += 1;
        }
        else if (ast_IsWhiteSpace(lex->curchunk[0]))
        {
            ast_LexerNext(lex, 1);
        }
        else
        {
            break;
        }
    }
    return TRUE;
}
ast_Bool ast_NextToken(ast_Lexer *lex)
{
}