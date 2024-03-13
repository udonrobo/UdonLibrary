//
//    文字列参照クラス (std::string_view代替)
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 Udonrobo
//

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <Udon/Algorithm/StringToNumberParser.hpp>
#include <Udon/Common/Platform.hpp>

#if UDON_PLATFORM_OUTPUT_STREAM == UDON_PLATFORM_OUTPUT_CONSOLE
#    include <iostream>
#endif

namespace Udon
{

    /// @brief 文字列参照クラス
    /// @details メモリ所有権を持たないのでヒープへのメモリアロケーションが発生しません
    /// @tparam CharType 文字列の各要素の型
    /// @tparam Traits
    template <typename CharType, typename Traits = std::char_traits<CharType>>
    class BasicStringView
    {
        static_assert(not std::is_array<CharType>::value, "CharType cannot be an array.");

        static_assert(std::is_trivial<CharType>::value, "CharType must be a trivial type.");

        static_assert(std::is_standard_layout<CharType>::value, "CharType must be a standard layout type.");

    public:
        using size_type              = size_t;
        using traits_type            = Traits;
        using char_type              = typename traits_type::char_type;
        using int_type               = typename traits_type::int_type;
        using off_type               = typename traits_type::off_type;
        using pos_type               = typename traits_type::pos_type;
        using state_type             = typename traits_type::state_type;
        using const_reference        = const char_type&;
        using const_pointer          = const char_type*;
        using const_iterator         = const char_type*;
        using const_receive_iterator = std::reverse_iterator<const_iterator>;

        static constexpr size_type npos = static_cast<size_type>(-1);

    private:
        const_pointer m_data;    // ビューの先頭を指すポインタ
        size_type     m_size;    // ビューの要素数

    public:
        /// @brief デフォルトコンストラクタ
        constexpr BasicStringView()
            : m_data()
            , m_size()
        {
        }

        /// @brief 文字列の先頭を指すポインタをもとにビューを構築
        /// @remark null終端文字が必須です
        /// @param string
        BasicStringView(const_pointer string)
            : m_data(string)
            , m_size(strlen(string))
        {
        }

        /// @brief 文字列の先頭を指すポインタ、サイズをもとにビューを構築
        /// @param string 先頭を指すポインタ
        /// @param size 要素数
        constexpr BasicStringView(const_pointer string, const size_type length)
            : m_data(string)
            , m_size(length)
        {
        }

        /// @brief 要素を指すイテレータをもとにビューを構築
        /// @tparam InputIterator STLの入力イテレータ要件に則す必要ある
        /// @param begin
        /// @param end
        template <typename InputIterator, typename = typename std::enable_if<std::is_convertible<typename std::iterator_traits<InputIterator>::iterator_category, std::input_iterator_tag>::value>::type>
        constexpr BasicStringView(InputIterator begin, InputIterator end)
            : m_data(begin)
            , m_size(std::distance(begin, end))
        {
        }

        /// @brief std::string からの変換
        /// @remark 一時オブジェクトからの変換は禁止(ダングリングポインタになる)
        /// @param string
        BasicStringView(const std::basic_string<CharType>& string)
            : m_data(string.data())
            , m_size(string.size())
        {
        }

        
#ifdef ARDUINO

        /// @brief Arduino の String からの変換
        /// @note T が char の場合のみ有効
        /// @param str
        template <typename T = CharType, typename = typename std::enable_if<std::is_same<T, char>::value>::type>
        BasicStringView(const String& str)
            : m_data(str.c_str())
            , m_size(str.length())
        {
        }
#endif

        /// @brief デフォルトコピーコンストラクタ
        BasicStringView(const BasicStringView&) = default;

        /// @brief デフォルト代入演算子
        BasicStringView& operator=(const BasicStringView&) = default;

        /// @brief ビューが要素を持つか
        constexpr explicit operator bool() const noexcept
        {
            return m_size;
        }

        /// @brief ビューへのポインタを取得する
        /// @return
        constexpr const_pointer data() const noexcept
        {
            return m_data;
        }

        /// @brief ビューのサイズを取得する
        /// @return
        constexpr size_type size() const noexcept
        {
            return m_size;
        }

        /// @brief ビュー終端にヌル終端文字を含むことは保証されないので c_str() は提供しない
        // const_pointer c_str() const noexcept = delete;

        /// @brief ビューのサイズが0か判定
        /// @return
        constexpr bool empty() const noexcept
        {
            return m_size == 0;
        }

        /// @brief 指定されたインデックスの文字を取得する
        /// @param index
        /// @return
        constexpr const_reference operator[](const size_type index) const noexcept
        {
            return m_data[index];
        }

        /// @brief 指定されたインデックスの文字を取得する
        const_reference at(const size_type index) const noexcept
        {
            if (index >= m_size)
            {
                // throw std::out_of_range
                return back();
            }
            return m_data[index];
        }

        /// @brief ビューの先頭文字を取得する
        /// @return
        constexpr const_reference front() const noexcept
        {
            return m_data[0];
        }

        /// @brief ビューの終端の文字を取得する
        /// @return
        constexpr const_reference back() const noexcept
        {
            return m_data[m_size - 1];
        }

