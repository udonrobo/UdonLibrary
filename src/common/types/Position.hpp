#pragma once

#include <common\types\Vector2D.hpp>

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

	};

	using Pos = Position<double>;

}