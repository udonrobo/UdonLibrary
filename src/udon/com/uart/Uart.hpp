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
