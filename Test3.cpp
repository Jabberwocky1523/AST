#include "stdlib.h"
#include "stdio.h"
#include "astMem.h"
#include "astBuffer.h"
#include "astString.h"
#include "astMath.h"
#include "astStack.h"
#include "astOpcode.h"
#include "astUtils.h"
#include "astMap.h"
#include "astVm.h"
#include "astTable.h"
int main3(int argc, const char *const *argv)
{
    char *a = "123";
    printf("%s %d\n", a, strlen(a));
    a = a + 1;
    printf("%s %d", a, strlen(a));
}