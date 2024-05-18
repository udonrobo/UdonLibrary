//
//    通信用メッセージ
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#include <Udon/Traits/EnumerableMacro.hpp>

namespace Udon
{

    namespace Message
    {

        /// @brief スイッチ
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

            UDON_ENUMERABLE(press);
        };

    }    // namespace Message

}    // namespace Udon
