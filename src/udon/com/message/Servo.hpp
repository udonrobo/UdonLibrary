//-----------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-----------------------------------------------
//
//    通信用メッセージ
//
//-----------------------------------------------


#pragma once

#include <stdint.h>
#include <udon/utility/Parsable.hpp>

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

            UDON_PARSABLE(angle);
        };

    }    // namespace message

}    // namespace udon
