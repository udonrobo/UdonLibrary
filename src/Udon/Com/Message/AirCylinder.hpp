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
#include <Udon/Utility/Parsable.hpp>

namespace Udon
{

    namespace Message
    {

        struct AirCylinder
        {

            /// @brief シリンダーの位置
            bool push;

#ifdef ARDUINO
            /// @brief デバッグ出力
            void show() const
            {
                Serial.print(F("Air: ")), Serial.print(push), Serial.print('\t');
            }
#endif

            UDON_PARSABLE(push);
        };

        

    }    // namespace Message

}    // namespace Udon