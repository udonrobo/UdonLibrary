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
//    HSV色空間 前方定義
//
//-------------------------------------------------------------------


#pragma once

#include <Udon/Utility/Parsable.hpp>
 #include <Udon/Com/Serialization.hpp>

namespace Udon
{
    struct RGB;

    struct HSV
    {

        /// @brief 要素の型
        using value_type = uint8_t;

        /// @brief 色相
        value_type h;

        /// @brief 彩度
        value_type s;

        /// @brief 明度
        value_type v;

        /// @brief デフォルトコンストラクタ
        constexpr HSV() noexcept
            : h()
            , s()
            , v()
        {
        }

        /// @brief コンストラクタ
        /// @param h 色相
        /// @param s 彩度
        /// @param v 明度
        constexpr HSV(value_type h, value_type s, value_type v) noexcept
            : h(h)
            , s(s)
            , v(v)
        {
        }

        /// @brief コンストラクタ
        /// @param hsv HSV色空間
        HSV(uint32_t hsv) noexcept
            : h(hsv >> 16)
            , s(hsv >> 8)
            , v(hsv >> 0)
        {
        }

        /// @brief 変換コンストラクタ
        /// @param rgb RGB色空間
        HSV(const RGB& rgb) noexcept;

        /// @brief コピーコンストラクタ
        HSV(const HSV&) = default;

        /// @brief コピー代入演算子
        HSV& operator=(const HSV&) = default;

        /// @brief 比較演算子
        constexpr bool operator==(const HSV& rhs) const noexcept
        {
            return h == rhs.h && s == rhs.s && v == rhs.v;
        }

        /// @brief 比較演算子
        constexpr bool operator!=(const HSV& rhs) const noexcept
        {
            return !(*this == rhs);
        }

        explicit constexpr operator bool() const noexcept
        {
            return h || s || v;
        }

        /// @brief RGB色空間に変換
        /// @return RGB色空間
        RGB toRGB() const noexcept;

#ifdef ARDUINO
        /// @brief デバッグ出力
        void show() const
        {
            Serial.print(F("h: ")), Serial.print(h), Serial.print('\t');
            Serial.print(F("s: ")), Serial.print(s), Serial.print('\t');
            Serial.print(F("v: ")), Serial.print(v), Serial.print('\t');
        }
#endif

        UDON_PARSABLE(h, s, v);

    };
    
} // namespace Udon
