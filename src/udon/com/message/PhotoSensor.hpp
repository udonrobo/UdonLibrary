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

#include <udon/utility/Parsable.hpp>

namespace udon
{

    namespace message
    {

        struct PhotoSensor
        {

            /// @brief 遮光しているかどうか
            bool shade;

#ifdef ARDUINO
            /// @brief デバッグ出力
            void show() const
            {
                Serial.print(shade ? F("shade") : F("ray"));
                Serial.print('\t');
            }
#endif

            UDON_PARSABLE(shade);
        };

    }    // namespace message

}    // namespace udon
