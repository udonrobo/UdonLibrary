//
//    UART
//
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#ifdef ARDUINO
#    include "Uart/UartReader.hpp"
#    include "Uart/UartWriter.hpp"
#endif

#ifdef SIV3D_INCLUDED
#    include "Uart/SivUartReader.hpp"
#    include "Uart/SivUartWriter.hpp"
#endif

#if defined(ROS_VERSION) && (ROS_VERSION == ROS_VERSION_FOXY || ROS_VERSION == ROS_VERSION_HUMBLE)
#    include "Uart/Ros2UartReader.hpp"
#    include "Uart/Ros2UartWriter.hpp"
#endif