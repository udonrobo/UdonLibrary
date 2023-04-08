#pragma once

namespace udon
{

	class Euler
	{

    public:

        /// @brief 要素の型
		using value_type = double;

        /// @brief ロール角
        value_type roll;

        /// @brief ピッチ角
        value_type pitch;

        /// @brief ヨー角
        value_type yaw;

		/// @brief デフォルトコンストラクタ
        constexpr Euler() noexcept
            : roll ()
            , pitch()
            , yaw  ()
        {}

        /// @brief デフォルトコピーコンストラクタ
        constexpr Euler(const Euler& rhs) noexcept
            : roll (rhs.roll )
            , pitch(rhs.pitch)
			, yaw  (rhs.yaw  )
        {}

        /// @brief コンストラクタ
        /// @param roll ロール角
        /// @param pitch ピッチ角
        /// @param yaw ヨー角
        constexpr Euler(value_type roll, value_type pitch, value_type yaw) noexcept
            : roll (roll )
            , pitch(pitch)
			, yaw  (yaw  )
        {
        }

        /// @brief デフォルトコピー代入演算子
        Euler& operator=(const Euler&) = default;

        /// @brief 算術演算子
        /// @param rhs 被演算子
        /// @return
        constexpr Euler operator+(const Euler& rhs) const noexcept { return { roll + rhs.roll, pitch + rhs.pitch, yaw + rhs.yaw }; }
        constexpr Euler operator-(const Euler& rhs) const noexcept { return { roll - rhs.roll, pitch - rhs.pitch, yaw - rhs.yaw }; }
        constexpr Euler operator*(const Euler& rhs) const noexcept { return { roll * rhs.roll, pitch * rhs.pitch, yaw * rhs.yaw }; }
        constexpr Euler operator/(const Euler& rhs) const noexcept { return { roll / rhs.roll, pitch / rhs.pitch, yaw / rhs.yaw }; }
        constexpr Euler operator+(value_type rhs) const noexcept { return { roll + rhs, pitch + rhs, yaw + rhs }; }
        constexpr Euler operator-(value_type rhs) const noexcept { return { roll - rhs, pitch - rhs, yaw - rhs }; }
        constexpr Euler operator*(value_type rhs) const noexcept { return { roll * rhs, pitch * rhs, yaw * rhs }; }
        constexpr Euler operator/(value_type rhs) const noexcept { return { roll / rhs, pitch / rhs, yaw / rhs }; }

        /// @brief 複合代入演算子
        /// @param rhs 被演算子
        /// @return
        constexpr Euler& operator+=(const Euler& rhs) noexcept { return *this = *this + rhs; };
        constexpr Euler& operator-=(const Euler& rhs) noexcept { return *this = *this - rhs; };
        constexpr Euler& operator*=(const Euler& rhs) noexcept { return *this = *this * rhs; };
        constexpr Euler& operator/=(const Euler& rhs) noexcept { return *this = *this / rhs; };
        constexpr Euler& operator+=(value_type rhs) noexcept { return *this = *this + rhs; };
        constexpr Euler& operator-=(value_type rhs) noexcept { return *this = *this - rhs; };
        constexpr Euler& operator*=(value_type rhs) noexcept { return *this = *this * rhs; };
        constexpr Euler& operator/=(value_type rhs) noexcept { return *this = *this / rhs; };

        /// @brief 比較演算子
        /// @param rhs 被演算子
        /// @return
        constexpr bool operator==(const Euler& rhs) const noexcept
        {
            return roll  == rhs.roll  &&
                   pitch == rhs.pitch &&
                   yaw   == rhs.yaw  ;
        };
        constexpr bool operator!=(const Euler& rhs) const noexcept
        {
            return !(*this == rhs);
        };

        /// @brief 要素のいずれかに0以外の値があるかどうかを返す
        /// @return
        explicit constexpr operator bool() const noexcept
        {
            return roll || pitch || yaw;
        }

        /// @brief 正規化されたオイラー角を取得する
        /// @param min
        /// @param max
        /// @return
        Euler normalized(double min, double max) const
        {
            return {
                udon::NormalizedAngle(roll , min, max),
                udon::NormalizedAngle(pitch, min, max),
                udon::NormalizedAngle(yaw  , min, max),
            };
        }

        /// @brief 値クリア
        constexpr void clear() noexcept
        {
            *this = {};
        }

        void show() const
        {
            Serial.print(F("roll: " )), Serial.print(roll ), Serial.print('\t');
            Serial.print(F("pitch: ")), Serial.print(pitch), Serial.print('\t');
            Serial.print(F("yaw: "  )), Serial.print(yaw  ), Serial.print('\t');
        }

	};
} // namespace udon
