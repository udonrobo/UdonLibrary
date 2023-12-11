//
//    通信用メッセージ
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include <stdint.h>
#include <Udon/Traits/ParsableMacro.hpp>

namespace Udon
{

    namespace Message
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

    }    // namespace Message

}    // namespace Udon
