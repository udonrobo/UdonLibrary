//
//    通信用メッセージ
//
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include <stdint.h>
#include <Udon/Traits/EnumerableMacro.hpp>

namespace Udon
{

    namespace Message
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

            UDON_ENUMERABLE(power);
        };

    }    // namespace Message

}    // namespace Udon
