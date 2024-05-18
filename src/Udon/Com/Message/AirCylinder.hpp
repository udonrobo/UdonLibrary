//
//    通信用メッセージ
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#include <stdint.h>
#include <Udon/Traits/EnumerableMacro.hpp>

namespace Udon
{

    namespace Message
    {

        /// @brief エアシリンダー
        struct AirCylinder
        {

            /// @brief 伸びているか
            bool push;

#ifdef ARDUINO
            /// @brief デバッグ出力
            void show() const
            {
                Serial.print(F("Air: ")), Serial.print(push), Serial.print('\t');
            }
#endif

            UDON_ENUMERABLE(push);
        };

    }    // namespace Message

}    // namespace Udon
