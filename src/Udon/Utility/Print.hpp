#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <algorithm>
#include <stdarg.h>
#include <stdio.h>
#include <Arduino.h>

namespace Udon
{

    struct Printer
    {

        Printer() = default;

        /// @brief 文字を出力します
        Printer& operator<<(const char c)
        {
            Serial.print(c);
            return *this;
        }

        /// @brief 文字列を出力します
        Printer& operator<<(const char* str)
        {
            Serial.print(str);
            return *this;
        }

        /// @brief 文字列を出力します
        Printer& operator<<(const String& value)
        {
            Serial.print(value);
            return *this;
        }

        /// @brief 整数を出力します
        Printer& operator<<(const int value)
        {
            Serial.print(value);
            return *this;
        }

        /// @brief 浮動小数を出力します 
        Printer& operator<<(const float value)
        {
            Serial.print(value);
            return *this;
        }

        /// @brief 浮動小数を出力します 
        Printer& operator<<(const double value)
        {
            Serial.print(value);
            return *this;
        }
    };

    inline String Format(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        char buf[256];
        vsnprintf(buf, sizeof buf, format, args);
        va_end(args);
        return buf;
    }

    extern Printer Print;

}    // namespace Udon