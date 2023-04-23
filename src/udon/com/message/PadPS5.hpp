#pragma once

#include <udon/com/serializer/Serializer.hpp>

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

            bool l1;
            bool r1;
            bool l2;
            bool r2;
            bool l3;
            bool r3;

            bool up;
            bool right;
            bool down;
            bool left;

            uint8_t analogRightX;
            uint8_t analogRightY;
            uint8_t analogLeftX;
            uint8_t analogLeftY;
            uint8_t analogL2;
            uint8_t analogR2;

            bool create;
            bool option;
            bool touch;

            bool mic;

            /// @brief メンバイテレーション演算子
            /// @tparam MIterator
            /// @param mit
            /// @param rhs
            /// @return
	        template<class MIterator>
            friend MIterator& operator|(MIterator& mit, udon::message::PadPS5& rhs) noexcept
            {
                return mit
                    | rhs.isConnected
                    | rhs.isConnected
                    | rhs.triangle
                    | rhs.circle
                    | rhs.cross
                    | rhs.square
                    | rhs.l1
                    | rhs.r1
                    | rhs.l2
                    | rhs.r2
                    | rhs.l3
                    | rhs.r3
                    | rhs.up
                    | rhs.right
                    | rhs.down
                    | rhs.left
                    | rhs.analogRightX
                    | rhs.analogRightY
                    | rhs.analogLeftX
                    | rhs.analogLeftY
                    | rhs.analogL2
                    | rhs.analogR2
                    | rhs.create
                    | rhs.option
                    | rhs.touch
                    | rhs.mic;
            }

        };
    }    // namespace message
}    // namespace udon
