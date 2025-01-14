#include <iostream>
#include <regex>
#include <string>

int mainr1()
{
    // C++ 的正则表达式
    std::string pattern = "^0[xX][0-9a-fA-F]*(\\.[0-9a-fA-F]*)?([pP][+\\-]?[0-9]+)?|^[0-9]*(\\.[0-9]*)?([eE][+\\-]?[0-9]+)?";

    // 要匹配的字符串
    std::string test_str = "132132312.13323 0x13"; // 你可以替换为其他测试字符串

    try
    {
        // 编译正则表达式
        std::regex regex(pattern);

        // 执行匹配
        std::smatch matches;
        if (std::regex_search(test_str, matches, regex))
        {
            // 打印匹配的位置和内容
            std::cout << "Group " << 0 << ": " << matches[0] << std::endl;
        }
        else
        {
            std::cout << "No match found" << std::endl;
        }
    }
    catch (const std::regex_error &e)
    {
        std::cerr << "Regex compilation failed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
