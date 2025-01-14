#include <iostream>
#include <regex>
#include <string>

int main()
{
    // 手动转义字符串中的特殊字符
    std::string input = "'adad adass'1' 1\""; // 示例输入
    // 修改后的正则表达式：不使用原始字符串字面量
    std::regex re("(^'(\\\\|\\\\'|\\\\n|[^'\\n])*')|(^\"(\\\\|\\\\\"|\\\\n|[^\"\\n])*\")");

    std::smatch matches;

    // 使用正则表达式匹配
    if (std::regex_search(input, matches, re))
    {
        std::cout << "Matched: " << matches[0] << std::endl;
    }
    else
    {
        std::cout << "No match found." << std::endl;
    }

    return 0;
}
