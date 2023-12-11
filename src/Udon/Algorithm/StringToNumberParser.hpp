//
//    文字列を数値に変換する関数群
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 Udonrobo
//

#pragma once

#include <Udon/Traits/AlwaysFalse.hpp>
#include <Udon/Stl/Optional.hpp>

#include <string>
#include <stdlib.h>

namespace Udon
{
    namespace StringToNumberParser
    {

        /// @brief 文字列を数値に変換する
        /// @tparam T 変換後の数値の型
        /// @param begin 変換する文字列の先頭
        /// @param end   変換する文字列の末尾
        /// @param radix 基数
        /// @return Udon::Optional<T> 変換後の数値 (変換できなかった場合は nullopt)
        template <typename T>
        inline Udon::Optional<T> Parse(const char* const begin, const char* const end, const int radix)
        {
            static_assert(Traits::AlwaysFalse<T>::value, "T is not parsable.");
        }

        template <>
        inline Udon::Optional<long> Parse<long>(const char* const begin, const char* const end, const int radix)
        {
            char* endPtr = nullptr;

            const auto result = strtol(begin, &endPtr, radix);

            // Arduino nano では errno が定義されていないため、errno によるエラー判定は行わない

            if (endPtr == end)
            {
                return result;
            }
            else
            {
                return Udon::nullopt;
            }
        }

        template <>
        inline Udon::Optional<int> Parse<int>(const char* const begin, const char* const end, const int radix)
        {
            return Parse<long>(begin, end, radix);
        }

        template <>
        inline Udon::Optional<double> Parse<double>(const char* const begin, const char* const end, const int /*radix*/)
        {
            char* endPtr = nullptr;

            const auto result = strtod(begin, &endPtr);

            if (endPtr == end)
            {
                return result;
            }
            else
            {
                return Udon::nullopt;
            }
        }

        template <>
        inline Udon::Optional<float> Parse<float>(const char* const begin, const char* const end, const int /*radix*/)
        {
            return Parse<double>(begin, end, 10);
        }

    }    // namespace StringToNumberParser

}    // namespace Udon