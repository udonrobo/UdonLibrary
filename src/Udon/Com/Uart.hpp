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
//    UART 通信
//
//-------------------------------------------------------------------

#pragma once

#ifdef ARDUINO
#    include <Udon/Com/Uart/UartReader.hpp>
#    include <Udon/Com/Uart/UartWriter.hpp>
#endif

#ifdef SIV3D_INCLUDED
#    include <Udon/Com/Uart/SivUartReader.hpp>
#    include <Udon/Com/Uart/SivUartWriter.hpp>
#endif

#ifdef ROS_VERSION == ROS_VERSION_FOXY || ROS_VERSION == ROS_VERSION_HUMBLE
#    include <Udon/Com/Uart/Ros2UartReader.hpp>
#    include <Udon/Com/Uart/Ros2UartWriter.hpp>
#endif