#pragma once
#include "astVector.h"
using namespace astVector;
template <class T>
bool Vector<T>::swap(Vector<T> &other)
{
    std::swap(m_start, other.m_start);
    std::swap(m_finish, other.m_finish);
    std::swap(m_end_of_storage, other.m_end_of_storage);
    return true;
}
template <class T>
void Vector<T>::reserve(size_type n)
{
    if (capacity() >= n)
    {
        return;
    }
    Iterator new_start = allocate(n);
    Iterator s = new_start;
    for (Iterator it = begin(); it != end(); it++)
    {
        *(s++) = *it;
    }
    Iterator new_finish = s;
    std::destroy(begin(), end());
    deallocate(m_start);
    m_start = new_start;
    m_finish = s;
    m_end_of_storage = m_start + n;
}
template <class T>
Vector<T> &Vector<T>::operator=(const Vector<T> &other)
{
    if (this == &other)
    {
        return *this;
    }
    if (other.size() > capacity())
    {
        Vector tmp(other);
        tmp.swap(*this);
    }
    else if (other.size() > size())
    {
        std::copy(other.begin(), other.begin() + size(), m_start);
        m_finish = std::uninitialized_copy(other.begin() + size(), other.end(), m_finish);
    }
    else
    {
        Iterator m_finishit = std::copy(other.begin(), other.end(), m_start);
        std::destroy(m_finishit, end());
        m_finish = m_finishit;
    }
    return *this;
}
template <class T>
Vector<T>::reference Vector<T>::operator[](size_type n)
{
    return *(begin() + n);
}
template <class T>
Vector<T>::const_reference Vector<T>::operator[](size_type n) const
{
    return *(begin() + n);
}
template <class T>
Vector<T>::size_type Vector<T>::size() const
{
    return m_finish - m_start;
}
template <class T>
Vector<T>::value_type *Vector<T>::data()
{
    return begin();
}
template <class T>
const Vector<T>::value_type *Vector<T>::data() const
{

    return begin();
}
template <class T>
bool Vector<T>::empty() const
{
    return begin() == end();
}
template <class T>
void Vector<T>::clear()
{
    std::destroy(begin(), end());
    m_finish = m_start;
}

template <class T>
Vector<T>::Iterator Vector<T>::insert(Iterator pos, const value_type &val)
{
    return insert(pos, 1, val);
}
template <class T>
Vector<T>::Iterator Vector<T>::insert(Iterator pos, size_type n, const value_type &val)
{
    if (n == 0)
    {
        return pos;
    }
    if (size_type(m_end_of_storage - m_finish) >= n)
    {
        size_type e = m_finish - pos;
        if (n < e)
        {
            std::uninitialized_move(m_finish - n, m_finish, m_finish);
            std::move_backward(pos, m_finish - n, m_finish);
            std::fill_n(pos, n, val);
            m_finish += n;
            return pos;
        }
        else
        {
            std::uninitialized_move(pos, m_finish, pos + n);
            std::fill(pos, m_finish, val);
            std::uninitialized_fill(m_finish, pos + n, val);
            m_finish += n;
            return pos;
        }
    }
    else
    {
        size_type new_Size = std::max(2 * size(), size() + n);
        Iterator new_Start = allocate(new_Size);
        Iterator new_finish = std::uninitialized_move(m_start, pos, new_Start);
        Iterator ret = new_finish;
        new_finish = std::uninitialized_fill_n(new_finish, n, val);
        new_finish = std::uninitialized_move(pos, m_finish, new_finish);
        std::destroy(begin(), end());
        deallocate(m_start);
        m_start = new_Start;
        m_finish = new_finish;
        m_end_of_storage = m_start + new_Size;
        return ret;
    }
}
template <class T>
Vector<T>::Iterator Vector<T>::erase(size_type pos)
{
    return erase(pos, pos + 1);
}
template <class T>
Vector<T>::Iterator Vector<T>::erase(Iterator first, Iterator last)
{
    Iterator new_finish = std::uninitialized_move(last, m_finish, first);
    std::destroy(new_finish, m_finish);
    m_finish = new_finish;
    return first;
}
template <class T>
void Vector<T>::push_back(const value_type &val)
{
    insert(end(), val);
}
template <class T>
void Vector<T>::pop_back()
{
    std::destroy(end() - 1, end());
    m_finish--;
}
template <class T>
Vector<T>::size_type Vector<T>::capacity() const
{
    return m_end_of_storage - m_start;
}
template <class T>
Vector<T>::Iterator Vector<T>::begin()
{
    return m_start;
}
template <class T>
Vector<T>::Iterator Vector<T>::end()
{
    return m_finish;
}
template <class T>
Vector<T>::Iterator Vector<T>::begin() const
{
    return m_start;
}
template <class T>
Vector<T>::Iterator Vector<T>::end() const
{
    return m_finish;
}