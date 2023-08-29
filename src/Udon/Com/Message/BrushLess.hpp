//-------------------------------------------------------------------
//
//    UdonLibrary
// 
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    通信用メッセージ
//
//-------------------------------------------------------------------


#pragma once

#include <stdint.h>
#include <Udon/Traits/Parsable.hpp>

namespace Udon
{

    namespace Message
    {

        struct BrushLess
        {

            /// @brief モーターの出力値
            int32_t power;

#ifdef ARDUINO
            /// @brief デバッグ出力
            void show() const
            {
                Serial.print(F("power: ")), Serial.print(power), Serial.print('\t');
            }
#endif

            UDON_PARSABLE(power);
        };

    }    // namespace Message

}    // namespace Udon
