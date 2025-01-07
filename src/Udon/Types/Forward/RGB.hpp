//
//    RGB色空間 前方定義
//
//    Copyright (c) 2022 udonrobo
//

#pragma once

#include <Udon/Traits/EnumerableMacro.hpp>
#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Traits/Typedef.hpp>

namespace Udon
{

    struct HSV;


    /// @brief RGB色空間
    struct RGB
    {

        /// @brief 要素の型
        using ValueType = uint8_t;

        /// @brief 赤成分
        ValueType r;

        /// @brief 緑成分
        ValueType g;

        /// @brief 青成分
        ValueType b;

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
        constexpr RGB(ValueType r, ValueType g, ValueType b) noexcept
            : r(r)
            , g(g)
            , b(b)
        {
        }

        /// @brief コンストラクタ
        /// @param rgb RGB色空間
        RGB(uint32_t rgb) noexcept
            : r(static_cast<ValueType>(rgb >> 16))
            , g(static_cast<ValueType>(rgb >> 8))
            , b(static_cast<ValueType>(rgb >> 0))
        {
        }

        /// @brief 変換コンストラクタ
        /// @param hsv HSV色空間
        RGB(const HSV& hsv) noexcept;

        /// @brief コピーコンストラクタ
        RGB(const RGB&) = default;

        /// @brief コピー代入演算子
        RGB& operator=(const RGB&) = default;

        template <typename FloatingPoint, Traits::EnableIfNullptrT<Traits::IsFloatingPoint<FloatingPoint>::value> = nullptr>
        constexpr RGB operator*(FloatingPoint rhs) const noexcept
        {
            return {
                static_cast<ValueType>(r * rhs),
                static_cast<ValueType>(g * rhs),
                static_cast<ValueType>(b * rhs)
            };
        }

        template <typename FloatingPoint, Traits::EnableIfNullptrT<Traits::IsFloatingPoint<FloatingPoint>::value> = nullptr>
        constexpr RGB operator/(FloatingPoint rhs) const noexcept
        {
            return {
                static_cast<ValueType>(r / rhs),
                static_cast<ValueType>(g / rhs),
                static_cast<ValueType>(b / rhs)
            };
        }

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

        /// @brief 24bit値への変換
        /// @return
        uint32_t to24bit() const noexcept
        {
            return (static_cast<uint32_t>(r) << 16) | (static_cast<uint32_t>(g) << 8) | (static_cast<uint32_t>(b) << 0);
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

        UDON_ENUMERABLE(r, g, b);
    };

}    // namespace Udon
