/// @file   CANBus.hpp
/// @date   2023/01/13
/// @brief  CANバスを管理するクラス群
/// @author 大河 祐介

#pragma once

#include "CANBusTeensy.hpp"

#if 0 //defined(ARDUINO_RASPBERRY_PI_PICO)
#	include "CANBusPico.hpp"
#endif

#include "CANBusSPI.hpp"
