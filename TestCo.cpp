#include "astCo.h"
int mainco(int argc, char *argv[])
{
    auto c_r = coroutine_7in7out();
    bool done = false;
    while (!done)
    {
        done = c_r.move_next();
    }
    return 0;
}