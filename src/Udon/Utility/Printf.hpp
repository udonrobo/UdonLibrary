//
//    全プラットフォームで使用可能なPrintf
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include <Udon/Utility/Platform.hpp>

namespace Udon
{
#if UDON_PLATFORM_OUTPUT_STREAM == UDON_PLATFORM_OUTPUT_SERIAL

#    if UDON_PLATFORM_HAS_SERIAL_PRINTF

    template <typename... Args>
    inline void Printf(Stream& stream, const char* format, Args&&... args)
    {
        stream.printf(format, args...);
    }

#    elif defined(ARDUINO)

    template <typename... Args>
    inline void Printf(Stream& stream, const char* format, Args&&... args)
    {
        char buf[256];
        snprintf(buf, sizeof buf, format, args...);
        stream.print(buf);
    }

#    else

    

#    endif

    template <typename... Args>
    inline void Printf(const char* format, Args... args)
    {
        Printf(Serial, format, args...);
    }

#elif UDON_PLATFORM_OUTPUT_STREAM == UDON_PLATFORM_OUTPUT_CONSOLE

    template <typename... Args>
    inline void Printf(const char* format, Args... args)
    {
        printf(format, args...);
    }

#endif

}    // namespace Udon
