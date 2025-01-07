//
//    疑似可変長リングバッファコンテナ
//
//    Copyright (c) 2022 udonrobo
//

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <algorithm>
#include <cstddef>
#include <iterator>

namespace Udon
{

    /// @brief リングバッファ
    /// @tparam T 要素の型
    /// @tparam Capacity バッファの容量
    template <typename T, size_t Capacity>
    class RingBuffer
    {
    public:
        struct iterator;
        struct const_iterator;

        using value_type             = T;
        using reference              = T&;
        using const_reference        = const T&;
        using pointer                = T*;
        using const_pointer          = const T*;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    private:
        value_type m_data[Capacity];
        size_t     m_head;
        size_t     m_tail;
        size_t     m_size;

    public:
        /// @brief コンストラクタ
        constexpr RingBuffer()
            : m_data{}
            , m_head{}
            , m_tail{}
            , m_size{}
        {
        }

        /// @brief コピーコンストラクタ
        /// @param other
        constexpr RingBuffer(const RingBuffer& other)
            : m_data{ other.m_data }
            , m_head{ other.m_head }
            , m_tail{ other.m_tail }
            , m_size{ other.m_size }
        {
        }

        /// @brief デフォルトで初期化するコンストラクタ
        /// @param size 使用するサイズ
        /// @param value 初期値
        RingBuffer(size_t size, const_reference value)
            : m_data{}
            , m_head{}
            , m_tail{}
            , m_size{ std::min(size, capacity()) }
        {
            std::fill(begin(), end(), value);
        }

        /// @brief デフォルトで初期化しないコンストラクタ
        /// @param size 使用するサイズ
        constexpr explicit RingBuffer(size_t size)
            : m_data{}
            , m_head{}
            , m_tail{}
            , m_size{ std::min(size, capacity()) }
        {
        }

        RingBuffer(std::initializer_list<value_type> init)
            : m_data{}
            , m_head{}
            , m_tail{}
            , m_size{ std::min(init.size, capacity()) }
        {
            std::copy(init.begin(), init.end(), begin());
        }

        /// @brief capacityを取得
        /// @return
        constexpr size_t capacity() const
        {
            return Capacity;
        }

        /// @brief バッファサイズを取得
        /// @return
        constexpr size_t size() const
        {
            return m_size;
        }

        /// @brief バッファサイズを変更
        /// @param size
        void resize(size_t size)
        {
            m_size = std::min(size, capacity());
        }

        constexpr bool empty() const
        {
            return m_size == 0;
        }

        constexpr bool full() const
        {
            return m_size == capacity();
        }

        /// @brief バッファの先頭に要素を追加
        /// @param value 追加する値
        void push(const_reference value)
        {
            if (full())
            {
                pop();
            }
            m_data[m_tail] = value;
            if (++m_tail == Capacity)
            {
                m_tail = 0;
            }
            ++m_size;
        }

        /// @brief バッファの先頭に要素を追加
        /// @param value 追加する値
        void push(value_type&& value)
        {
            if (full())
            {
                pop();
            }
            m_data[m_tail] = std::move(value);
            if (++m_tail == Capacity)
            {
                m_tail = 0;
            }
            ++m_size;
        }

        /// @brief バッファの末尾を取得し要素を削除
        /// @return 末尾の要素
        value_type pop()
        {
            if (empty())
            {
                return {};
            }
            auto&& retval = std::move(m_data[m_head]);
            if (++m_head == Capacity)
            {
                m_head = 0;
            }
            --m_size;
            return retval;
        }

        reference back()
        {
            return m_data[(m_tail + Capacity - 1) % Capacity];
        }
        const_reference back() const
        {
            return m_data[(m_tail + Capacity - 1) % Capacity];
        }

        reference front()
        {
            return m_data[m_head];
        }
        const_reference front() const
        {
            return m_data[m_head];
        }

        reference operator[](size_t index)
        {
            return m_data[(m_head + index) % Capacity];
        }
        const_reference operator[](size_t index) const
        {
            return m_data[(m_head + index) % Capacity];
        }

