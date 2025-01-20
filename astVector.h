#ifndef AST_VECTOR_H
#define AST_VECTOR_H
#include "cstddef"
#include "iostream"
#include "memory"
namespace astVector
{
    template <class T>
    class Vector
    {
    public:
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = value_type &;
        using const_reference = const value_type &;
        using iterator = value_type *;
        using Iterator = iterator;
        using const_iterator = value_type *;
        using pointer = value_type *;

    private:
        T *allocate(size_type n)
        {
            return (T *)operator new(sizeof(T) * n);
        }
        bool deallocate(T *p)
        {
            operator delete(p);
            return true;
        }
        T *m_start;
        T *m_finish;
        T *m_end_of_storage;

    public:
        Vector()
        {
            m_start = nullptr;
            m_finish = nullptr;
            m_end_of_storage = nullptr;
        }
        Vector(size_type n, const value_type &val = value_type())
        {
            m_start = allocate(n);
            for (size_type i = 0; i < n; i++)
            {
                m_start[i] = val;
            }
            m_finish = m_end_of_storage = m_start + n;
        }
        ~Vector()
        {
            std::destroy(begin(), end());
            deallocate(m_start);
        }
        template <class InputIt>
        Vector(InputIt first, InputIt last)
        {
            size_type n = std::distance(first, last);
            m_start = allocate(n);
            size_type i = 0;
            for (Iterator it = first; it != last; it++)
            {
                *(m_start + i) = *it;
                i++;
            }
            m_end_of_storage = m_finish = m_start + n;
        }
        Vector(const Vector &other) : Vector(other.begin(), other.end()) {};
        bool swap(Vector &other);
        void reserve(size_type n);
        Vector &operator=(const Vector &other);
        reference operator[](size_type n);
        const_reference operator[](size_type n) const;
        size_type size() const;
        value_type *data();
        const value_type *data() const;
        bool empty() const;
        void clear();

        Iterator insert(Iterator pos, const value_type &val);
        Iterator insert(Iterator pos, size_type n, const value_type &val);
        Iterator erase(size_type pos);
        Iterator erase(Iterator first, Iterator last);
        void push_back(const value_type &val);
        void pop_back();
        size_type capacity() const;
        Iterator begin();
        Iterator end();
        Iterator begin() const;
        Iterator end() const;
    };
}
#include "astVector.cpp"
#endif