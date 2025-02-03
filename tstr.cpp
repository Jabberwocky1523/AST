#include <iostream>
#include <vector>
#include <variant>
#include <string>
#include <format>
#include <sstream>
#include "astStr.h"
#include "fstream"
using VariantType = std::variant<int, double, std::string>;

std::string variantToString(const VariantType &v)
{
    return std::visit([](auto &&arg) -> std::string
                      {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, int>) {
            return std::format("{}", arg);
        } else if constexpr (std::is_same_v<T, double>) {
            return std::format("{:.14g}", arg);
        } else if constexpr (std::is_same_v<T, std::string>) {
            return std::format("{}", arg);
        } }, v);
}

std::string formatWithVector(const std::string &fmt, const std::vector<VariantType> &vec)
{
    std::vector<std::string> args;
    for (const auto &v : vec)
    {
        args.push_back(variantToString(v));
    }

    std::ostringstream result;
    size_t argIndex = 0;
    for (size_t i = 0; i < fmt.size(); ++i)
    {
        if (fmt[i] == '{' && i + 1 < fmt.size() && fmt[i + 1] == '}')
        {
            if (argIndex < args.size())
            {
                result << args[argIndex++];
                ++i; // Skip '}'
            }
            else
            {
                result << "{}"; // 占位符超出元素数量
            }
        }
        else
        {
            result << fmt[i];
        }
    }
    return result.str();
}

int mainstr()
{
    String a = "1";
    std::cin >> a;
    std::cout << a;
    return 0;
}