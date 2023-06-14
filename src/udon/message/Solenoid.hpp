#pragma once

#include <stdint.h>
#include <udon/com/serialization/Serializer.hpp>

namespace udon
{

	namespace message
	{

		struct Solenoid
		{

			/// @brief ソレノイドの位置
			bool release;

#ifdef ARDUINO
			/// @brief デバッグ出力
			void show() const
			{
				Serial.print(F("Sole: ")), Serial.print(release), Serial.print('\t');
			}
#endif

			/// @brief シリアライズ後のバイト数を求める
			/// @return
			constexpr size_t capacity() const
			{
				return udon::Capacity(release);
			}

			/// @brief
			/// @tparam T
			/// @param acc
			template <typename Acc>
			void accessor(Acc& acc)
			{
				acc(release);
			}
		};

	}

} // namespace udon
