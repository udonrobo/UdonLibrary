#pragma once

#include <stdint.h>
#include <udon/com/serialization/Serializer.hpp>

namespace udon
{

	namespace message
	{

		struct Encoder
		{

			/// @brief エンコーダーのカウント値
			int32_t count;

#ifdef ARDUINO
			/// @brief デバッグ出力
			void show() const
			{
				Serial.print(F("count: ")), Serial.print(count), Serial.print('\t');
			}
#endif

			/// @brief シリアライズ後のバイト数を求める
			/// @return
			constexpr size_t capacity() const
			{
				return udon::Capacity(count);
			}

			/// @brief
			/// @tparam T
			/// @param acc
			template <typename Acc>
			void accessor(Acc& acc)
			{
				acc(count);
			}
		};

	}

} // namespace udon
