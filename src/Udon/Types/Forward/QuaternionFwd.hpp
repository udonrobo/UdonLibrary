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
//    クオータニオン 前方定義
//
//-------------------------------------------------------------------


#pragma once

#include <Udon/Utility/Parsable.hpp>
#include <Udon/Com/Serialization.hpp>

namespace Udon
{

    template <typename T>
    struct Euler3D;

    struct Quaternion
    {

        /// @brief 要素の型
        using value_type = double;

        value_type x;
        value_type y;
        value_type z;
        value_type w;

        /// @brief デフォルトコンストラクタ
        constexpr Quaternion() noexcept
            : x()
            , y()
            , z()
            , w()
        {
        }

        /// @brief コンストラクタ
        /// @param x x成分
        /// @param y y成分
        constexpr Quaternion(value_type x, value_type y, value_type z, value_type w) noexcept
            : x(x)
            , y(y)
            , z(z)
            , w(w)
        {
        }

        /// @brief デフォルトコピーコンストラクタ
        Quaternion(const Quaternion&) = default;

        /// @brief デフォルトコピー代入演算子
        Quaternion& operator=(const Quaternion&) = default;

        /// @brief 内積
        /// @param rhs 被演算子
        /// @return
        constexpr Quaternion operator*(const Quaternion& rhs) const noexcept
        {
            return 
            {
                w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
                w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
                w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w,
                w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z,
            };
        }

        /// @brief 複合代入演算子
        /// @param rhs 被演算子
        /// @return
        Quaternion& operator*=(const Quaternion& rhs) noexcept { return *this = *this * rhs; };

        /// @brief 比較演算子
        /// @param rhs 被演算子
        /// @return
        constexpr bool operator==(const Quaternion& rhs) const noexcept
        {
            return x == rhs.x &&
                   y == rhs.y &&
                   z == rhs.z &&
                   w == rhs.w;
        };
        constexpr bool operator!=(const Quaternion& rhs) const noexcept
        {
            return !(*this == rhs);
        };

        /// @brief 要素のいずれかに0以外の値があるかどうかを返す
        /// @return
        explicit constexpr operator bool() const noexcept
        {
            return x || y || z || w;
        }

        /// @brief 逆クオータニオン
        /// @return 
        constexpr Quaternion inverce() const noexcept
        {
            return { -x, -y, -z, w };
        }

        /// @brief 単位クオータニオン
        /// @return 
        static Quaternion Identity() noexcept
        {
            return { 0, 0, 0, 1 };
        }

        /// @brief X軸回転クオータニオン
        /// @param angle 回転角度
        /// @return
        static Quaternion RotateX(value_type angle) noexcept
        {
            return { sin(angle / 2), 0, 0, cos(angle / 2) };
        }

        /// @brief Y軸回転クオータニオン
        /// @param angle 回転角度
        /// @return
        static Quaternion RotateY(value_type angle) noexcept
        {
            return { 0, sin(angle / 2), 0, cos(angle / 2) };
        }

        /// @brief Z軸回転クオータニオン
        /// @param angle 回転角度
        /// @return
        static Quaternion RotateZ(value_type angle) noexcept
        {
            return { 0, 0, sin(angle / 2), cos(angle / 2) };
        }

        /// @brief 要素がゼロであるか
        constexpr bool isZero() const noexcept
        {
            return !operator bool();
        }

        /// @brief 値クリア
        void clear() noexcept
        {
            *this = {};
        }

        template <typename T = value_type>
        Euler3D<T> toEuler() const noexcept;

        /// @brief ヨー角を取得
        double toYaw() const noexcept
        {
            return atan2(2 * (w * z + x * y), 1 - 2 * (y * y + z * z));
        }

        /// @brief ピッチ角を取得
        double toPitch() const noexcept
        {
            return asin(2 * (w * y - z * x));
        }

        /// @brief ロール角を取得
        double toRoll() const noexcept
        {
            return atan2(2 * (w * x + y * z), 1 - 2 * (x * x + y * y));
        }

#ifdef SIV3D_INCLUDED
        /// @brief s3d::Vec2からの変換コンストラクタ
        /// @param v s3d::Vec2
        operator s3d::Quaternion() const noexcept
        {
            return s3d::Quaternion(x, y, z, w);
        }
#endif

#ifdef ARDUINO
        /// @brief デバッグ出力
        void show() const
        {
            Serial.print(F("x: ")), Serial.print(x), Serial.print('\t');
            Serial.print(F("y: ")), Serial.print(y), Serial.print('\t');
            Serial.print(F("z: ")), Serial.print(z), Serial.print('\t');
            Serial.print(F("w: ")), Serial.print(w), Serial.print('\t');
        }
#endif

        UDON_PARSABLE(x, y, z, w)
    };
}    // namespace Udon