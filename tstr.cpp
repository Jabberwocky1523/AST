#include "astStr.h"
#include "iostream"
using namespace std;
int main()
{
    astStr::String str("123");
    std::cout << str.c_str() << " " << str.length() << std::endl;
    str.pop_back();
    str.append("234");
    str = str.substr(0, 4);
    std::cout << str.data() << " " << str.length() << std::endl;
    str.erase(str.length() - 2);
    std::cout << str.data() << " " << str.length() << std::endl;
    str += "234";
    std::cout << str.data() << " " << str.length() << std::endl;
    str.erase(str.end() - 1);
    astStr::String::Iterator it = str.find("323");
    std::cout << str.data() << " " << str.length() << std::endl;
    cout << *it;
}
