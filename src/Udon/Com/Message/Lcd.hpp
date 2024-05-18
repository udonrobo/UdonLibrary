//
//    通信用メッセージ
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#include <stdarg.h>
#include <stdint.h>
#include <Udon/Traits/EnumerableMacro.hpp>

namespace Udon
{

    namespace Message
    {

        /// @brief LCD
        /// @tparam Column 列数
        /// @tparam Row 行数
        template <size_t Column, size_t Row>
        struct Lcd
        {
            /// @brief LCDのテキスト
            char text[Row][Column];

            /// @brief LCDに文字列を設定
            /// @param index 行
            /// @param format 書式
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

            UDON_ENUMERABLE(text);
        };

        /// @brief LCD 16x2
        using Lcd1602 = Lcd<16, 2>;

        /// @brief LCD 20x4
        using Lcd2004 = Lcd<20, 4>;

    }    // namespace Message

}    // namespace Udon
