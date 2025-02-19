#include "astCo.h"
coro_ret<int> coroutine_7in7out()
{
    // 进入协程看initial_suspend，返回std::suspend_always{};会有一次挂起

    // std::cout << "Coroutine co_await std::suspend_never" << std::endl;
    // // co_await std::suspend_never{} 不会挂起
    // co_await std::suspend_never{};
    co_await std::suspend_always{};
    std::cout << "Coroutine stage 1 ,co_yield" << std::endl;
    co_yield 101;
    std::cout << "Coroutine stage 2 ,co_yield" << std::endl;
    co_yield 202;
    std::cout << "Coroutine stage 3 ,co_yield" << std::endl;
    co_yield 303;
    std::cout << "Coroutine stage end, co_return" << std::endl;
    co_return 808;
}