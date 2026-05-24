#include "iostream"
#include "dlfcn.h"
#include "../DllBuild/astState.h"
typedef void (*func)();
ast_Integer reopen(ast_State *L)
{
    void *handle = dlopen("./libre.dylib", RTLD_LAZY);
    if (!handle)
    {
        std::cout << "error" << std::endl;
        return 1;
    }
    func myfunc = (func)dlsym(handle, "test");
    const char *err = dlerror();
    if (err)
    {
        std::cerr << err << std::endl;
        dlclose(handle);
        return 1;
    }
    myfunc();
    return 0;
}
extern "C"
{
    ast_Integer astOpen(ast_State *L)
    {
        FuncRegs func = {{"reopen", reopen}};
        NewLib(L, func);
        return 1;
    }
}

int main()
{
}
