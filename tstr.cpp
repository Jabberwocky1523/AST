#include "astStr.h"
#include "iostream"
#include "astVector.h"
using namespace std;
using namespace astStr;
using namespace astVector;
void test(int *b)
{
    Vector<int *> a;
    a.push_back(b);
    a.push_back(b + 1);
}
int main()
{
    Vector<int *> a;
    int *p = new int[2];
    *p = 1;
    *(p + 1) = 2;
    test(p);
    cout << *p << *(p + 1);
    return 0;
}
