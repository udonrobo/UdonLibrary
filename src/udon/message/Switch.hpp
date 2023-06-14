#pragma once

#include <udon/com/serialization/Serializer.hpp>

namespace udon
{

	namespace message
	{

		struct Switch
		{

			/// @brief エンコーダーのカウント値
			bool isPress;

#ifdef ARDUINO
			/// @brief デバッグ出力
			void show() const
			{
				Serial.print(F("switch: ")), Serial.print(isPress), Serial.print('\t');
			}
#endif

			/// @brief シリアライズ後のバイト数を求める
			/// @return
			constexpr size_t capacity() const
			{
				return udon::Capacity(isPress);
			}

			/// @brief
			/// @tparam T
			/// @param acc
			template <typename Acc>
			void accessor(Acc& acc)
			{
				acc(isPress);
			}
		};

    	using EmergencySwitch = Switch;

	}

} // namespace udon
