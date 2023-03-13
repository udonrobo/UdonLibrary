/// @file   CanBus.hpp
/// @date   2023/01/13
/// @brief  Canバスを管理するクラス群
/// @author 大河 祐介

#pragma once

#include "CanBusTeensy.hpp"

#if 0 //defined(ARDUINO_RASPBERRY_PI_PICO)
#	include "CanBusPico.hpp"
#endif

#include "CanBusSPI.hpp"
