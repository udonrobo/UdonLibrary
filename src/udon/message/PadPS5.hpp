#pragma once

#include <udon/com/serialization/Serializer.hpp>

#include <stdint.h>

namespace udon
{
    namespace message
    {
        struct PadPS5
        {
			bool isConnected;

			bool triangle;
			bool circle;
			bool cross;
			bool square;

			bool up;
			bool right;
			bool down;
			bool left;

			bool l1;
			bool r1;
			bool l2;
			bool r2;
			bool l3;
			bool r3;

			int8_t analogRightX;
			int8_t analogRightY;
			int8_t analogLeftX;
			int8_t analogLeftY;
			int8_t analogR2;
			int8_t analogL2;

			bool create;
			bool option;
			bool touch;

			bool mic;

#ifdef ARDUINO
			/// @brief デバッグ出力
			void show() const
			{
                // todo
                // Serial.print(isConnected);
                // Serial.print(triangle);
                // Serial.print(circle);
                // Serial.print(cross);
                // Serial.print(square);
                // Serial.print(l1);
                // Serial.print(r1);
                // Serial.print(l2);
                // Serial.print(r2);
                // Serial.print(l3);
                // Serial.print(r3);
                // Serial.print(up);
                // Serial.print(right);
                // Serial.print(down);
                // Serial.print(left);
                // Serial.print(analogRightX);
                // Serial.print(analogRightY);
                // Serial.print(analogLeftX);
                // Serial.print(analogLeftY);
                // Serial.print(analogL2);
                // Serial.print(analogR2);
                // Serial.print(create);
                // Serial.print(option);
                // Serial.print(touch);
                // Serial.print(mic);
			}
#endif

			/// @brief シリアライズ後のバイト数を求める
			/// @return
			constexpr size_t capacity() const
			{
				return udon::Capacity(isConnected
                                    , triangle
                                    , circle
                                    , cross
                                    , square
                                    , l1
                                    , r1
                                    , l2
                                    , r2
                                    , l3
                                    , r3
                                    , up
                                    , right
                                    , down
                                    , left
                                    , analogRightX
                                    , analogRightY
                                    , analogLeftX
                                    , analogLeftY
                                    , analogL2
                                    , analogR2
                                    , create
                                    , option
                                    , touch
                                    , mic);
			}

			/// @brief
			/// @tparam T
			/// @param acc
			template <typename Acc>
			void accessor(Acc& acc)
			{
				acc(isConnected
                    , triangle
                    , circle
                    , cross
                    , square
                    , l1
                    , r1
                    , l2
                    , r2
                    , l3
                    , r3
                    , up
                    , right
                    , down
                    , left
                    , analogRightX
                    , analogRightY
                    , analogLeftX
                    , analogLeftY
                    , analogL2
                    , analogR2
                    , create
                    , option
                    , touch
                    , mic);
			}

        };
    }    // namespace message
}    // namespace udon
