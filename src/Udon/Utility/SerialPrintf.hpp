#pragma once

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

namespace Udon
{

#if UDON_PLATFORM_HAS_SERIAL_PRINTF

    template <typename... Args>
    inline void SerialPrintf(Stream& stream, const char* format, Args... args)
    {
        stream.printf(format, args...);
    }

#else

    template <typename... Args>
    inline void SerialPrintf(Stream& stream, const char* format, Args... args)
    {
        char buf[256];
        snprintf(buf, sizeof buf, format, args...);
        stream.print(buf);
    }

#endif

    template <typename... Args>
    inline void SerialPrintf(const char* format, Args... args)
    {
        SerialPrintf(Serial, format, args...);
    }

}    // namespace Udon
