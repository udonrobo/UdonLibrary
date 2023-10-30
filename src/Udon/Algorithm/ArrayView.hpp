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
    public:
        using value_type             = T;
        using reference              = T&;
        using const_reference        = const T&;
        using pointer                = T*;
        using const_pointer          = const T*;
        using iterator               = T*;
        using const_iterator         = const T*;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

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

        template <typename InputIterator, typename = typename std::enable_if<std::is_convertible<typename std::iterator_traits<InputIterator>::iterator_category, std::input_iterator_tag>::value>::type>
        ArrayView(InputIterator first, InputIterator last)
            : m_data(first)
            , m_size(std::distance(first, last))
        {
        }

        explicit operator bool() const noexcept
        {
            return m_size;
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
        const_iterator begin() const
        {
            return { m_data };
        }
        
        iterator end()
        {
            return { m_data + m_size };
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

        const_reverse_iterator rbegin() const
        {
            return const_reverse_iterator{ end() };
        }
        reverse_iterator rbegin()
        {
            return reverse_iterator{ end() };
        }

        const_reverse_iterator rend() const
        {
            return const_reverse_iterator{ begin() };
        }
        reverse_iterator rend()
        {
            return reverse_iterator{ begin() };
        }

        const_reverse_iterator crbegin() const
        {
            return const_reverse_iterator{ cend() };
        }
        
        const_reverse_iterator crend() const
        {
            return const_reverse_iterator{ cbegin() };
        }
    };

}    // namespace Udon
