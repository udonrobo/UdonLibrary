#pragma once

#include <stdint.h>
#include <udon/com/serializer/Serializer.hpp>

namespace udon
{

	namespace message
	{

		struct Servo
		{

			/// @brief モーターの出力値
			uint16_t angle;

#ifdef ARDUINO
			/// @brief デバッグ出力
			void show() const
			{
				Serial.print(F("angle: ")), Serial.print(angle), Serial.print('\t');
			}
#endif

			/// @brief シリアライズ後のバイト数を求める
			/// @return
			constexpr size_t capacity() const
			{
				return udon::Capacity(angle);
			}

			/// @brief
			/// @tparam T
			/// @param acc
			template <typename Acc>
			void accessor(Acc& acc)
			{
				acc(angle);
			}
		};

	}

} // namespace udon
