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

        /// @brief エンコーダー
        struct Encoder
        {

            /// @brief エンコーダーのカウント値
            int32_t count;

#ifdef ARDUINO
            /// @brief デバッグ出力
            void show() const
            {
                Serial.print(F("count: ")), Serial.print(count), Serial.print('\t');
            }
#endif

            UDON_ENUMERABLE(count);
        };

    }    // namespace Message

}    // namespace Udon
