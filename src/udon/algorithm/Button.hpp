#pragma once

#include <udon/algorithm/Input.hpp>

namespace udon
{
	struct Button
	{

		/// @brief 押されているか
		bool press;

		/// @brief 押された瞬間か
		bool click;

		/// @brief 離された瞬間か
		bool release;

		/// @brief デフォルトコンストラクタ
		Button() = default;

		/// @brief デフォルトコピーコンストラクタ
		/// @param
		Button(const Button&) = default;

		/// @brief Inputクラスからの変換コンストラクタ
		/// @param input
		Button(const Input& input)
			: press(input.press())
			, click(input.clicked())
			, release(input.released())
		{
		}

	};
} // namespace udon