        /// @brief 指定された範囲のビューを作成する
        /// @param beginIndex 開始位置
        /// @param endIndex 終端位置(ビューに含まれない)
        /// @return 作成されたビュー
        BasicStringView substring(const size_type beginIndex, const size_type endIndex = npos) const
        {
            if (beginIndex >= m_size)
            {
                return {};
            }
            if (endIndex == npos)
            {
                return {
                    std::next(cbegin(), beginIndex),
                    m_size - beginIndex
                };
            }
            if (endIndex <= beginIndex)
            {
                return {};
            }
            return {
                std::next(cbegin(), beginIndex),
                std::next(cbegin(), std::min(endIndex, m_size)),
            };
        }

        /// @brief 指定された終端文字までのビューを作成する
        /// @param terminate 終端文字(ビューに含まれない)
        /// @remark 開始位置は現在のビューの開始位置
        /// @return 作成されたビュー
        BasicStringView substringUntil(const char_type terminate) const
        {
            return {
                cbegin(),
                std::find(cbegin(), cend(), terminate)
            };
        }

        /// @brief 先頭のN文字を削除したビューを作成する
        /// @param n 削除する文字数
        /// @return 作成されたビュー
        BasicStringView removePrefix(const size_type n) const
        {
            if (n >= m_size)
            {
                return {};
            }
            return {
                std::next(cbegin(), n),
                m_size - n
            };
        }

        /// @brief 末尾のN文字を削除したビューを作成する
        /// @param n 削除する文字数
        /// @return 作成されたビュー
        BasicStringView removeSuffix(const size_type n) const
        {
            if (n >= m_size)
            {
                return {};
            }
            return {
                cbegin(),
                m_size - n
            };
        }

        /// @brief ビューが指定したビューから始まっているか判定する
        /// @param string 判定するビュー
        /// @return true: 始まっている false: 始まっていない
        bool startsWith(const BasicStringView& string) const noexcept
        {
            return m_size >= string.m_size and traits_type::compare(m_data, string.m_data, string.m_size) == 0;
        }

        /// @brief ビューが指定したビューで終わっているか判定する
        /// @param string 判定するビュー
        /// @return
        bool endsWith(const BasicStringView& string) const noexcept
        {
            return m_size >= string.m_size and traits_type::compare(m_data + m_size - string.m_size, string.m_data, string.m_size) == 0;
        }

        /// @brief 指定された区切り文字で区切り、ビューのリストを作成する
        /// @param delimiter 区切り文字
        /// @return ビューのリスト
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

        /// @brief  ビューを数値に変換する
        /// @tparam T 変換する数値の型
        /// @param  radix 基数(整数のみ)
        /// @return 変換された数値 (変換できない場合はnullopt)
        template <typename T = int>
        Udon::Optional<T> toNumber(const int radix = 10) const
        {
            static_assert(std::is_arithmetic<T>::value, "T must be arithmetic type.");

            return StringToNumberParser::Parse<T>(cbegin(), cend(), radix);
        }

        /// @brief STL の文字列に変換する
        /// @remark メモリアロケーションが発生します。
        /// @return 文字列
        std::basic_string<char_type> toString() const
        {
            return { cbegin(), cend() };
        }

        /// @brief ビューを入れ替える
        /// @param other 入れ変え先
        void swap(BasicStringView& other) noexcept
        {
            std::swap(m_data, other.m_data);
            std::swap(m_size, other.m_size);
        }

        /// @brief ビューが一致するか比較する
        /// @param lhs 被演算子
        /// @param rhs 被演算子
        /// @return 一致するかどうか
        friend bool operator==(const BasicStringView& lhs, const BasicStringView& rhs) noexcept
        {
            return lhs.m_size == rhs.m_size and traits_type::compare(lhs.m_data, rhs.m_data, lhs.m_size) == 0;
        }

        /// @brief ビューが不一致であるか比較する
        /// @param lhs 被演算子
        /// @param rhs 被演算子
        /// @return 不一致かどうか
        friend bool operator!=(const BasicStringView& lhs, const BasicStringView& rhs) noexcept
        {
            return not(lhs == rhs);
        }

#if UDON_PLATFORM_OUTPUT_STREAM == UDON_PLATFORM_OUTPUT_CONSOLE

        /// @brief std::ostream への出力
        friend std::basic_ostream<char_type>& operator<<(std::basic_ostream<char_type>& os, const BasicStringView& string)
        {
            return os.write(string.m_data, string.m_size);
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
            Serial.print("]");
        }
        void showString() const
        {
            Serial.write(m_data, m_size);
        }
#endif

        // iterator 要件
        const_iterator begin() const noexcept { return m_data; }
        const_iterator end() const noexcept { return m_data + m_size; }

        const_iterator cbegin() const noexcept { return m_data; }
        const_iterator cend() const noexcept { return m_data + m_size; }

        const_receive_iterator rbegin() const noexcept { return const_receive_iterator(end()); }
        const_receive_iterator rend() const noexcept { return const_receive_iterator(begin()); }

        const_receive_iterator crbegin() const noexcept { return const_receive_iterator(cend()); }
        const_receive_iterator crend() const noexcept { return const_receive_iterator(cbegin()); }
    };

}    // namespace Udon

namespace Udon
{
    using StringView  = BasicStringView<char>;
    using WStringView = BasicStringView<wchar_t>;
}    // namespace Udon

namespace Udon
{

    namespace Literals
    {

        /// @brief Udon::StringView 構築リテラル
        constexpr StringView operator""_sv(const char* string, size_t length) noexcept
        {
            return { string, length };
        }

        /// @brief Udon::WStringView 構築リテラル
        constexpr WStringView operator""_sv(const wchar_t* string, size_t length) noexcept
        {
            return { string, length };
        }

    }    // namespace Literals

}    // namespace Udon