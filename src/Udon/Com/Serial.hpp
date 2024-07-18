//
//    Serial
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#ifdef ARDUINO
#    include "Serial/SerialReader.hpp"
#    include "Serial/SerialWriter.hpp"
#endif

#ifdef SIV3D_INCLUDED
#    include "Serial/SivSerialReader.hpp"
#    include "Serial/SivSerialWriter.hpp"
#endif

#if defined(ROS_VERSION) && (ROS_VERSION == ROS_VERSION_FOXY || ROS_VERSION == ROS_VERSION_HUMBLE)
#    include "Serial/Ros2SerialReader.hpp"
#    include "Serial/Ros2SerialWriter.hpp"
#endif