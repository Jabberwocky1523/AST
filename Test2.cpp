#include "iostream"
#include "any"
#include "string"
#include "typeinfo"
#include "optional"
#include "variant"
auto getany(std::any &&a) -> std::optional<int>
{
    if (a.type() == typeid(int))
    {
        std::optional<int> res = std::any_cast<int>(a);
        return res;
    }
    else if (a.type() == typeid(std::string))
    {
        std::optional<std::string> res = std::any_cast<std::string>(a);
    }
    return std::nullopt;
}
int mainany()
{
    std::any t;
    t = 1;
    auto a = std::any_cast<std::string>(t);
    auto b = std::is_same_v<std::any, decltype(t)>;
    std::variant<int, std::string> v = 1;
    std::cout << a;
}