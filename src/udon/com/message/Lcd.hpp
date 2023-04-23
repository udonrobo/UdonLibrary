#pragma once

#include <stdint.h>
#include <udon/com/serializer/Serializer.hpp>

namespace udon
{

	namespace message
	{

		struct Lcd1602
		{

			/// @brief LCDのテキスト
			char top[16];
			char bottom[16];

#ifdef ARDUINO
			/// @brief デバッグ出力
			void show() const
			{
				Serial.print(F("lcd: "));
				Serial.print(top)   , Serial.print(' ');
				Serial.print(bottom), Serial.print('\t');
			}
#endif

			/// @brief シリアライズ後のバイト数を求める
			/// @return
			constexpr size_t capacity() const
			{
				return udon::Capacity(top, bottom);
			}

			/// @brief
			/// @tparam T
			/// @param acc
			template <typename Acc>
			void accessor(Acc& acc)
			{
				acc(top, bottom);
			}
		};

	}

} // namespace udon
