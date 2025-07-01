#include "iostream"
#include "dlfcn.h"
typedef int (*func)(int, int);
auto main() -> int
{
    void *handle = dlopen("libuser.dylib", RTLD_LAZY);
    if (!handle)
    {
        std::cout << "error" << std::endl;
        return 0;
    }
    func myfunc = (func)dlsym(handle, "add");
    const char *err = dlerror();
    if (err)
    {
        std::cerr << err << std::endl;
        dlclose(handle);
        return 1;
    }
    int result = myfunc(6, 2);
    std::cout << result << std::endl;
    dlclose(handle);
    return 0;
}
