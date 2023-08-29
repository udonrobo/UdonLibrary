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

        struct Switch
        {

            /// @brief ボタンが押されているか
            bool press;

#ifdef ARDUINO
            /// @brief デバッグ出力
            void show() const
            {
                Serial.print(press ? F("press") : F("not press"));
                Serial.print('\t');
            }
#endif

            UDON_PARSABLE(press);
        };

    }    // namespace Message

}    // namespace Udon
