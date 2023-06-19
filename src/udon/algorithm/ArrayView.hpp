//-----------------------------------------------
//
//	UdonLibrary
//
//	Copyright (c) 2022-2023 Okawa Yusuke
//	Copyright (c) 2022-2023 udonrobo
//
//	Licensed under the MIT License.
//
//-----------------------------------------------
//
//  配列参照クラス
//
//-----------------------------------------------

#pragma once

#include <udon/stl/EnableSTL.hpp>

#include <cstddef>    // std::ptrdiff_t
#include <iterator>


namespace udon
{

    template <typename T>
    class ArrayView
    {
        T*     m_data;
        size_t m_size;

    public:
        ArrayView()
            : m_data()
            , m_size()
        {
        }

        template <size_t N>
        ArrayView(T (&m_data)[N])
            : m_data(m_data)
            , m_size(N)
        {
        }

        ArrayView(T* m_data, const size_t m_size)
            : m_data(m_data)
            , m_size(m_size)
        {
        }

        constexpr size_t size() const
        {
            return m_size;
        }

        T& operator[](const size_t index)
        {
            return m_data[index];
        }
        const T& operator[](const size_t index) const
        {
            return m_data[index];
        }

        T& at(const size_t index)
        {
            return m_data[index];
        }
        const T& at(const size_t index) const
        {
            return m_data[index];
        }

        struct iterator
        {
            using iterator_category = std::random_access_iterator_tag;
            using value_type        = T;
            using difference_type   = std::ptrdiff_t;
            using pointer           = T*;
            using reference         = T&;

            T* p;

            reference operator*()
            {
                return *p;
            }

            pointer operator->()
            {
                return p;
            }

            iterator& operator++()
            {
                ++p;
                return *this;
            }
            iterator& operator--()
            {
                --p;
                return *this;
            }
            iterator operator+(size_t n)
            {
                return { p + n };
            }
            iterator operator-(size_t n)
            {
                return { p - n };
            }
            difference_type operator-(const iterator& other)
            {
                return p - other.p;
            }

            bool operator<(const iterator& other) const
            {
                return p < other.p;
            }
            bool operator>(const iterator& other) const
            {
                return p > other.p;
            }
            bool operator<=(const iterator& other) const
            {
                return p <= other.p;
            }
            bool operator>=(const iterator& other) const
            {
                return p >= other.p;
            }
            bool operator==(const iterator& other) const
            {
                return p == other.p;
            }
            bool operator!=(const iterator& other) const
            {
                return p != other.p;
            }
        };

        iterator begin()
        {
            return { m_data };
        }

        iterator end()
        {
            return { m_data + m_size };
        }
    };

}    // namespace udon
