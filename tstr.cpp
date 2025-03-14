// #include <iostream>
// #include <filesystem>

// namespace fs = std::filesystem;

// std::optional<fs::path> findFile(const fs::path &directory, const std::string &filename)
// {
//     for (const auto &entry : fs::recursive_directory_iterator(directory))
//     {
//         if (entry.is_regular_file() && entry.path().filename() == filename)
//         {
//             return fs::absolute(entry.path());
//         }
//     }
//     return std::nullopt;
// }

// int mainstr()
// {
//     fs::path directory = "./";
//     std::string filename = "vector.lua";

//     auto result = findFile(directory, filename);
//     if (result)
//     {
//         std::cout << "文件找到: " << result.value() << std::endl;
//     }
//     else
//     {
//         std::cout << "文件未找到: " << filename << std::endl;
//     }

//     return 0;
// }
// #include "functional"
// #include "iostream"
// template <typename t>
// class fun
// {
// public:
//     t p;
//     fun() {}
//     fun(t pm) : p(pm) {}
//     template <typename U, typename... Args>
//     auto operator()(U &&obj, Args &&...args) -> decltype((obj.*p)(std::forward<Args>(args)...))
//     {
//         return (obj.*p)(std::forward<Args>(args)...);
//     }
// };
// template <class t>
// fun<t> mem(t pm)
// {
//     return fun<t>(pm);
// }
// class t
// {
// public:
//     int a;
//     int cal(int a, int b) { return a + b; }
// };
// int main()
// {
//     auto a = mem(&t::cal);
//     t c;
//     c.a = 1;
//     auto b = a(c, 1, 2);
//     std::cout << b;
// }
// #include <iostream>
// #include <functional>

// template <typename T>
// class Fun
// {
// public:
//     T p;
//     Fun() {}
//     Fun(T pm) : p(pm) {}

//     template <typename U, typename... Args>
//     auto operator()(U obj, Args... args)
//     {
//         return (obj.*p)(args...);
//     }
// };

// template <class T>
// Fun<T> mem(T pm)
// {
//     return Fun<T>(pm);
// }

// class Test
// {
// public:
//     int a;
//     int cal(int a, int b) { return a + b; }
//     ~Test()
//     {
//         printf("123");
//     }
// };

// int mainstr()
// {
//     auto a = mem(&Test::cal);
//     Test *c = new Test;
//     c->a = 1;
//     int result = a(*c, 1, 2);
//     std::cout << c->Test::cal(1, 2) << std::endl;
//     return 0;
// }
#include "astStr.h"
#include "iostream"
auto main() -> int
{
    String ss("test");
    String m("es");
    String::Iterator it = ss.findByKmp(m);
    std::cout << *it;
    return 0;
}