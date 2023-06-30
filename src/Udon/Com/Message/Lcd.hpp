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

#include <stdarg.h>
#include <stdint.h>
#include <Udon/Utility/Parsable.hpp>

namespace Udon
{

    namespace Message
    {

        template <size_t Column, size_t Row>
        struct Lcd
        {
            /// @brief LCDのテキスト
            char text[Row][Column];

            void printf(size_t index, const char* format, ...)
            {
                va_list args;
                va_start(args, format);
                vsnprintf(text[index], Column, format, args);
                va_end(args);
            }

#ifdef ARDUINO
            /// @brief デバッグ出力
            void show() const
            {
                Serial.print(F("lcd: "));
                for (auto&& row : text)
                {
                    Serial.println(row);
                }
            }
#endif

            UDON_PARSABLE(text);
        };

        using Lcd1602 = Lcd<16, 2>;
        using Lcd2004 = Lcd<20, 4>;

    }    // namespace Message

}    // namespace Udon
