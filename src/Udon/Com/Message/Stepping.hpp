//
//    通信用メッセージ
//
//    Copyright (c) 2022 udonrobo
//

#pragma once

#include <stdint.h>
#include <Udon/Traits/EnumerableMacro.hpp>

namespace Udon
{

    namespace Message
    {

        /// @brief ステッピングモーター
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

            UDON_ENUMERABLE(position);
        };

    }    // namespace Message

}    // namespace Udon
