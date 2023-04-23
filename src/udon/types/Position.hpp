#pragma once

#include <udon/types/Vector2D.hpp>

namespace udon
{

	/// @brief ロボットの位置
	/// @tparam Ty 要素の型
	template<class Ty>
	struct Position
	{

		/// @brief 要素の型
		using value_type = Ty;

		/// @brief 座標
		udon::Vector2D<value_type> vector;

		/// @brief 旋回角 [rad]
		value_type turn;

		/// @brief デフォルトコンストラクタ
		constexpr Position() noexcept
			: vector()
			, turn  ()
		{}

		/// @brief デフォルトコピーコンストラクタ
		constexpr Position(const Position& rhs) noexcept
			: vector(rhs.vector)
			, turn  (rhs.turn  )
		{}

		/// @brief コンストラクタ
		constexpr Position(const udon::Vector2D<value_type>& vector, value_type turn) noexcept
			: vector(vector)
			, turn  (turn  )
		{}

		/// @brief デフォルトコピー代入演算子
		Position& operator=(const Position&) = default;

        /// @brief 算術演算子
        /// @param rhs 被演算子
        /// @return
		constexpr Position operator+(const Position& rhs) const noexcept { return { vector + rhs.vector, turn + rhs.turn }; }
		constexpr Position operator-(const Position& rhs) const noexcept { return { vector - rhs.vector, turn - rhs.turn }; }
		constexpr Position operator*(const Position& rhs) const noexcept { return { vector * rhs.vector, turn * rhs.turn }; }
		constexpr Position operator/(const Position& rhs) const noexcept { return { vector / rhs.vector, turn / rhs.turn }; }
		constexpr Position operator+(value_type rhs) const noexcept { return { vector + rhs, turn + rhs }; }
		constexpr Position operator-(value_type rhs) const noexcept { return { vector - rhs, turn - rhs }; }
		constexpr Position operator*(value_type rhs) const noexcept { return { vector * rhs, turn * rhs }; }
		constexpr Position operator/(value_type rhs) const noexcept { return { vector / rhs, turn / rhs }; }

        /// @brief 複合代入演算子
        /// @param rhs 被演算子
        /// @return
		constexpr Position operator+=(const Position& rhs) noexcept { return *this = *this + rhs; }
		constexpr Position operator-=(const Position& rhs) noexcept { return *this = *this - rhs; }
		constexpr Position operator*=(const Position& rhs) noexcept { return *this = *this * rhs; }
		constexpr Position operator/=(const Position& rhs) noexcept { return *this = *this / rhs; }
		constexpr Position operator+=(value_type rhs) noexcept { return *this = *this + rhs; }
		constexpr Position operator-=(value_type rhs) noexcept { return *this = *this - rhs; }
		constexpr Position operator*=(value_type rhs) noexcept { return *this = *this * rhs; }
		constexpr Position operator/=(value_type rhs) noexcept { return *this = *this / rhs; }

        /// @brief 要素のいずれかに0以外の値があるかどうかを返す
		constexpr operator bool() const noexcept
		{
			return vector || turn;
		}

        /// @brief 要素がすべて0であるかを変えす
        constexpr bool isZero() const noexcept
        {
            return !Position::operator bool();
        }

        /// @brief 値クリア
        constexpr void clear() noexcept
        {
            *this = {};
        }

        /// @brief メモリアラインを除去したサイズを取得する
        /// @return
        static constexpr size_t PackedSize() noexcept
        {
            return
                udon::Vector2D<value_type>::PackedSize() +
                sizeof(value_type);
        }

        /// @brief シリアライズ後のバイト数を求める
        /// @return
        constexpr size_t capacity() const
        {
            return udon::Capacity(vector, turn);
        }

        /// @brief
        /// @tparam T
        /// @param acc
        template <typename Acc>
        void accessor(Acc& acc)
        {
            acc(vector, turn);
        }

	};

	using Pos = Position<double>;

}