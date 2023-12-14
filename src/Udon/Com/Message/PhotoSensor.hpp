//
//    通信用メッセージ
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include <Udon/Traits/EnumerableMacro.hpp>

namespace Udon
{

    namespace Message
    {

        struct PhotoSensor
        {

            /// @brief 入光中であるかどうか
            bool light;

#ifdef ARDUINO
            /// @brief デバッグ出力
            void show() const
            {
                Serial.print(light ? "Light\t" : "Dark\t");
            }
#endif

            UDON_ENUMERABLE(light);
        };

    }    // namespace Message

}    // namespace Udon
