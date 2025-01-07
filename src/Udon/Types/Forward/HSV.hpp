//
//    HSV色空間 前方定義
//
//    Copyright (c) 2022 udonrobo
//

#pragma once

#include <Udon/Traits/EnumerableMacro.hpp>
#include <Udon/Serializer/Serializer.hpp>

namespace Udon
{

    struct RGB;


    /// @brief HSV色空間
    struct HSV
    {

        /// @brief 要素の型
        using ValueType = uint8_t;

        /// @brief 色相
        ValueType h;

        /// @brief 彩度
        ValueType s;

        /// @brief 明度
        ValueType v;

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
        constexpr HSV(ValueType h, ValueType s, ValueType v) noexcept
            : h(h)
            , s(s)
            , v(v)
        {
        }

        /// @brief コンストラクタ
        /// @param hsv HSV色空間
        HSV(uint32_t hsv) noexcept
            : h(static_cast<ValueType>(hsv >> 16))
            , s(static_cast<ValueType>(hsv >> 8))
            , v(static_cast<ValueType>(hsv >> 0))
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
        friend constexpr bool operator==(const HSV& lhs, const HSV& rhs) noexcept
        {
            return lhs.h == rhs.h && lhs.s == rhs.s && lhs.v == rhs.v;
        }

        /// @brief 比較演算子
        friend constexpr bool operator!=(const HSV& lhs, const HSV& rhs) noexcept
        {
            return not(lhs == rhs);
        }

        explicit constexpr operator bool() const noexcept
        {
            return h or s or v;
        }

        /// @brief 24bit値へ変換
        /// @return
        uint32_t to24bit() const noexcept
        {
            return static_cast<uint32_t>(h) << 16 | static_cast<uint32_t>(s) << 8 | static_cast<uint32_t>(v) << 0;
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

        UDON_ENUMERABLE(h, s, v);
    };

}    // namespace Udon
