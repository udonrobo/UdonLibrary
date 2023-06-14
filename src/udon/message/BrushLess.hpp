#pragma once

#include <stdint.h>
#include <udon/com/serialization/Serializer.hpp>

namespace udon
{

    namespace message
    {

        struct BrushLess
        {

            /// @brief モーターの出力値
            int32_t power;

#ifdef ARDUINO
            /// @brief デバッグ出力
            void show() const
            {
                Serial.print(F("power: ")), Serial.print(power), Serial.print('\t');
            }
#endif

            UDON_PARSABLE(power);
        };

    }    // namespace message

}    // namespace udon
