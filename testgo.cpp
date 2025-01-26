#include "astStr.h"
#include "astUtils.h"
#include "iostream"
int maingo(int argc, const char *const *argv)
{
    const char *file_name = "../luac.out";
    if (argc >= 2)
    {
        file_name = argv[1];
    }
    ast_Dump2File((char *)file_name);
}