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

        /// @brief 距離センサー
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

            UDON_ENUMERABLE(distance);
        };

    }    // namespace Message

}    // namespace Udon
