#include "stdlib.h"
#include "stdio.h"
#include "astObject.h"

int main()
{
    printf("%d", (size_t)(~(size_t)0) - 2);
}