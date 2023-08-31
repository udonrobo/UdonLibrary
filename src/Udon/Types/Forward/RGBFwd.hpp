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
//    RGB色空間 前方定義
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Traits/ParsableMacro.hpp>
#include <Udon/Com/Serialization.hpp>

namespace Udon
{

    struct HSV;

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

        /// @brief デフォルトコンストラクタ
        constexpr RGB() noexcept
            : r()
            , g()
            , b()
        {
        }

        /// @brief コンストラクタ
        /// @param r 赤成分
        /// @param g 緑成分
        /// @param b 青成分
        constexpr RGB(value_type r, value_type g, value_type b) noexcept
            : r(r)
            , g(g)
            , b(b)
        {
        }

        /// @brief コンストラクタ
        /// @param rgb RGB色空間
        RGB(uint32_t rgb) noexcept
            : r(rgb >> 16)
            , g(rgb >> 8)
            , b(rgb >> 0)
        {
        }

        /// @brief 変換コンストラクタ
        /// @param hsv HSV色空間
        RGB(const HSV& hsv) noexcept;

        /// @brief コピーコンストラクタ
        RGB(const RGB&) = default;

        /// @brief コピー代入演算子
        RGB& operator=(const RGB&) = default;   

        /// @brief 比較演算子
        constexpr bool operator==(const RGB& rhs) const noexcept
        {
            return r == rhs.r && g == rhs.g && b == rhs.b;
        }

        /// @brief 比較演算子
        constexpr bool operator!=(const RGB& rhs) const noexcept
        {
            return !(*this == rhs);
        }

        explicit constexpr operator bool() const noexcept
        {
            return r || g || b;
        }

        /// @brief HSV色空間に変換
        /// @return HSV色空間
        HSV toHSV() const noexcept;

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

}    // namespace Udon
