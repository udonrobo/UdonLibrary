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
#include <Udon/Common/Platform.hpp>
#include <cstddef>    // std::ptrdiff_t
#include <iterator>
#include <algorithm>
#include <cstring>

#if UDON_PLATFORM_OUTPUT_STREAM == UDON_PLATFORM_OUTPUT_CONSOLE
#    include <iostream>
#endif

namespace Udon
{

    template <typename T>
    class ArrayView
    {
    public:
        using size_type              = size_t;
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
        pointer   m_data;
        size_type m_size;

    public:
        ArrayView()
            : m_data()
            , m_size()
        {
        }

        template <size_type N>
        ArrayView(value_type (&m_data)[N])
            : m_data(m_data)
            , m_size(N)
        {
        }

        ArrayView(const char* string)
            : m_data(string)
            , m_size(strlen(string))
        {
        }

        ArrayView(pointer m_data, const size_type m_size)
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

        constexpr size_type size() const
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

        reference operator[](const size_type index)
        {
            return m_data[index];
        }
        const_reference operator[](const size_type index) const
        {
            return m_data[index];
        }

        reference at(const size_type index)
        {
            return m_data[index];
        }
        const_reference at(const size_type index) const
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

        /// @brief 指定された範囲からビューを作成する。
        /// @param beginIndex 開始位置
        /// @param endIndex 終端位置
        /// @return
        ArrayView subView(size_type beginIndex, size_type endIndex) const
        {
            return {
                std::next(cbegin(), beginIndex),
                std::min(endIndex - beginIndex, m_size)
            };
        }

        /// @brief 指定された範囲からビューを作成する。
        /// @remark 終端は現在のビューの終端
        /// @param beginIndex 開始位置
        /// @return
        ArrayView subView(size_type beginIndex) const
        {
            return {
                std::next(cbegin(), beginIndex),
                m_size
            };
        }

        /// @brief 特定の値まで検索し、そこまでの範囲の一つ手前までをビューとする。
        /// @param terminate 終端の値
        /// @return
        ArrayView subViewUntil(const value_type& terminate) const
        {
            return {
                cbegin(),
                std::find(cbegin(), cend(), terminate)
            };
        }

        /// @brief 自身が指定のビューから始まるかどうか
        /// @remark
        ///      [0, 1, 2, 3, 4].startsWith([0, 1, 2]) == true
        ///      [4, 3, 2, 1, 0].startsWith([0, 1, 2]) == false
        /// @param rhs
        /// @return
        bool startsWith(const ArrayView& rhs) const
        {
            if (size() >= rhs.size())
            {
                return std::equal(rhs.cbegin(), rhs.cend(), cbegin());
            }
            else
            {
                return false;
            }
        }

        friend bool operator==(const ArrayView& lhs, const ArrayView& rhs)
        {
            if (lhs.size() == rhs.size())
            {
                return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
            }
            else
            {
                return false;
            }
        }
        friend bool operator!=(const ArrayView& lhs, const ArrayView& rhs)
        {
            return not(lhs == rhs);
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

#if UDON_PLATFORM_OUTPUT_STREAM == UDON_PLATFORM_OUTPUT_CONSOLE
        friend std::ostream& operator<<(std::ostream& ostm, const ArrayView& rhs)
        {
            bool isFirst = true;
            for (auto&& element : rhs)
            {
                if (isFirst)
                {
                    ostm << element;
                }
                else
                {
                    ostm << ", " << element;
                }
                isFirst = false;
            }
            return ostm;
        }
#endif

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
