#include "astStr.h"
#include "iostream"
using namespace astStr;
String::String()
{
    _size = 0;
    _buffer = new char[1];
    _buffer[0] = '\0';
}
String::String(const String &str)
{
    this->_size = str.size();
    this->_buffer = new char[_size + 1];
    strcpy(this->_buffer, str._buffer);
}
String::String(const String &str, size_l pos, size_l len)
{
    if (pos > str._size)
    {
        this->_buffer = new char[1];
        _buffer[0] = '\0';
        _size = 0;
    }
    else
    {
        if (pos + len > str._size)
        {
            _size = str._size - pos;
        }
        else
        {
            _size = len;
        }
        _buffer = new char[_size + 1];
        const char *p = str.c_str() + pos;
        for (int i = 0; i < _size; i++)
        {
            _buffer[i] = p[i];
        }
        _buffer[_size] = '\0';
    }
}
String::String(const char *str)
{
    _size = strlen(str);
    _buffer = new char[_size + 1];
    strcpy(_buffer, str);
}
String::String(const char *str, String::size_l len)
{
    if (strlen(str) < len)
    {
        _size = strlen(str);
        _buffer = new char[_size + 1];
        strcpy(_buffer, str);
    }
    else
    {
        _size = len;
        _buffer = new char[_size + 1];
        strncpy(_buffer, str, len);
    }
}
String::~String()
{
    if (_buffer)
    {
        delete[] _buffer;
    }
    _size = 0;
    _buffer = nullptr;
}
const char *String::c_str() const
{
    return _buffer;
}
String::size_l String::length() const
{
    return _size;
}
String::size_l String::size() const
{
    return _size;
}
char &String::operator[](size_l pos)
{
    return _buffer[pos];
}
String &String::operator=(const String &str)
{
    if (this->_size != 0)
    {
        delete[] _buffer;
    }
    _size = str._size;
    _buffer = new char[_size + 1];
    strcpy(_buffer, str.c_str());
    return *this;
}
String &String::operator=(const char *str)
{
    if (this->_size != 0)
    {
        delete[] _buffer;
    }
    _size = strlen(str);
    _buffer = new char[_size + 1];
    strcpy(_buffer, str);
    return *this;
}
String &String::operator=(const char byte)
{
    if (this->_size != 0)
    {
        delete[] _buffer;
    }
    _size = 1;
    _buffer = new char[_size + 1];
    _buffer[0] = byte;
    _buffer[1] = '\0';
    return *this;
}
String &String::operator+=(const String &str)
{
    _size += str._size;
    char *data = new char[_size + 1];
    strcpy(data, _buffer);
    strcat(data, str._buffer);
    delete[] _buffer;
    _buffer = data;
    return *this;
}
String &String::operator+=(const char *str)
{
    _size += strlen(str);
    char *data = new char[_size + 1];
    strcpy(data, _buffer);
    strcat(data, str);
    delete[] _buffer;
    _buffer = data;
    return *this;
}
String &String::operator+=(const char byte)
{
    _size += 1;
    char *data = new char[_size + 1];
    strcpy(data, _buffer);
    strcat(data, &byte);
    delete[] _buffer;
    _buffer = data;
    return *this;
}
String &String::append(const String &str)
{
    *this += str;
    return *this;
}
String &String::append(const char *str)
{
    *this += str;
    return *this;
}
String &String::append(const char byte)
{
    *this += byte;
    return *this;
}
String &String::push_back(const char byte)
{
    this->append(byte);
    return *this;
}
char String::pop_back()
{
    if (_size == 0)
    {
        char c = '\0';
        return c;
    }
    char c = _buffer[_size - 1];
    _buffer[_size - 1] = '\0';
    _size--;
    return c;
}
bool String::empty() const
{
    return _size == 0;
}
void String::clear()
{
    delete[] _buffer;
    _size = 0;
    _buffer = new char[1];
    _buffer[0] = '\0';
}
String::size_l String::compare(const String &str) const
{
    return strcmp(_buffer, str._buffer);
}

String::size_l String::compare(const char *str) const
{
    return strcmp(_buffer, str);
}
String String::substr(String::size_l pos, String::size_l len)
{
    if (pos >= _size)
    {
        String str;
        return str;
    }
    size_l __size;
    if (pos + len > _size)
    {
        __size = _size - pos;
    }
    else
    {
        __size = len;
    }
    char *__buffer = new char[__size + 1];
    strncpy(__buffer, _buffer + pos, __size);
    __buffer[__size] = '\0';
    String str(__buffer);
    return str;
}
String &String::erase(size_l pos)
{
    if (pos >= _size)
    {
        return *this;
    }
    Iterator begin(_buffer + pos);
    Iterator end(_buffer + _size - 1);
    for (; begin != end; begin++)
    {
        std::swap(*begin, *(&begin + 1));
    }
    this->pop_back();
    return *this;
}
String &String::erase(const Iterator &it)
{
    size_l pos = it - this->begin();
    return this->erase(pos);
}
const char *String::data()
{
    return _buffer;
}
String::Iterator String::find(char byte)
{

    return std::find(this->begin(), this->end(), byte);
}
String::Iterator String::find(const char *str)
{
    String s(str);
    return std::search(this->begin(), this->end(), s.begin(), s.end());
}
String::Iterator String::find(String &str)
{
    return std::search(this->begin(), this->end(), str.begin(), str.end());
}
int *aux_Kmp(String &str)
{
    int *next = (int *)calloc(str.length(), sizeof(int));
    next[0] = -1;
    for (int i = 2; i < str.length(); i++)
    {
        if (str[i - 1] == str[next[i - 1]])
        {
            next[i] = next[i - 1] + 1;
        }
        else
        {
            int tmp = next[i - 1];
            while (tmp >= 0)
            {
                if (str[tmp] == str[i - 1])
                {
                    break;
                }
                tmp = next[tmp];
            }
            next[i] = tmp + 1;
        }
    }
    return next;
}
String::Iterator String::findByKmp(String &str)
{
    int *next = aux_Kmp(str);
    int lt = 0, rt = 0;
    String tmp = *this;
    while (lt < this->length() && rt < str.length())
    {
        if (tmp[lt] == str[rt] || rt == -1)
        {
            lt++, rt++;
        }
        else
        {
            rt = next[rt];
        }
    }
    free(next);
    if (rt == str.length())
    {
        return this->begin() + (lt - rt);
    }
    return nullptr;
}
bool operator==(const String &lhs, const String &rhs)
{
    return (lhs.compare(rhs) == 0);
}
bool operator==(const char *lhs, const String &rhs)
{
    return (rhs.compare(lhs) == 0);
}
bool operator==(const String &lhs, const char *rhs)
{
    return (lhs.compare(rhs) == 0);
}
bool operator!=(const String &lhs, const String &rhs)
{
    return (lhs.compare(rhs) != 0);
}
bool operator!=(const char *lhs, const String &rhs)
{
    return (rhs.compare(lhs) != 0);
}
bool operator!=(const String &lhs, const char *rhs)
{
    return (lhs.compare(rhs) != 0);
}
bool operator==(astStr::String &lhs, astStr::String &rhs)
{
    return (lhs.compare(rhs) == 0);
}
std::ostream &operator<<(std::ostream &out, const String &str)
{
    out << str.c_str();
    return out;
}
std::istream &operator>>(std::istream &in, String &str)
{
    char tmp[1024];
    in >> tmp;
    str = tmp;
    return in;
}