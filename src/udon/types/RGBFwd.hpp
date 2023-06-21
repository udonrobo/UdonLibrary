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
//  RGB色空間 前方定義
//
//-----------------------------------------------

#pragma once

#include <udon/utility/Parsable.hpp>

namespace udon
{

    struct RGB
    {

        /// @brief 要素の型
        using value_type = uint8_t;

        /// @brief 赤成分
        value_type r;

        /// @brief 緑成分
        value_type g;

        /// @brief 青成分
        value_type b;

        constexpr RGB() noexcept
            : r()
            , g()
            , b()
        {
        }

        constexpr RGB(value_type r, value_type g, value_type b) noexcept
            : r(r)
            , g(g)
            , b(b)
        {
        }

        RGB(uint32_t rgb) noexcept
            : r(rgb >> 16)
            , g(rgb >> 8)
            , b(rgb >> 0)
        {
        }

        RGB(const RGB&) = default;

        RGB& operator=(const RGB&) = default;

        constexpr bool operator==(const RGB& rhs) const noexcept
        {
            return r == rhs.r && g == rhs.g && b == rhs.b;
        }

        constexpr bool operator!=(const RGB& rhs) const noexcept
        {
            return !(*this == rhs);
        }

        explicit constexpr operator bool() const noexcept
        {
            return r || g || b;
        }

#ifdef ARDUINO
        /// @brief デバッグ出力
        void show() const
        {
            Serial.print(F("r: ")), Serial.print(r), Serial.print('\t');
            Serial.print(F("g: ")), Serial.print(g), Serial.print('\t');
            Serial.print(F("b: ")), Serial.print(b), Serial.print('\t');
        }
#endif

        UDON_PARSABLE(r, g, b);
    };

}    // namespace udon
