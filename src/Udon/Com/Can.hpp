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
//    CAN 通信
//
//-------------------------------------------------------------------

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

// 仮想ノードクラス
#include "Can/CanReader.hpp"
#include "Can/CanWriter.hpp"

#include "Pad/CanPadPS5.hpp"
