#include "stdlib.h"
#include "stdio.h"
#include "astObject.h"

int main()
{
    TValue *a;
    setnvalue(a, 1);
    printf("%d", ttype(a));
    printnvalue(a);
    setbvalue(a, 1);
    printf("%d", ttype(a));
    printbvalue(a);
}