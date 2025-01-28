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
int mainstr()
{
    std::string a;
}
