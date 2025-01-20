#ifndef AST_STR_H
#define AST_STR_H
#include "string.h"
#include <algorithm>
#include "ostream"
namespace astStr
{
    class String
    {
    public:
        class Iterator
        {
        private:
            char *c;

        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type = char;
            using difference_type = std::ptrdiff_t;
            using pointer = char *;
            using reference = char &;

        public:
            Iterator()
            {
                c = nullptr;
            }
            Iterator(char *b)
            {
                c = b;
            }
            Iterator(const Iterator &it)
            {
                c = it.c;
            }
            Iterator(Iterator &it)
            {
                c = it.c;
            }
            reference operator*()
            {
                return *c;
            }
            pointer operator&() const
            {

                return c;
            }
            pointer operator->() const
            {
                return c;
            }
            bool operator==(const Iterator &other)
            {
                return c == other.c;
            }
            bool operator!=(const Iterator &other)
            {
                return c != other.c;
            }
            friend int operator-(const Iterator &it1, const Iterator &it2)
            {
                return it1.c - it2.c;
            }
            Iterator operator-(difference_type len)
            {
                return Iterator(c - len);
            }
            Iterator operator+(difference_type len)
            {
                return Iterator(c + len);
            }
            bool operator<(const Iterator &it)
            {
                return this->c < it.c;
            }
            bool operator<=(const Iterator &it)
            {
                return this->c <= it.c;
            }
            bool operator>(const Iterator &it)
            {
                return this->c > it.c;
            }
            bool operator>=(const Iterator &it)
            {
                return this->c >= it.c;
            }
            Iterator &operator++()
            {
                c++;
                return *this;
            }
            Iterator operator++(int)
            {
                Iterator it(*this);
                c++;
                return it;
            }
            Iterator &operator--()
            {
                c--;
                return *this;
            }
            Iterator operator--(int)
            {
                Iterator it(*this);
                c--;
                return it;
            }
        };
        using size_l = int;
        static const size_l npos = -1;
        String();
        String(const String &str);
        String(const String &str, size_l pos, size_l len = npos);
        String(const char *str);
        String(const char *str, size_l len);
        ~String();
        const char *c_str() const;
        size_l length() const;
        size_l size() const;
        char &operator[](size_l pos);
        String &operator=(const String &str);
        String &operator=(const char *str);
        String &operator=(const char byte);
        String &operator+=(const String &str);
        String &operator+=(const char *str);
        String &operator+=(const char byte);
        String &append(const String &str);
        String &append(const char *str);
        String &append(const char byte);
        String &push_back(const char byte);
        String &erase(size_l pos);
        String &erase(const Iterator &it);
        char pop_back();
        void clear();
        bool empty() const;
        size_l compare(const String &str) const;
        size_l compare(const char *str) const;
        String substr(size_l pos, size_l len);
        Iterator find(char byte);
        Iterator find(const char *str);
        Iterator find(String &str);
        const char *data();
        Iterator begin()
        {
            Iterator it(_buffer);
            return it;
        }
        Iterator end()
        {
            Iterator it(_buffer + _size);
            return it;
        }

    private:
        char *_buffer;
        size_l _size;
    };

}
bool operator==(const astStr::String &lhs, const char *rhs);
bool operator==(const astStr::String &lhs, const astStr::String &rhs);
bool operator==(const char *lhs, const astStr::String &rhs);
bool operator!=(const astStr::String &lhs, const astStr::String &rhs);
bool operator!=(const char *lhs, const astStr::String &rhs);
bool operator!=(const astStr::String &lhs, const char *rhs);
std::ostream &operator<<(std::ostream &out, const astStr::String &str);
#endif