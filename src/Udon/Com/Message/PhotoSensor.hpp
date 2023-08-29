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

#include <Udon/Traits/Parsable.hpp>

namespace Udon
{

    namespace Message
    {

        struct PhotoSensor
        {

            /// @brief 入光中であるかどうか
            bool light;

#ifdef ARDUINO
            /// @brief デバッグ出力
            void show() const
            {
                Serial.print(light ? "Light\t" : "Dark\t");
            }
#endif

            UDON_PARSABLE(light);
        };

    }    // namespace Message

}    // namespace Udon
