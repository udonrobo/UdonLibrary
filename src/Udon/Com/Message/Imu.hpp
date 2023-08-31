//-------------------------------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    通信用メッセージ
//
//-------------------------------------------------------------------


#pragma once

#include <stdint.h>
#include <Udon/Traits/Parsable.hpp>

namespace Udon
{

    namespace Message
    {
        /// @brief IMUの姿勢角(yaw角)
        /// @details 3軸の姿勢角を格納する場合はUdon::Eulerを使用してください。
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

            UDON_PARSABLE(yaw);
        };

    }    // namespace Message

}    // namespace Udon
