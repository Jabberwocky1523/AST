#include "astStr.h"
#include "iostream"
#include "astVector.h"
#include "astStat.h"
#include "astExp.h"
using namespace std;
using namespace astStr;
using namespace astVector;
void test(int *b)
{
    Vector<int *> a;
    a.push_back(b);
    a.push_back(b + 1);
}
typedef struct test2
{
    ast_Integer a;
    String b;
} test2;
int main()
{
    ast_Exp e;
    e.tt = UNOPEXP;
    ExpObject *eo = (ExpObject *)malloc(sizeof(ExpObject));
    eo->unopexp.Line = 1;
    eo->unopexp.Op = 0;
    ExpObject *i = (ExpObject *)malloc(sizeof(ExpObject));
    i->stringexp.Line = 1;
    i->stringexp.str = "213";
    ExpObject *s = (ExpObject *)malloc(sizeof(ExpObject));
    s->stringexp.Line = 1;
    s->stringexp.str = "123";
    ExpObject *con = (ExpObject *)malloc(sizeof(ExpObject));
    con->concatexp.Line = 0;
    ast_Exp c;
    c.tt = STRINGEXP;
    c.exp = i;
    con->concatexp.exps.push_back(c);
    c.exp = s;
    con->concatexp.exps.push_back(c);
    c.exp = con;
    c.tt = CONCATEXP;
    eo->unopexp.exp = c;
    e.exp = eo;
    ast_FreeExp(e);
    // test2 *t = (test2 *)malloc(sizeof(test2));
    // t->a = 1;
    // t->b = "123";
    // t->b.~String();
    // free(t);
    cout << 1;
}
