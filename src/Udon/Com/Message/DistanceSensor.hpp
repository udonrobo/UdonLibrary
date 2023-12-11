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

        struct DistanceSensor
        {

            /// @brief 距離センサーの値 mm
            double distance;

#ifdef ARDUINO
            /// @brief デバッグ出力
            void show() const
            {
                Serial.print(F("distance: ")), Serial.print(distance), Serial.print('\t');
            }
#endif

            UDON_PARSABLE(distance);
        };

    }    // namespace Message

}    // namespace Udon
