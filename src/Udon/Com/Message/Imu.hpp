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
        /// @brief IMUの姿勢角(yaw角)
        /// @note 3軸の姿勢角を格納する場合はUdon::Eulerを使用してください。
        struct Yaw
        {

            /// @brief 旋回角
            double yaw;

#ifdef ARDUINO
            /// @brief デバッグ出力
            void show() const
            {
                Serial.print(F("yaw: ")), Serial.print(yaw), Serial.print('\t');
            }
#endif

            UDON_ENUMERABLE(yaw);
        };

    }    // namespace Message

}    // namespace Udon
