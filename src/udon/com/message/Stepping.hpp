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

        struct Stepping
        {

            /// @brief ステッピングモーターの出力値
            int32_t position;

#ifdef ARDUINO
            /// @brief デバッグ出力
            void show() const
            {
                Serial.print(F("pos: ")), Serial.print(position), Serial.print('\t');
            }
#endif

            UDON_PARSABLE(position);
        };

    }    // namespace message

}    // namespace udon
