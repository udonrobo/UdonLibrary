#pragma once

#include <stdint.h>
#include <udon/com/serializer/Serializer.hpp>

namespace udon
{

	namespace message
	{

		struct Yaw
		{

			/// @brief 旋回角
			double yaw;

#ifdef ARDUINO
			/// @brief デバッグ出力
			void show() const
			{
				Serial.print(F("yaw: ")), Serial.print(yaw), Serial.print('\t');
			}
#endif

			/// @brief シリアライズ後のバイト数を求める
			/// @return
			constexpr size_t capacity() const
			{
				return udon::Capacity(yaw);
			}

			/// @brief
			/// @tparam T
			/// @param acc
			template <typename Acc>
			void accessor(Acc& acc)
			{
				acc(yaw);
			}
		};

	}

} // namespace udon
