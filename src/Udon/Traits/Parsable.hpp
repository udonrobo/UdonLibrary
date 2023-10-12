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
//    メンバ変数のパースを可能にするマクロ
//
//-------------------------------------------------------------------

#pragma once

#include "PackedSizable.hpp"
#include "Accessible.hpp"

namespace Udon
{

    namespace Traits
    {
        template <typename, typename = void>
        struct HasMemberFunctionParsable
            : std::false_type
        {
        };
        template <typename T>
        struct HasMemberFunctionParsable<T, typename std::enable_if<std::is_same<
                                                decltype(std::declval<T>().parsable()), bool>::value>::type>
            : std::true_type
        {
        };

        template <typename, typename = void>
        struct ParsableInvocable
            : std::false_type
        {
        };
        template <typename T>
        struct ParsableInvocable<T, typename std::enable_if<std::is_same<
                                        decltype(Parsable(std::declval<const T&>())), bool>::value>::type>
            : std::true_type
        {
        };

        template <typename HasMemberFunctionParsable, typename std::enable_if<Traits::HasMemberFunctionParsable<HasMemberFunctionParsable>::value, std::nullptr_t>::type = nullptr>
        inline constexpr bool InvokeParsable(const HasMemberFunctionParsable& rhs)
        {
            return rhs.parsable();
        }
        template <typename ParsableInvocable, typename std::enable_if<Traits::ParsableInvocable<ParsableInvocable>::value, std::nullptr_t>::type = nullptr>
        inline constexpr bool InvokeParsable(const ParsableInvocable& rhs)
        {
            return Parsable(rhs);
        }

        template <typename T, typename = void>
        struct MemberParsable
            : std::false_type
        {
        };
        template <typename T>
        struct MemberParsable<T, typename std::enable_if<
                                     HasMemberFunctionParsable<T>::value ^ ParsableInvocable<T>::value>::type>
        {
            static constexpr bool value = InvokeParsable(T{});
        };
        template <typename T>
        struct MemberParsable<T, typename std::enable_if<
                                     HasMemberFunctionParsable<T>::value && ParsableInvocable<T>::value>::type>
            : std::false_type
        {
            static_assert(AlwaysFalse<T>::value, "T has both member function parsable() and global function Parsable().");    // メンバ関数とグローバル関数の両方が定義されている場合はコンパイルエラー
        };

        /// @brief T がパース可能であるか
        template <typename, typename = void>
        struct Parsable
            : std::false_type
        {
        };
        template <typename T>
        struct Parsable<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>    // 算術型
            : std::true_type
        {
        };
        template <typename T>
        struct Parsable<T, typename std::enable_if<std::is_enum<T>::value>::type>    // 列挙型
            : std::true_type
        {
		};
        template <typename T>
        struct Parsable<T, typename std::enable_if<PackedSizable<T>::value &&            // バッファサイズを返すメンバー関数を持つ
                                                   Accessible<T>::value &&             // アクセッサを持つ
                                                   MemberParsable<T>::value>::type>    // 各メンバがパース可能である
            : std::true_type
        {
        };
        template <typename T, size_t N>
        struct Parsable<T[N]>    // 配列
            : Parsable<T>
        {
        };

        /// @brief メンバ変数がパース可能であるか
        template <typename Head>
        inline constexpr bool IsMemberParsable(const Head&)
        {
            return Parsable<Head>::value;
        }
        template <typename Head, typename... Tails>
        inline constexpr bool IsMemberParsable(const Head& head, const Tails&... tails)
        {
            return IsMemberParsable(head) && IsMemberParsable(tails...);
        }

    }    // namespace Traits

}    // namespace Udon
