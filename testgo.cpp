#include "lib/libtest.h"
#include "astStr.h"
#include "astUtils.h"
#include "iostream"
int maingo()
{
    char *buffer;
    int len;
    OPENANDREAD("/Users/tinakurisu/Desktop/AST/lua/hello_world.lua", buffer, len);
    GoString buf;
    buf.p = buffer;
    GoString name;
    name.p = "123";
    // String a = buf.p;
    // std::cout << a;
    // testParser(buf, name);
}