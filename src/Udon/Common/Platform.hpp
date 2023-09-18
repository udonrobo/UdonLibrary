﻿//-------------------------------------------------------------------
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
//    プラットフォーム定義
//
//-------------------------------------------------------------------
//
//     次のマクロが定義されます。新たにプラットフォームを追加する場合は、これらのマクロを適切に定義してください。
//
//     UDON_PLATFORM_HAS_STL            (0 or 1)
//     UDON_PLATFORM_HAS_SERIAL_PRINTF  (0 or 1)
//     UDON_PLATFORM_OUTPUT_STREAM               (UDON_PLATFORM_OUTPUT_SERIAL or UDON_PLATFORM_OUTPUT_CONSOLE)
//     UDON_PLATFORM_ENDIANNESS         (UDON_PLATFORM_LITTLE_ENDIAN or UDON_PLATFORM_BIG_ENDIAN)
//
//-------------------------------------------------------------------

#pragma once

// UDON_PLATFORM_OUTPUT_STREAM
#define UDON_PLATFORM_OUTPUT_SERIAL 0
#define UDON_PLATFORM_OUTPUT_CONSOLE 1

// UDON_PLATFORM_ENDIANNESS
#define UDON_PLATFORM_LITTLE_ENDIAN 0
#define UDON_PLATFORM_BIG_ENDIAN 1

//-------------------------------------------------------------------
//
//    Arduino Nano / Uno / Mega
//
//-------------------------------------------------------------------

#if defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA) || defined(ARDUINO_AVR_MEGA2560)

#    define UDON_PLATFORM_HAS_STL 0
#    define UDON_PLATFORM_HAS_SERIAL_PRINTF 0
#    define UDON_PLATFORM_OUTPUT_STREAM UDON_PLATFORM_OUTPUT_SERIAL
#    define UDON_PLATFORM_ENDIANNESS UDON_PLATFORM_LITTLE_ENDIAN

//-------------------------------------------------------------------
//
//    Teensy
//
//-------------------------------------------------------------------

#elif defined(TEENSYDUINO)

#    define UDON_PLATFORM_HAS_STL 1
#    define UDON_PLATFORM_HAS_SERIAL_PRINTF 1
#    define UDON_PLATFORM_OUTPUT_STREAM UDON_PLATFORM_OUTPUT_SERIAL
#    define UDON_PLATFORM_ENDIANNESS UDON_PLATFORM_LITTLE_ENDIAN

//-------------------------------------------------------------------
//
//    Raspberry Pi Pico
//
//-------------------------------------------------------------------

#elif defined(ARDUINO_ARCH_RP2040)

#    define UDON_PLATFORM_HAS_STL 1
#    define UDON_PLATFORM_HAS_SERIAL_PRINTF 1
#    define UDON_PLATFORM_OUTPUT_STREAM UDON_PLATFORM_OUTPUT_SERIAL
#    define UDON_PLATFORM_ENDIANNESS UDON_PLATFORM_LITTLE_ENDIAN

//-------------------------------------------------------------------
//
//    msvc
//
//-------------------------------------------------------------------

#elif defined(_MSC_VER)

#    pragma warning(disable : 4819)

#    define UDON_PLATFORM_HAS_STL __has_include(<vector>)
#    define UDON_PLATFORM_HAS_SERIAL_PRINTF 0
#    define UDON_PLATFORM_OUTPUT_STREAM UDON_PLATFORM_OUTPUT_CONSOLE
#    if defined(_M_IX86) || defined(_M_X64) || defined(_M_IA64) || defined(_M_ARM) || defined(_M_ARM64) || defined(_M_AMD64) || defined(_M_MRX000) || defined(_M_PPC) || defined(_M_ALPHA) || defined(_M_SH)
#        define UDON_PLATFORM_ENDIANNESS UDON_PLATFORM_LITTLE_ENDIAN
#    elif defined(_MIPSEB) || defined(_POWER) || defined(__s390__)
#        define UDON_PLATFORM_ENDIANNESS UDON_PLATFORM_BIG_ENDIAN
#    else
#        error "Unknown Endian"
#    endif

//-------------------------------------------------------------------
//
//    未定義のプラットフォーム
//
//-------------------------------------------------------------------

#else
#    error "Platform is not defined!"
#endif

//-------------------------------------------------------------------
//
//    未定義チェック
//
//-------------------------------------------------------------------

#ifndef UDON_PLATFORM_HAS_STL
#    error "UDON_PLATFORM_HAS_STL is not defined!"
#endif

#ifndef UDON_PLATFORM_HAS_SERIAL_PRINTF
#    error "UDON_PLATFORM_HAS_SERIAL_PRINTF is not defined!"
#endif

#ifndef UDON_PLATFORM_OUTPUT_STREAM
#    error "UDON_PLATFORM_OUTPUT_STREAM is not defined!"
#endif

#ifndef UDON_PLATFORM_ENDIANNESS
#    error "UDON_PLATFORM_ENDIANNESS is not defined!"
#endif
