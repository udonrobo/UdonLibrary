#pragma once

#include <stdint.h>
#include <udon/com/serialization/Serializer.hpp>

namespace udon
{

    namespace message
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

    }    // namespace message

}    // namespace udon
