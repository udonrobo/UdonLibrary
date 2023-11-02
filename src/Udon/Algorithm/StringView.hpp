#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include "StringToNumberParser.hpp"

namespace Udon
{
    template <typename CharType, typename Traits = std::char_traits<CharType>>
    class BasicStringView
    {
    public:
        using size_type              = size_t;
        using traits_type            = Traits;
        using char_type              = typename traits_type::char_type;
        using int_type               = typename traits_type::int_type;
        using off_type               = typename traits_type::off_type;
        using pos_type               = typename traits_type::pos_type;
        using state_type             = typename traits_type::state_type;
        using const_reference        = const CharType&;
        using const_pointer          = const CharType*;
        using const_iterator         = const CharType*;
        using const_receive_iterator = std::reverse_iterator<const_iterator>;

    private:
        const_pointer m_data;
        size_type     m_size;

    public:
        BasicStringView()
            : m_data()
            , m_size()
        {
        }

        BasicStringView(const_pointer string)
            : m_data(string)
            , m_size(strlen(string))
        {
        }

        BasicStringView(const_pointer string, const size_type size)
            : m_data(string)
            , m_size(size)
        {
        }

        BasicStringView(const_iterator begin, const_iterator end)
            : m_data(begin)
            , m_size(end - begin)
        {
        }

        BasicStringView(const BasicStringView&) = default;

        BasicStringView& operator=(const BasicStringView&) = default;

        explicit operator bool() const noexcept
        {
            return m_size;
        }

        const_pointer data() const noexcept
        {
            return m_data;
        }

        // ヌル終端文字が含まれることは保証されないので、c_str() は提供しない。
        // const_pointer c_str() const noexcept;

        size_type length() const noexcept
        {
            return m_size;
        }

        size_type size() const noexcept
        {
            return m_size;
        }

        bool empty() const noexcept
        {
            return m_size == 0;
        }

        const_reference operator[](const size_type index) const noexcept
        {
            return m_data[index];
        }

        const_reference at(const size_type index) const noexcept
        {
            if (index >= m_size)
            {
                // throw std::out_of_range
                return back();
            }
            return m_data[index];
        }

        const_reference front() const noexcept
        {
            return m_data[0];
        }

        const_reference back() const noexcept
        {
            return m_data[m_size - 1];
        }

        BasicStringView substring(const size_type beginIndex, const size_type endIndex) const
        {
            return BasicStringView{
                std::next(cbegin(), beginIndex),
                std::next(cbegin(), std::min(endIndex, m_size)),
            };
        }

        BasicStringView substring(const size_type beginIndex) const
        {
            return substring(beginIndex, m_size);
        }

        // まで
        BasicStringView substringUntil(const char_type terminate) const
        {
            return {
                cbegin(),
                std::find(cbegin(), cend(), terminate)
            };
        }

        // から
        BasicStringView substringFrom(const char_type terminate) const
        {
            return {
                std::next(std::find(cbegin(), cend(), terminate)),    // find()はterminateを指しているので、次の文字から
                cend()
            };
        }

        bool startsWith(const BasicStringView& string) const noexcept
        {
            return m_size >= string.m_size and
                   std::equal(string.cbegin(), string.cend(), cbegin());
        }

        bool endsWith(const BasicStringView& string) const noexcept
        {
            return m_size >= string.m_size and
                   std::equal(string.crbegin(), string.crend(), crbegin());
        }

        std::vector<BasicStringView> split(const char_type delimiter) const
        {
            std::vector<BasicStringView> tokens;

            const_iterator begin = cbegin();
            for (auto it = cbegin(); it != cend(); ++it)
            {
                const auto next = std::next(it);
                if (*it == delimiter)    // 区切り文字検索
                {
                    tokens.emplace_back(begin, it);
                    begin = next;    // it は 区切り文字を指しているので飛ばす
                }
                else if (next == cend())    // 最後は普通に追加
                {
                    tokens.emplace_back(begin, cend());
                }
            }
            return tokens;
        }

        template <typename T>
        T parse() const
        {
            return StringToNumberParser<T>::Parse({ cbegin(), cend() });
        }

        void swap(BasicStringView& other) noexcept
        {
            std::swap(m_data, other.m_data);
            std::swap(m_size, other.m_size);
        }

        // 比較演算子
        friend bool operator==(const BasicStringView& lhs, const BasicStringView& rhs) noexcept
        {
            return lhs.m_size == rhs.m_size and
                   std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
        }
        friend bool operator!=(const BasicStringView& lhs, const BasicStringView& rhs) noexcept
        {
            return not(lhs == rhs);
        }

        // std::ostream への出力
        friend std::ostream& operator<<(std::ostream& os, const BasicStringView& string)
        {
            return os.write(string.m_data, string.m_size);
        }

        // iterator 要件
        const_iterator begin() const noexcept { return m_data; }
        const_iterator end() const noexcept { return m_data + m_size; }

        const_iterator cbegin() const noexcept { return m_data; }
        const_iterator cend() const noexcept { return m_data + m_size; }

        const_receive_iterator rbegin() const noexcept { return const_receive_iterator(end()); }
        const_receive_iterator rend() const noexcept { return const_receive_iterator(begin()); }

        const_receive_iterator crbegin() const noexcept { return const_receive_iterator(cend()); }
        const_receive_iterator crend() const noexcept { return const_receive_iterator(cbegin()); }

        // char traits 要件 (https://cpprefjp.github.io/reference/string/char_traits.html)
        static constexpr void assign(const_pointer dest, const size_type size, const char_type& ch)
        {
            traits_type::assign(dest, size, ch);
        }
        static constexpr bool eq(const char_type& lhs, const char_type& rhs)
        {
            return traits_type::eq(lhs, rhs);
        }
        static constexpr bool lt(const char_type& lhs, const char_type& rhs)
        {
            return traits_type::lt(lhs, rhs);
        }
        static constexpr int compare(const_pointer lhs, const_pointer rhs, const size_type size)
        {
            return traits_type::compare(lhs, rhs, size);
        }
        static constexpr size_type length(const_pointer string)
        {
            return traits_type::length(string);
        }
        static constexpr const_pointer find(const_pointer string, const size_type size, const char_type& ch)
        {
            return traits_type::find(string, size, ch);
        }
        static constexpr const_pointer move(const_pointer dest, const_pointer src, const size_type size)
        {
            return traits_type::move(dest, src, size);
        }
        static constexpr const_pointer copy(const_pointer dest, const_pointer src, const size_type size)
        {
            return traits_type::copy(dest, src, size);
        }
    };

}    // namespace Udon

#include <string>

namespace Udon
{
    using StringView  = BasicStringView<char>;
    using WStringView = BasicStringView<wchar_t>;
}    // namespace Udon
