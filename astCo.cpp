// #include "astCo.h"
// coro_ret<int> coroutine_7in7out()
// {
//     co_await std::suspend_always{};
//     std::cout << "Coroutine stage 1 ,co_yield" << std::endl;
//     co_yield 101;
//     std::cout << "Coroutine stage 2 ,co_yield" << std::endl;
//     co_yield 202;
//     std::cout << "Coroutine stage 3 ,co_yield" << std::endl;
//     co_yield 303;
//     std::cout << "Coroutine stage end, co_return" << std::endl;
//     co_return 808;
// }