// #include "astExp.h"
// ast_Bool ast_FreeExp(ast_Exp exp)
// {
//     if (exp.exp == nullptr)
//     {
//         return FALSE;
//     }
//     switch (exp.tt)
//     {
//     case NILEXP:
//     case TRUEEXP:
//     case FALSEEXP:
//     case INTEGEREXP:
//     case FLOATEXP:
//         free(exp.exp);
//         return TRUE;
//     case STRINGEXP:
//     {
//         exp.exp->stringexp.str.~String();
//         free(exp.exp);
//         return TRUE;
//     }
//     case NAMEEXP:
//     {
//         exp.exp->nameexp.Name.~String();
//         free(exp.exp);
//         return TRUE;
//     }
//     case UNOPEXP:
//     {
//         ast_FreeExp(exp.exp->unopexp.exp);
//         free(exp.exp);
//         return TRUE;
//     }
//     case BINOPEXP:
//     {
//         ast_FreeExp(exp.exp->binopexp.exp1);
//         ast_FreeExp(exp.exp->binopexp.exp2);
//         free(exp.exp);
//         return TRUE;
//     }
//     case CONCATEXP:
//     {
//         for (ast_Exp e : exp.exp->concatexp.exps)
//         {
//             ast_FreeExp(e);
//         }
//         exp.exp->concatexp.exps.~Vector();
//         free(exp.exp);
//         return TRUE;
//     }
//     case TABLECONSTRUCTOREXP:
//     {
//         for (ast_Exp e : exp.exp->tableexp.keyexps)
//         {
//             ast_FreeExp(e);
//         }
//         exp.exp->tableexp.keyexps.~Vector();
//         for (ast_Exp e : exp.exp->tableexp.valexps)
//         {
//             ast_FreeExp(e);
//         }
//         exp.exp->tableexp.valexps.~Vector();
//         free(exp.exp);
//         return TRUE;
//     }
//     case FUNCDEFEXP:
//     {
//         return TRUE;
//     }
//     case PARENSEXP:
//     {
//         ast_FreeExp(exp.exp->parensexp.exp);
//         return TRUE;
//     }
//     case TABLEACCESSEXP:
//     {
//         ast_FreeExp(exp.exp->tableaccessexp.KeyExp);
//         ast_FreeExp(exp.exp->tableaccessexp.PrefixExp);
//         free(exp.exp);
//         return TRUE;
//     }
//     case FUNCCALLEXP:
//     {
//         if (exp.exp->funccallexp.NameExp != nullptr)
//         {
//             free(exp.exp->funccallexp.NameExp);
//         }
//         for (ast_Exp e : exp.exp->funccallexp.args)
//         {
//             ast_FreeExp(e);
//         }
//         exp.exp->funccallexp.args.~Vector();
//         ast_FreeExp(exp.exp->funccallexp.PrefixExp);
//         return TRUE;
//     }
//     }
// }