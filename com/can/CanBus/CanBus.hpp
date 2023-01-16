/// @file   CanBus.hpp
/// @date   2023/01/13
/// @brief  CANバスを管理するクラス群
/// @author 大河 祐介

#pragma once

//     vvv teensy3.2 vvv        vvv teensy3.5 vvv         vvv teensy3.6 vvv       vvv teensy4.0/4.1 vvv
#if defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(__IMXRT1062__)
#	include "CanBusTeensy.hpp"
#endif

#if 0 //defined(ARDUINO_RASPBERRY_PI_PICO)
#	include "CanBusPico.hpp"
#endif

#include "CanBusSpi.hpp"
