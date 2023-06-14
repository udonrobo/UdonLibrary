#pragma once

#include <stdint.h>
#include <udon/utility/Parsable.hpp>

namespace udon
{

    namespace message
    {

        struct Motor
        {

            /// @brief モーターの出力値
            int16_t power;

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
