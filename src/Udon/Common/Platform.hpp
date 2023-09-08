#pragma once

//-------------------------------------------------------------------
//
//    エンディアン定義
//
//-------------------------------------------------------------------

#define UDON_LITTLE_ENDIAN
#define UDON_BIG_ENDIAN

//-------------------------------------------------------------------
//
//    Arduino Nano / Uno / Mega
//
//-------------------------------------------------------------------

#if defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA) || defined(ARDUINO_AVR_MEGA2560)

#    define UDON_PLATFORM_HAS_STL 0
#    define UDON_PLATFORM_HAS_SERIAL_PRINTF 0
#    define UDON_PLATFORM_ENDIANNESS UDON_LITTLE_ENDIAN

//-------------------------------------------------------------------
//
//    Teensy
//
//-------------------------------------------------------------------

#elif defined(TEENSYDUINO)

#    define UDON_PLATFORM_HAS_STL 1
#    define UDON_PLATFORM_HAS_SERIAL_PRINTF 1
#    define UDON_PLATFORM_ENDIANNESS UDON_LITTLE_ENDIAN

//-------------------------------------------------------------------
//
//    Raspberry Pi Pico
//
//-------------------------------------------------------------------

#elif defined(ARDUINO_ARCH_RP2040)

#    define UDON_PLATFORM_HAS_STL 1
#    define UDON_PLATFORM_HAS_SERIAL_PRINTF 1
#    define UDON_PLATFORM_ENDIANNESS UDON_LITTLE_ENDIAN

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

#ifndef UDON_PLATFORM_ENDIANNESS
#    error "UDON_PLATFORM_ENDIANNESS is not defined!"
#endif
