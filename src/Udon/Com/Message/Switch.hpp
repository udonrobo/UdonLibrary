//
//    通信用メッセージ
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include <Udon/Traits/ParsableMacro.hpp>

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
