#include "astStat.h"
ast_Stat parseEmptyStat(ast_Lexer *lex)
{
    ast_Stat res;
    res.tt = EMPTYSTAT;
    ast_Token token;
    ast_NextTokenOfKind(lex, token, TOKEN_SEP_SEMI);
    EmptyStat *ob = (EmptyStat *)malloc(sizeof(EmptyStat));
    res.stat = cast(StatObject *, ob);
    return res;
}
ast_Stat parseBreakStat(ast_Lexer *lex);