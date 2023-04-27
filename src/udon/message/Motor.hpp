#pragma once

#include <stdint.h>
#include <udon/com/serializer/Serializer.hpp>

namespace udon
{

	namespace message
	{

		struct Motor
		{

			/// @brief モーターの出力値
			int16_t power;

#ifdef ARDUINO
			/// @brief デバッグ出力
			void show() const
			{
				Serial.print(F("power: ")), Serial.print(power), Serial.print('\t');
			}
#endif

			/// @brief シリアライズ後のバイト数を求める
			/// @return
			constexpr size_t capacity() const
			{
				return udon::Capacity(power);
			}

			/// @brief
			/// @tparam T
			/// @param acc
			template <typename Acc>
			void accessor(Acc& acc)
			{
				acc(power);
			}
		};

	}

} // namespace udon
