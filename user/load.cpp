// #include "iostream"
// #include "../astState.h"
// #include "test.h"
// #include "stdlib.h"
// #include "stdio.h"
// #include "../astMem.h"
// #include "../astBuffer.h"
// #include "../astString.h"
// #include "../astMath.h"
// #include "../astStack.h"
// #include "../astOpcode.h"
// #include "../astUtils.h"
// #include "../astMap.h"
// #include "../astVm.h"
// #include "../log.h"
// #include "../astGc.h"
// #include "../astUser.h"
// #include "../astTable.h"
// #include "../astError.h"
// #include "../auxLib.h"
// typedef ast_Integer (*func)(ast_State *);
// auto main() -> int
// {
//     ast_State *L = (ast_State *)malloc(sizeof(ast_State));
//     global_State *g_s = (global_State *)malloc(sizeof(global_State));
//     ast_Init(L, g_s);
//     void *handle = dlopen("libuser.dylib", RTLD_LAZY);
//     if (!handle)
//     {
//         std::cout << "error" << std::endl;
//         return 0;
//     }
//     func myfunc = (func)dlsym(handle, "astOpen");
//     const char *err = dlerror();
//     if (err)
//     {
//         std::cerr << err << std::endl;
//         dlclose(handle);
//         return 1;
//     }
//     int result = myfunc(L);
//     std::cout << result << std::endl;
//     dlclose(handle);
//     return 0;
// }
