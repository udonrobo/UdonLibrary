//
//    通信用メッセージ
//
//    Copyright (c) 2022 udonrobo
//

#pragma once

#include <Udon/Traits/EnumerableMacro.hpp>

namespace Udon
{

    namespace Message
    {

        /// @brief フォトセンサー
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
