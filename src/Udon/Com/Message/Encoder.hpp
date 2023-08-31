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
#include <Udon/Traits/ParsableMacro.hpp>

namespace Udon
{

    namespace Message
    {

        struct Encoder
        {

            /// @brief エンコーダーのカウント値
            int32_t count;

#ifdef ARDUINO
            /// @brief デバッグ出力
            void show() const
            {
                Serial.print(F("count: ")), Serial.print(count), Serial.print('\t');
            }
#endif

            UDON_PARSABLE(count);
        };

    }    // namespace Message

}    // namespace Udon
