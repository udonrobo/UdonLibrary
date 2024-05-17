//
//    通信用メッセージ
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#include <stdint.h>
#include <Udon/Traits/EnumerableMacro.hpp>

namespace Udon
{

    namespace Message
    {

        struct Servo
        {

            /// @brief 角度
            uint16_t angle;

#ifdef ARDUINO
            /// @brief デバッグ出力
            void show() const
            {
                Serial.print(F("angle: ")), Serial.print(angle), Serial.print('\t');
            }
#endif

            UDON_ENUMERABLE(angle);
        };

    }    // namespace Message

}    // namespace Udon
