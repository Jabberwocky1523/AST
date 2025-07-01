#include "iostream"
#include "test.h"
extern "C"
{
    int add(int a, int b)
    {
        return a + b;
    }
}
// auto main() -> int
// {
// }