#pragma once

#include <stdarg.h>
#include <stdint.h>
#include <udon/com/serialization/Serializer.hpp>
#include <udon/utility/Parsable.hpp>

namespace udon
{

    namespace message
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

        using Lcd1602 = Lcd<2, 16>;
        using Lcd2004 = Lcd<4, 20>;

    }    // namespace message

}    // namespace udon
