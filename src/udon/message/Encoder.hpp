#pragma once

#include <stdint.h>
#include <udon/com/serialization/Serializer.hpp>

namespace udon
{

    namespace message
    {

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

            UDON_PARSABLE(count);
        };

    }    // namespace message

}    // namespace udon
