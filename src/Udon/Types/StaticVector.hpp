//
//    疑似可変長配列コンテナ
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <algorithm>
#include <initializer_list>
#include <iterator>

namespace Udon
{

    /// @brief 疑似可変長配列コンテナ
    /// @tparam T 要素の型
    /// @tparam Capacity 最大要素数
    template <typename T, size_t Capacity = 128>
    class StaticVector
    {
    public:
        using value_type             = T;
        using size_type              = size_t;
        using reference              = T&;
        using const_reference        = const T&;
        using pointer                = T*;
        using const_pointer          = const T*;
        using iterator               = T*;
        using const_iterator         = const T*;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    private:
        value_type m_data[Capacity];
        size_type  m_size;

    public:
        StaticVector()
            : m_size()
        {
        }

        StaticVector(std::initializer_list<T> init)
            : m_size(init.size())
        {
            std::copy(init.begin(), init.end(), begin());
        }

        explicit StaticVector(size_type n)
            : m_size(n)
        {
            std::fill(begin(), end(), value_type());
        }

        StaticVector(size_type n, const value_type& value)
            : m_size(n)
        {
            std::fill(begin(), end(), value);
        }

        template <typename InputIterator, typename = typename std::enable_if<std::is_convertible<typename std::iterator_traits<InputIterator>::iterator_category, std::input_iterator_tag>::value>::type>
        StaticVector(InputIterator first, InputIterator last)
            : m_size(std::distance(first, last))
        {
            std::copy(first, last, begin());
        }

        StaticVector(const StaticVector& other)
            : m_size(other.m_size)
        {
            std::copy(other.cbegin(), other.cend(), begin());
        }

        StaticVector(StaticVector&& other)
            : m_size(other.m_size)
        {
            for (size_type i = 0; i < m_size; ++i)
            {
                m_data[i] = std::move(other.m_data[i]);
            }
        }

        explicit operator bool() const
        {
            return m_size > 0;
        }

        size_type size() const
        {
            return m_size;
        }

        size_type capacity() const
        {
            return Capacity;
        }

        bool empty() const
        {
            return m_size == 0;
        }

        bool full() const
        {
            return m_size == Capacity;
        }

        void push_back(const value_type& value)
        {
            if (not full())
            {
                m_data[m_size++] = value;
            }
        }

        void push_back(value_type&& value)
        {
            if (not full())
            {
                m_data[m_size++] = std::move(value);
            }
        }

        void pop_back()
        {
            if (m_size > 0)
            {
                --m_size;
            }
        }

        void resize(size_type n)
        {
            if (n < m_size)
            {
                m_size = n;
            }
            else
            {
                std::fill(begin() + m_size, begin() + n, value_type{});
                m_size = n;
            }
        }

        void insert(iterator position, const value_type& val)
        {
            if (not full())
            {
                std::copy_backward(position, end(), end() + 1);
                *position = val;
                ++m_size;
            }
        }

        void insert(iterator position, value_type&& val)
        {
            if (not full())
            {
                std::copy_backward(position, end(), end() + 1);
                *position = std::move(val);
                ++m_size;
            }
        }

        void insert(iterator position, size_type n, const value_type& val)
        {
            if (m_size + n <= Capacity)
            {
                std::copy_backward(position, end(), end() + n);
                std::fill(position, position + n, val);
                m_size += n;
            }
        }

        template <typename InputIterator, typename = typename std::enable_if<std::is_convertible<typename std::iterator_traits<InputIterator>::iterator_category, std::input_iterator_tag>::value>::type>
        void insert(iterator position, InputIterator first, InputIterator last)
        {
            size_type n = std::distance(first, last);
            if (m_size + n <= Capacity)
            {
                std::copy_backward(position, end(), end() + n);
                std::copy(first, last, position);
                m_size += n;
            }
        }

        void erase(iterator position)
        {
            std::copy(position + 1, end(), position);
            --m_size;
        }

        void erase(iterator first, iterator last)
        {
            std::copy(last, end(), first);
            m_size -= std::distance(first, last);
        }

        void clear()
        {
            m_size = 0;
        }

        reference front()
        {
            return *m_data;
        }

        const_reference front() const
        {
            return *m_data;
        }

        reference back()
        {
            return m_data[m_size - 1];
        }

        const_reference back() const
        {
            return m_data[m_size - 1];
        }

        const_reference at(size_type index) const
        {
            return m_data[index];
        }

        reference at(size_type index)
        {
            return m_data[index];
        }

        const_reference operator[](size_type index) const
        {
            return m_data[index];
        }

        reference operator[](size_type index)
        {
            return m_data[index];
        }

        iterator begin()
        {
            return m_data;
        }
        const_iterator begin() const
        {
            return m_data;
        }

        iterator end()
        {
            return m_data + m_size;
        }
        const_iterator end() const
        {
            return m_data + m_size;
        }

        const_iterator cbegin() const
        {
            return m_data;
        }

        const_iterator cend() const
        {
            return m_data + m_size;
        }

        reverse_iterator rbegin()
        {
            return reverse_iterator(m_data + m_size - 1);
        }
        const_reverse_iterator rbegin() const
        {
            return const_reverse_iterator(m_data + m_size - 1);
        }

        reverse_iterator rend()
        {
            return reverse_iterator(m_data - 1);
        }
        const_reverse_iterator rend() const
        {
            return const_reverse_iterator(m_data - 1);
        }

        const_reverse_iterator crbegin() const
        {
            return const_reverse_iterator(m_data + m_size - 1);
        }

        const_reverse_iterator crend() const
        {
            return const_reverse_iterator(m_data - 1);
        }

#ifdef ARDUINO
        void show() const
        {
            Serial.print("[");
            for (size_type i = 0; i < m_size; ++i)
            {
                Serial.print(m_data[i]);
                if (i < m_size - 1)
                {
                    Serial.print(", ");
                }
            }
            Serial.println("]");
        }
#endif
    };
}    // namespace Udon