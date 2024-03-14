//
//    CAN 通信
//
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

//
//    Raspberry Pi Pico 用バスクラス
//
#if defined(ARDUINO_ARCH_RP2040)
#    include "Can/CanBusSpiPico.hpp"
#endif

//
//    Teensy 用バスクラス
//
#if defined(TEENSYDUINO)
#    include "Can/CanBusTeensy.hpp"
#endif

//
//    送受信ノード
//
#include "Can/CanReader.hpp"
#include "Can/CanWriter.hpp"
