#pragma once

#include <udon/utility/Parsable.hpp>

namespace udon
{

    namespace message
    {

        struct Switch
        {

            /// @brief エンコーダーのカウント値
            bool isPress;

#ifdef ARDUINO
            /// @brief デバッグ出力
            void show() const
            {
                Serial.print(F("switch: ")), Serial.print(isPress), Serial.print('\t');
            }
#endif

            UDON_PARSABLE(isPress);
        };

        using EmergencySwitch = Switch;

    }    // namespace message

}    // namespace udon
