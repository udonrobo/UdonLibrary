//
//    クオータニオン 前方定義
//
//    Copyright (c) 2022-2024 udonrobo
//


#pragma once

#include <Udon/Traits/EnumerableMacro.hpp>
#include <Udon/Serializer/Serializer.hpp>

namespace Udon
{

    struct Euler;


    /// @brief クオータニオンの各成分の正負を表す構造体
    struct QuaternionDirection
    {
        bool x;
        bool y;
        bool z;
    };


    /// @brief クオータニオン
    struct Quaternion
    {

        /// @brief 要素の型
        using ValueType = double;

        ValueType x;
        ValueType y;
        ValueType z;
        ValueType w;

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
        constexpr Quaternion(ValueType x, ValueType y, ValueType z, ValueType w) noexcept
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
            return {
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
        constexpr Quaternion inverse() const noexcept
        {
            return { -x, -y, -z, w };
        }

        /// @brief 回転方向を修正したクオータニオンを取得する
        /// @param direction 回転方向
        /// @return 修正後のクオータニオン
        constexpr Quaternion directionRevision(const QuaternionDirection& direction) const noexcept
        {
            return {
                x * (direction.x ? 1 : -1),
                y * (direction.y ? 1 : -1),
                z * (direction.z ? 1 : -1),
                w
            };
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
        static Quaternion RotateX(ValueType angle) noexcept
        {
            return { sin(angle / 2), 0, 0, cos(angle / 2) };
        }

        /// @brief Y軸回転クオータニオン
        /// @param angle 回転角度
        /// @return
        static Quaternion RotateY(ValueType angle) noexcept
        {
            return { 0, sin(angle / 2), 0, cos(angle / 2) };
        }

        /// @brief Z軸回転クオータニオン
        /// @param angle 回転角度
        /// @return
        static Quaternion RotateZ(ValueType angle) noexcept
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

        /// @brief オイラー角に変換
        /// @return オイラー角
        Euler toEuler() const noexcept;

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

        /// @brief s3d::Quaternion からの変換
        Quaternion(const s3d::Quaternion& q) noexcept
            : x(q.x)
            , y(q.y)
            , z(q.z)
            , w(q.w)
        {
        }

        /// @brief s3d::Quaternion への変換
        /// @param v s3d::Quaternion
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

        UDON_ENUMERABLE(x, y, z, w)
    };
}    // namespace Udon