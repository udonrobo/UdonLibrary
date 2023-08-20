//-------------------------------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    配列参照クラス
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <cstddef>    // std::ptrdiff_t
#include <iterator>


namespace Udon
{

    template <typename T>
    class ArrayView
    {
        using value_type      = T;
        using reference       = T&;
        using const_reference = const T&;
        using pointer         = T*;
        using const_pointer   = const T*;
        using iterator        = T*;
        using const_iterator  = const T*;

    private:
        pointer m_data;
        size_t  m_size;

    public:
        ArrayView()
            : m_data()
            , m_size()
        {
        }

        template <size_t N>
        ArrayView(value_type (&m_data)[N])
            : m_data(m_data)
            , m_size(N)
        {
        }

        ArrayView(pointer m_data, const size_t m_size)
            : m_data(m_data)
            , m_size(m_size)
        {
        }

        constexpr size_t size() const
        {
            return m_size;
        }

        pointer data()
        {
            return m_data;
        }
        const_pointer data() const
        {
            return m_data;
        }

        reference operator[](const size_t index)
        {
            return m_data[index];
        }
        const_reference operator[](const size_t index) const
        {
            return m_data[index];
        }

        reference at(const size_t index)
        {
            return m_data[index];
        }
        const_reference at(const size_t index) const
        {
            return m_data[index];
        }

        reference front()
        {
            return m_data[0];
        }
        const_reference front() const
        {
            return m_data[0];
        }

        reference back()
        {
            return m_data[m_size - 1];
        }
        const_reference back() const
        {
            return m_data[m_size - 1];
        }

        bool empty() const
        {
            return m_size == 0;
        }

        void clear()
        {
            fill(value_type());
        }

        void fill(const value_type& value)
        {
            for (auto& v : *this)
            {
                v = value;
            }
        }

        iterator begin()
        {
            return { m_data };
        }
        iterator end()
        {
            return { m_data + m_size };
        }
        const_iterator begin() const
        {
            return { m_data };
        }
        const_iterator end() const
        {
            return { m_data + m_size };
        }

        const_iterator cbegin() const
        {
            return { m_data };
        }
        const_iterator cend() const
        {
            return { m_data + m_size };
        }

        struct const_reverce_iterator
        {
            using iterator_category = std::random_access_iterator_tag;
            using value_type        = T;
            using difference_type   = std::ptrdiff_t;
            using pointer           = T*;
            using reference         = T&;

            pointer p;

            const_reverce_iterator& operator++()
            {
                --p;
                return *this;
            }
            const_reverce_iterator operator++(int)
            {
                const_reverce_iterator tmp = *this;
                --p;
                return tmp;
            }
            const_reverce_iterator& operator--()
            {
                ++p;
                return *this;
            }
            const_reverce_iterator operator--(int)
            {
                const_reverce_iterator tmp = *this;
                ++p;
                return tmp;
            }
            const_reverce_iterator& operator+=(const difference_type n)
            {
                p -= n;
                return *this;
            }
            const_reverce_iterator operator+(const difference_type n) const
            {
                return { p - n };
            }
            const_reverce_iterator& operator-=(const difference_type n)
            {
                p += n;
                return *this;
            }
            const_reverce_iterator operator-(const difference_type n) const
            {
                return { p + n };
            }
            difference_type operator-(const const_reverce_iterator& rhs) const
            {
                return rhs.p - p;
            }
            reference operator[](const difference_type n) const
            {
                return *(p - n);
            }
            reference operator*() const
            {
                return *p;
            }
            pointer operator->() const
            {
                return p;
            }
            bool operator==(const const_reverce_iterator& rhs) const
            {
                return p == rhs.p;
            }
            bool operator!=(const const_reverce_iterator& rhs) const
            {
                return p != rhs.p;
            }
            bool operator<(const const_reverce_iterator& rhs) const
            {
                return p > rhs.p;
            }
            bool operator>(const const_reverce_iterator& rhs) const
            {
                return p < rhs.p;
            }
            bool operator<=(const const_reverce_iterator& rhs) const
            {
                return p >= rhs.p;
            }
            bool operator>=(const const_reverce_iterator& rhs) const
            {
                return p <= rhs.p;
            }
        };

        struct reverce_iterator
            : public const_reverce_iterator
        {
            using iterator_category = std::random_access_iterator_tag;
            using value_type        = T;
            using difference_type   = std::ptrdiff_t;
            using pointer           = T*;
            using reference         = T&;

            reference operator*()
            {
                return const_cast<reference>(const_reverce_iterator::operator*());
            }
            pointer operator->()
            {
                return const_cast<pointer>(const_reverce_iterator::operator->());
            }

            reverce_iterator& operator++()
            {
                const_reverce_iterator::operator++();
                return *this;
            }
            reverce_iterator operator++(int)
            {
                reverce_iterator tmp = *this;
                const_reverce_iterator::operator++();
                return tmp;
            }
            reverce_iterator& operator--()
            {
                const_reverce_iterator::operator--();
                return *this;
            }
            reverce_iterator operator--(int)
            {
                reverce_iterator tmp = *this;
                const_reverce_iterator::operator--();
                return tmp;
            }
            reverce_iterator& operator+=(const difference_type n)
            {
                const_reverce_iterator::operator+=(n);
                return *this;
            }
            reverce_iterator operator+(const difference_type n) const
            {
                return { const_reverce_iterator::operator+(n) };
            }
            reverce_iterator& operator-=(const difference_type n)
            {
                const_reverce_iterator::operator-=(n);
                return *this;
            }
            reverce_iterator operator-(const difference_type n) const
            {
                return { const_reverce_iterator::operator-(n) };
            }
            difference_type operator-(const reverce_iterator& rhs) const
            {
                return const_reverce_iterator::operator-(rhs);
            }
            reference operator[](const difference_type n) const
            {
                return const_cast<reference>(const_reverce_iterator::operator[](n));
            }
        };

        reverce_iterator rbegin()
        {
            return { m_data + m_size - 1 };
        }
        reverce_iterator rend()
        {
            return { m_data - 1 };
        }
        const_reverce_iterator rbegin() const
        {
            return { m_data + m_size - 1 };
        }
        const_reverce_iterator rend() const
        {
            return { m_data - 1 };
        }

        const_reverce_iterator crbegin() const
        {
            return { m_data + m_size - 1 };
        }
        const_reverce_iterator crend() const
        {
            return { m_data - 1 };
        }
    };

}    // namespace Udon
