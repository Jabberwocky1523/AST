#include "astCo.h"
int main(int argc, char *argv[])
{
    bool done = false;
    std::cout << "Start coroutine_7in7out ()\n";
    // 调用协程,得到返回值c_r，后面使用这个返回值来管理协程。
    auto c_r = coroutine_7in7out();
    // 第一次停止因为initial_suspend 返回的是suspend_always
    // 此时没有进入Stage 1
    std::cout << "Coroutine " << (done ? "is done " : "isn't done ")
              << "ret =" << c_r.get() << std::endl;
    done = c_r.move_next();
    // 此时是，co_await std::suspend_always{}
    std::cout << "Coroutine " << (done ? "is done " : "isn't done ")
              << "ret =" << c_r.get() << std::endl;
    done = c_r.move_next();
    // 此时打印Stage 1
    std::cout << "Coroutine " << (done ? "is done " : "isn't done ")
              << "ret =" << c_r.get() << std::endl;
    done = c_r.move_next();
    std::cout << "Coroutine " << (done ? "is done " : "isn't done ")
              << "ret =" << c_r.get() << std::endl;
    done = c_r.move_next();
    std::cout << "Coroutine " << (done ? "is done " : "isn't done ")
              << "ret =" << c_r.get() << std::endl;
    done = c_r.move_next();
    std::cout << "Coroutine " << (done ? "is done " : "isn't done ")
              << "ret =" << c_r.get() << std::endl;
    return 0;
}