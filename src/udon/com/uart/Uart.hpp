//-----------------------------------------------
//
//	UdonLibrary
// 
//	Copyright (c) 2022-2023 Okawa Yusuke
//	Copyright (c) 2022-2023 udonrobo
//
//	Licensed under the MIT License.
//
//-----------------------------------------------
//
//  UART 通信
//
//-----------------------------------------------


#pragma once

#ifdef ARDUINO
#    include <udon/com/uart/UartReader.hpp>
#    include <udon/com/uart/UartWriter.hpp>
#endif

#ifdef SIV3D_INCLUDED
#    include <udon/com/uart/SivUartReader.hpp>
#    include <udon/com/uart/SivUartWriter.hpp>
#endif
