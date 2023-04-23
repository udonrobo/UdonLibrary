#pragma once

#include <udon/math/Math.hpp>  // udon::NormalizedAngle
#include <udon/com/serializer/Serializer.hpp>

namespace udon
{

    template<typename T>
	class Euler3D
	{

    public:

        /// @brief 要素の型
		using value_type = T;

        /// @brief ロール角
        value_type roll;

        /// @brief ピッチ角
        value_type pitch;

        /// @brief ヨー角
        value_type yaw;

		/// @brief デフォルトコンストラクタ
        constexpr Euler3D() noexcept
            : roll ()
            , pitch()
            , yaw  ()
        {}

        /// @brief デフォルトコピーコンストラクタ
        constexpr Euler3D(const Euler3D& rhs) noexcept
            : roll (rhs.roll )
            , pitch(rhs.pitch)
			, yaw  (rhs.yaw  )
        {}

        /// @brief コンストラクタ
        /// @param roll ロール角
        /// @param pitch ピッチ角
        /// @param yaw ヨー角
        constexpr Euler3D(value_type roll, value_type pitch, value_type yaw) noexcept
            : roll (roll )
            , pitch(pitch)
			, yaw  (yaw  )
        {
        }

        /// @brief デフォルトコピー代入演算子
        Euler3D& operator=(const Euler3D&) = default;

        /// @brief 算術演算子
        /// @param rhs 被演算子
        /// @return
        constexpr Euler3D operator+(const Euler3D& rhs) const noexcept { return { roll + rhs.roll, pitch + rhs.pitch, yaw + rhs.yaw }; }
        constexpr Euler3D operator-(const Euler3D& rhs) const noexcept { return { roll - rhs.roll, pitch - rhs.pitch, yaw - rhs.yaw }; }
        constexpr Euler3D operator*(const Euler3D& rhs) const noexcept { return { roll * rhs.roll, pitch * rhs.pitch, yaw * rhs.yaw }; }
        constexpr Euler3D operator/(const Euler3D& rhs) const noexcept { return { roll / rhs.roll, pitch / rhs.pitch, yaw / rhs.yaw }; }
        constexpr Euler3D operator+(value_type rhs) const noexcept { return { roll + rhs, pitch + rhs, yaw + rhs }; }
        constexpr Euler3D operator-(value_type rhs) const noexcept { return { roll - rhs, pitch - rhs, yaw - rhs }; }
        constexpr Euler3D operator*(value_type rhs) const noexcept { return { roll * rhs, pitch * rhs, yaw * rhs }; }
        constexpr Euler3D operator/(value_type rhs) const noexcept { return { roll / rhs, pitch / rhs, yaw / rhs }; }

        /// @brief 複合代入演算子
        /// @param rhs 被演算子
        /// @return
        constexpr Euler3D& operator+=(const Euler3D& rhs) noexcept { return *this = *this + rhs; };
        constexpr Euler3D& operator-=(const Euler3D& rhs) noexcept { return *this = *this - rhs; };
        constexpr Euler3D& operator*=(const Euler3D& rhs) noexcept { return *this = *this * rhs; };
        constexpr Euler3D& operator/=(const Euler3D& rhs) noexcept { return *this = *this / rhs; };
        constexpr Euler3D& operator+=(value_type rhs) noexcept { return *this = *this + rhs; };
        constexpr Euler3D& operator-=(value_type rhs) noexcept { return *this = *this - rhs; };
        constexpr Euler3D& operator*=(value_type rhs) noexcept { return *this = *this * rhs; };
        constexpr Euler3D& operator/=(value_type rhs) noexcept { return *this = *this / rhs; };

        /// @brief 比較演算子
        /// @param rhs 被演算子
        /// @return
        constexpr bool operator==(const Euler3D& rhs) const noexcept
        {
            return roll  == rhs.roll  &&
                   pitch == rhs.pitch &&
                   yaw   == rhs.yaw  ;
        };
        constexpr bool operator!=(const Euler3D& rhs) const noexcept
        {
            return !(*this == rhs);
        };

        /// @brief 要素のいずれかに0以外の値があるかどうかを返す
        /// @return
        explicit constexpr operator bool() const noexcept
        {
            return roll || pitch || yaw;
        }

        /// @brief 要素がゼロであるか返す
        constexpr bool isZero() const noexcept
        {
            return !operator bool();
        }

        /// @brief 正規化されたオイラー角を取得する
        /// @param min
        /// @param max
        /// @return
        Euler3D normalized(value_type min, value_type max) const
        {
            return {
                udon::NormalizedAngle(roll , min, max),
                udon::NormalizedAngle(pitch, min, max),
                udon::NormalizedAngle(yaw  , min, max),
            };
        }

        /// @brief 回転方向を修正したオイラー角を取得する
        /// @param direction 回転方向
        /// @return 修正後のオイラー角
        Euler3D directionRevision(const Euler3D<bool>& direction)
        {
			return
			{
				roll  * (direction.roll  ? 1 : -1),
				pitch * (direction.pitch ? 1 : -1),
				yaw   * (direction.yaw   ? 1 : -1),
			};
        }

        /// @brief 値クリア
        constexpr void clear() noexcept
        {
            *this = {};
        }

#ifdef ARDUINO
        /// @brief デバッグ出力
        void show() const
        {
            Serial.print(F("r: ")), Serial.print(roll ), Serial.print('\t');
            Serial.print(F("p: ")), Serial.print(pitch), Serial.print('\t');
            Serial.print(F("y: ")), Serial.print(yaw  ), Serial.print('\t');
        }
#endif

        /// @brief シリアライズ後のバイト数を求める
        /// @return
        constexpr size_t capacity() const
        {
            return udon::Capacity(roll, pitch, yaw);
        }

        /// @brief
        /// @tparam T
        /// @param acc
        template <typename Acc>
        void accessor(Acc& acc)
        {
            acc(roll, pitch, yaw);
        }

	};

    using Euler = Euler3D<double>;

} // namespace udon
