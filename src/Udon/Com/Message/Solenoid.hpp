//
//    通信用メッセージ
//
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include <stdint.h>
#include <Udon/Traits/EnumerableMacro.hpp>

namespace Udon
{

    namespace Message
    {

        struct Solenoid
        {

            /// @brief ソレノイドの位置
            bool push;

#ifdef ARDUINO
            /// @brief デバッグ出力
            void show() const
            {
                Serial.print(F("Sole: ")), Serial.print(push), Serial.print('\t');
            }
#endif

            UDON_ENUMERABLE(push);
        };



    }    // namespace Message

}    // namespace Udon
