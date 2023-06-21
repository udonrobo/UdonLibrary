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

    struct Color
    {

        /// @brief 要素の型
        using value_type = uint8_t;

        /// @brief 赤成分
        value_type r;

        /// @brief 緑成分
        value_type g;

        /// @brief 青成分
        value_type b;

        constexpr Color() noexcept
            : r()
            , g()
            , b()
        {
        }

        constexpr Color(value_type r, value_type g, value_type b) noexcept
            : r(r)
            , g(g)
            , b(b)
        {
        }

        constexpr Color(const Color&) = default;

        constexpr Color& operator=(const Color&) = default;

        constexpr operator==(const Color& rhs) const noexcept
        {
            return r == rhs.r && g == rhs.g && b == rhs.b;
        }

        constexpr operator!=(const Color& rhs) const noexcept
        {
            return !(*this == rhs);
        }

        explicit constexpr operator bool() const noexcept
        {
            return r | g | b;
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