        struct const_iterator
        {
            using iterator_category = std::random_access_iterator_tag;
            using value_type        = T;
            using difference_type   = std::ptrdiff_t;
            using pointer           = const T*;
            using reference         = const T&;

            pointer m_data;
            size_t  m_index;
            size_t  m_size;

            const_iterator(pointer data, size_t index, size_t size)
                : m_data{ data }
                , m_index{ index }
                , m_size{ size }
            {
            }

            reference operator*() const
            {
                return m_data[m_index % m_size];
            }
            pointer operator->() const
            {
                return &m_data[m_index % m_size];
            }
            const_iterator& operator++()
            {
                ++m_index;
                return *this;
            }
            const_iterator& operator--()
            {
                --m_index;
                return *this;
            }
            const_iterator operator+(size_t offset) const
            {
                return { m_data, m_index + offset, m_size };
            }
            const_iterator operator-(size_t offset) const
            {
                return { m_data, m_index - offset, m_size };
            }
            difference_type operator-(const const_iterator& other) const
            {
                return m_index - other.m_index;
            }
            const_iterator operator+=(size_t offset)
            {
                m_index += offset;
                return *this;
            }
            const_iterator operator-=(size_t offset)
            {
                m_index -= offset;
                return *this;
            }
            bool operator!=(const const_iterator& other) const
            {
                return m_index != other.m_index;
            }
            bool operator==(const const_iterator& other) const
            {
                return m_index == other.m_index;
            }
            bool operator<(const const_iterator& other) const
            {
                return m_index < other.m_index;
            }
            bool operator<=(const const_iterator& other) const
            {
                return m_index <= other.m_index;
            }
            bool operator>(const const_iterator& other) const
            {
                return m_index > other.m_index;
            }
            bool operator>=(const const_iterator& other) const
            {
                return m_index >= other.m_index;
            }
            reference operator[](size_t offset) const
            {
                return m_data[(m_index + offset) % m_size];
            }
        };

        struct iterator
            : public const_iterator
        {
            using iterator_category = std::random_access_iterator_tag;
            using value_type        = T;
            using difference_type   = std::ptrdiff_t;
            using pointer           = T*;
            using reference         = T&;

            iterator(pointer data, size_t index, size_t size)
                : const_iterator{ data, index, size }
            {
            }

            reference operator*()
            {
                return const_cast<reference>(const_iterator::operator*());
            }
            pointer operator->()
            {
                return const_cast<pointer>(const_iterator::operator->());
            }

            iterator& operator++()
            {
                const_iterator::operator++();
                return *this;
            }
            iterator& operator--()
            {
                const_iterator::operator--();
                return *this;
            }
            iterator operator+(size_t offset) const
            {
                return const_iterator::operator+();
            }
            iterator operator-(size_t offset) const
            {
                return const_iterator::operator-();
            }
            difference_type operator-(const iterator& other) const
            {
                return const_iterator::operator-(other);
            }
            iterator operator+=(size_t offset)
            {
                const_iterator::operator+=(offset);
                return *this;
            }
            iterator operator-=(size_t offset)
            {
                const_iterator::operator-=(offset);
                return *this;
            }
            reference operator[](size_t offset)
            {
                return const_cast<reference>(const_iterator::operator[](offset));
            }
        };

        /// @brief バッファの先頭イテレーターを取得
        /// @return
        iterator begin()
        {
            return { m_data, m_head, Capacity };
        }

        /// @brief バッファの末尾イテレーターを取得
        /// @return
        iterator end()
        {
            return { m_data, m_head + m_size, Capacity };
        }

        /// @brief バッファの先頭イテレーターを取得
        /// @return
        const_iterator begin() const
        {
            return { m_data, m_head, Capacity };
        }
        /// @brief バッファの末尾イテレーターを取得
        /// @return
        const_iterator end() const
        {
            return { m_data, m_head + m_size, Capacity };
        }

        /// @brief バッファの先頭イテレーターを取得
        /// @return
        const_iterator cbegin() const
        {
            return { m_data, m_head, Capacity };
        }

        /// @brief バッファの末尾イテレーターを取得
        /// @return
        const_iterator cend() const
        {
            return { m_data, m_head + m_size, Capacity };
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
