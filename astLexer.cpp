#include "astLexer.h"
#include "string.h"
#include "astMap.h"
#include "astString.h"
#include "string"
#include "astMath.h"
#include "log.h"
#include "astMap.h"
#include <regex>
#include <iostream>
extern ast_Map *kw;
ast_Lexer *ast_NewLexer(ast_State *L, char *chunk, char *chunkName)
{
    ast_Lexer *lexer = (ast_Lexer *)calloc(1, sizeof(ast_Lexer));
    lexer->chunk = chunk;
    lexer->chunkName = chunkName;
    lexer->Line = 1;
    lexer->curchunk = lexer->chunk;
    lexer->chunkSize = strlen(lexer->chunk);
    lexer->L = L;
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
    // if (strlen(lex->curchunk) * 2 > lex->chunkSize && lex->chunkSize > 2)
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
ast_Integer ast_ScanNewLine(char *chunk)
{
    std::string input = chunk;
    std::regex reNewLine(R"(\r\n|\n\r|\n|\r)");

    auto matches_begin = std::sregex_iterator(input.begin(), input.end(), reNewLine);
    auto matches_end = std::sregex_iterator();

    int count = std::distance(matches_begin, matches_end);
    return count;
}
std::string ast_Escape(std::string str)
{
    std::string buf;
    while (str.length() > 0)
    {
        if (str[0] != '\\')
        {
            buf.push_back(str[0]);
            str = str.substr(1, str.length() - 1);
            continue;
        }
        if (str.length() == 1)
        {
            PANIC("unfished str");
        }
        switch (str[1])
        {
        case 'a':
        {
            buf.push_back('\a');
            str = str.substr(2, str.length() - 2);
            continue;
        }
        case 'b':
        {
            buf.push_back('\b');
            str = str.substr(2, str.length() - 2);
            continue;
        }
        case 'f':
        {
            buf.push_back('\f');
            str = str.substr(2, str.length() - 2);
            continue;
        }
        case 'n':
        {
            buf.push_back('\n');
            str = str.substr(2, str.length() - 2);
            continue;
        }
        case '\n':
        {
            buf.push_back('\n');
            str = str.substr(2, str.length() - 2);
            continue;
        }
        case 'r':
        {
            buf.push_back('\r');
            str = str.substr(2, str.length() - 2);
            continue;
        }
        case 't':
        {
            buf.push_back('\t');
            str = str.substr(2, str.length() - 2);
            continue;
        }
        case 'v':
        {
            buf.push_back('\v');
            str = str.substr(2, str.length() - 2);
            continue;
        }
        case '"':
        {
            buf.push_back('"');
            str = str.substr(2, str.length() - 2);
            continue;
        }
        case '\'':
        {
            buf.push_back('\'');
            str = str.substr(2, str.length() - 2);
            continue;
        }
        case '\\':
        {
            buf.push_back('\\');
            str = str.substr(2, str.length() - 2);
            continue;
        }
        default:
        {
            str = str.substr(2, str.length() - 2);
            continue;
        }
            // case '0':
            // case '1':
            // case '2':
            // case '3':
            // case '4':
            // case '5':
            // case '6':
            // case '7':
            // case '8':
            // case '9':
            // {
            // }
        }
    }
    return buf;
}
TValue ast_ScanIdentifier(ast_Lexer *lex, char *chunk)
{
    std::string input = lex->curchunk;
    std::regex reIdentifier(R"(^[_\d\w]+)");
    if (chunk != nullptr)
    {
        input = chunk;
    }
    std::smatch matches;
    try
    {
        if (std::regex_search(input, matches, reIdentifier))
        {
            std::string text = matches[0];
            ast_LexerNext(lex, text.length());
            return Char2Ob(lex->L, text.c_str());
        }
        else
        {
            return Char2Ob(lex->L, "");
        }
    }
    catch (const std::regex_error &e)
    {
        std::cerr << "Regex compilation failed: " << e.what() << std::endl;
        return Nil2Ob();
    }
}
TValue ast_ScanStr(ast_Lexer *lex)
{
    std::string input = lex->curchunk;
    std::regex re("(^'(\\\\|\\\\'|\\\\n|[^'\\n])*')|(^\"(\\\\|\\\\\"|\\\\n|[^\"\\n])*\")");
    std::smatch matches;
    try
    {
        if (std::regex_search(input, matches, re))
        {
            std::string text = matches[0];
            ast_LexerNext(lex, text.length());
            text = text.substr(1, text.length() - 2);
            if (text.find("\\") != std::string::npos)
            {
                lex->Line += ast_ScanNewLine((char *)text.c_str());
                text = ast_Escape(text);
            }
            return Char2Ob(lex->L, text.c_str());
        }
        else
        {
            return Char2Ob(lex->L, "");
        }
    }
    catch (const std::regex_error &e)
    {
        std::cerr << "Regex compilation failed: " << e.what() << std::endl;
        return Nil2Ob();
    }
}
TValue ast_ScanNumber(ast_Lexer *lex, char *chunk)
{
    std::string pattern = "^0[xX][0-9a-fA-F]*(\\.[0-9a-fA-F]*)?([pP][+\\-]?[0-9]+)?|^[0-9]*(\\.[0-9]*)?([eE][+\\-]?[0-9]+)?";
    std::string input = lex->curchunk;
    if (chunk != nullptr)
    {
        input = chunk;
    }
    try
    {
        std::regex regex(pattern);
        std::smatch matches;
        if (std::regex_search(input, matches, regex))
        {
            std::string text = matches[0];
            ast_LexerNext(lex, text.length());
            return Char2Ob(lex->L, text.c_str());
        }
        else
        {
            return Char2Ob(lex->L, "");
        }
    }
    catch (const std::regex_error &e)
    {
        std::cerr << "Regex compilation failed: " << e.what() << std::endl;
        return Nil2Ob();
    }
}
ast_Token ast_NewToken()
{
    ast_Token t;
    t.kind = 0;
    t.line = 0;
    t.token = nullptr;
    t.size = 0;
    return t;
}
ast_Bool ast_IsLetter(char c)
{
    return (ast_Bool)((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}
ast_Bool ast_NextToken(ast_Lexer *lex, ast_Token &token)
{
    if (token.token == nullptr)
    {
        token.token = (char *)malloc(sizeof(char) * 8);
        token.size = 8;
    }
    ast_SkipWhiteSpaces(lex);
    if (lex->chunkSize == 0 || CClen(lex) == 0)
    {
        CopyToken(token, lex, "EOF", TOKEN_EOF);
        return FALSE;
    }
    switch (lex->curchunk[0])
    {
    case ';':
    {
        ast_LexerNext(lex, 1);
        CopyToken(token, lex, ";", TOKEN_SEP_SEMI);
        return TRUE;
    }
    case ',':
    {
        ast_LexerNext(lex, 1);
        CopyToken(token, lex, ",", TOKEN_SEP_COMMA);
        return TRUE;
    }
    case '(':
    {
        ast_LexerNext(lex, 1);
        CopyToken(token, lex, "(", TOKEN_SEP_LPAREN);
        return TRUE;
    }
    case ')':
    {
        ast_LexerNext(lex, 1);
        CopyToken(token, lex, ")", TOKEN_SEP_RPAREN);
        return TRUE;
    }
    case ']':
    {
        ast_LexerNext(lex, 1);
        CopyToken(token, lex, "]", TOKEN_SEP_RBRACK);
        return TRUE;
    }
    case '{':
    {
        ast_LexerNext(lex, 1);
        CopyToken(token, lex, ")", TOKEN_SEP_LCURLY);
        return TRUE;
    }
    case '}':
    {
        ast_LexerNext(lex, 1);
        CopyToken(token, lex, "}", TOKEN_SEP_RCURLY);
        return TRUE;
    }
    case '+':
    {
        ast_LexerNext(lex, 1);
        CopyToken(token, lex, "+", TOKEN_OP_ADD);
        return TRUE;
    }
    case '-':
    {
        ast_LexerNext(lex, 1);
        CopyToken(token, lex, "-", TOKEN_OP_SUB);
        return TRUE;
    }
    case '*':
    {
        ast_LexerNext(lex, 1);
        CopyToken(token, lex, "*", TOKEN_OP_MUL);
        return TRUE;
    }
    case '^':
    {
        ast_LexerNext(lex, 1);
        CopyToken(token, lex, "^", TOKEN_OP_POW);
        return TRUE;
    }
    case '%':
    {
        ast_LexerNext(lex, 1);
        CopyToken(token, lex, "%", TOKEN_OP_MOD);
        return TRUE;
    }
    case '&':
    {
        ast_LexerNext(lex, 1);
        CopyToken(token, lex, "&", TOKEN_OP_BAND);
        return TRUE;
    }
    case '|':
    {
        ast_LexerNext(lex, 1);
        CopyToken(token, lex, "|", TOKEN_OP_BOR);
        return TRUE;
    }
    case '#':
    {
        ast_LexerNext(lex, 1);
        CopyToken(token, lex, "#", TOKEN_OP_LEN);
        return TRUE;
    }
    case ':':
    {
        if (ast_StrHasPrefix(lex, "::"))
        {
            ast_LexerNext(lex, 2);
            CopyToken(token, lex, "::", TOKEN_SEP_LABEL);
            return TRUE;
        }
        else
        {
            ast_LexerNext(lex, 1);
            CopyToken(token, lex, ":", TOKEN_SEP_COLON);
            return TRUE;
        }
    }
    case '/':
    {
        if (ast_StrHasPrefix(lex, "//"))
        {
            ast_LexerNext(lex, 2);
            CopyToken(token, lex, "//", TOKEN_OP_IDIV);
            return TRUE;
        }
        else
        {
            ast_LexerNext(lex, 1);
            CopyToken(token, lex, "/", TOKEN_OP_DIV);
            return TRUE;
        }
    }
    case '~':
    {
        if (ast_StrHasPrefix(lex, "~="))
        {
            ast_LexerNext(lex, 2);
            CopyToken(token, lex, "~=", TOKEN_OP_NE);
            return TRUE;
        }
        else
        {
            ast_LexerNext(lex, 1);
            CopyToken(token, lex, "~", TOKEN_OP_WAVE);
            return TRUE;
        }
    }
    case '=':
    {
        if (ast_StrHasPrefix(lex, "=="))
        {
            ast_LexerNext(lex, 2);
            CopyToken(token, lex, "==", TOKEN_OP_EQ);
            return TRUE;
        }
        else
        {
            ast_LexerNext(lex, 1);
            CopyToken(token, lex, "=", TOKEN_OP_ASSIGN);
            return TRUE;
        }
    }
    case '<':
    {
        if (ast_StrHasPrefix(lex, "<<"))
        {
            ast_LexerNext(lex, 2);
            CopyToken(token, lex, "<<", TOKEN_OP_SHL);
            return TRUE;
        }
        else if (ast_StrHasPrefix(lex, "<="))
        {
            ast_LexerNext(lex, 2);
            CopyToken(token, lex, "<=", TOKEN_OP_LE);
            return TRUE;
        }
        else
        {
            ast_LexerNext(lex, 1);
            CopyToken(token, lex, "<", TOKEN_OP_LT);
            return TRUE;
        }
    }
    case '>':
    {
        if (ast_StrHasPrefix(lex, ">>"))
        {
            ast_LexerNext(lex, 2);
            CopyToken(token, lex, ">>", TOKEN_OP_SHR);
            return TRUE;
        }
        else if (ast_StrHasPrefix(lex, ">="))
        {
            ast_LexerNext(lex, 2);
            CopyToken(token, lex, ">=", TOKEN_OP_GE);
            return TRUE;
        }
        else
        {
            ast_LexerNext(lex, 1);
            CopyToken(token, lex, ">", TOKEN_OP_GT);
            return TRUE;
        }
    }
    case '.':
    {
        if (ast_StrHasPrefix(lex, "..."))
        {
            ast_LexerNext(lex, 3);
            CopyToken(token, lex, "...", TOKEN_VARARG);
            return TRUE;
        }
        else if (ast_StrHasPrefix(lex, ".."))
        {
            ast_LexerNext(lex, 2);
            CopyToken(token, lex, "..", TOKEN_OP_CONCAT);
            return TRUE;
        }
        else if (strlen(lex->curchunk) == 1 || !(lex->curchunk[1] >= '0' && lex->curchunk[1] <= '9'))
        {
            ast_LexerNext(lex, 1);
            CopyToken(token, lex, ".", TOKEN_SEP_DOT);
            return TRUE;
        }
        CopyToken(token, lex, "EOF", TOKEN_EOF);
        return TRUE;
    }
    case '[':
    {
        ast_LexerNext(lex, 1);
        CopyToken(token, lex, "[", TOKEN_SEP_LBRACK);
        return TRUE;
    }
    case '\'':
    case '\"':
    {
        TValue str = ast_ScanStr(lex);
        CopyToken(token, lex, getstr(&str.value.gc->ts), TOKEN_STRING);
        return TRUE;
    }
    }
    char c = lex->curchunk[0];
    if (c == '.' || (c >= '0' && c <= '9'))
    {
        TValue num = ast_ScanNumber(lex, lex->curchunk);
        if (strcmp(getstr(&num.value.gc->ts), "") == 0)
        {
            PANIC("unfinish num");
        }
        CopyToken(token, lex, getstr(&num.value.gc->ts), TOKEN_NUMBER);
        return TRUE;
    }
    if (c == '_' || ast_IsLetter(c))
    {
        TValue str = ast_ScanIdentifier(lex, nullptr);
        if (strcmp(getstr(&str.value.gc->ts), "") == 0)
        {
            PANIC("unfinish identifier");
        }
        TValue kind = astMap_GetValFromKey(kw, str);
        if (kind.tt != AST_TNIL)
        {
            CopyToken(token, lex, getstr(&str.value.gc->ts), kind.value.i);
            return TRUE;
        }
        else
        {
            CopyToken(token, lex, getstr(&str.value.gc->ts), TOKEN_IDENTIFIER);
            return TRUE;
        }
    }
    PANIC("unfinish token");
    return FALSE;
}
char *KindToCatagory(ast_Integer kind)
{
    if (kind < TOKEN_SEP_SEMI)
    {
        return "other";
    }
    else if (kind <= TOKEN_SEP_RCURLY)
    {
        return "separator";
    }
    else if (kind <= TOKEN_OP_NOT)
    {
        return "operator";
    }
    else if (kind <= TOKEN_KW_WHILE)
    {
        return "keyword";
    }
    else if (kind == TOKEN_IDENTIFIER)
    {
        return "identifier";
    }
    else if (kind == TOKEN_NUMBER)
    {
        return "number";
    }
    else if (kind == TOKEN_STRING)
    {
        return "string";
    }
    return "other";
}
ast_Bool ast_TextLexer(ast_Lexer *lex)
{
    ast_Token token = ast_NewToken();
    while (1)
    {
        ast_NextToken(lex, token);
        printf("[%2d] [%-10s] %s\n", token.line, KindToCatagory(token.kind), token.token);
        if (token.kind == TOKEN_EOF)
        {
            break;
        }
    }
    return TRUE;
}