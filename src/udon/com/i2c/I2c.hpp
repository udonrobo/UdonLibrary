//-----------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-----------------------------------------------
//
//    I2c 通信
//
//-----------------------------------------------

#pragma once

#ifdef ARDUINO
#    include <udon/com/i2c/I2cBus.hpp>
#    include <udon/com/i2c/I2cMasterReader.hpp>
#    include <udon/com/i2c/I2cMasterWriter.hpp>
#    include <udon/com/i2c/I2cSlaveReader.hpp>
#    include <udon/com/i2c/I2cSlaveWriter.hpp>
#endif
