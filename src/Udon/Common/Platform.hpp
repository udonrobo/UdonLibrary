#pragma once

#define UDON_LITTLE_ENDIAN 0
#define UDON_BIG_ENDIAN 1

#if defined(ARDUINO_ARCH_RP2040)

#    define UDON_PLATFORM_HAS_SERIAL_PRINTF 1
#    define UDON_PLATFORM_ENDIANNESS UDON_LITTLE_ENDIAN

#endif

#ifndef UDON_PLATFORM_HAS_SERIAL_PRINTF
#    error "This platform is not supported"
#endif

#ifndef UDON_PLATFORM_ENDIANNESS
#    error "This platform is not supported"
#endif