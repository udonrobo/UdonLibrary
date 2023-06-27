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
//    IM920 
//
//    Sender --[UART]--> IM920 ~~[920MHz]~~> IM920 --[UART]--> Receiver
//    ^^^^^^                                                   ^^^^^^^^
//
//-----------------------------------------------


#pragma once

#ifdef ARDUINO
#    include <udon/com/im920/Im920Normal.hpp>
#    include <udon/com/im920/Im920Reader.hpp>
#    include <udon/com/im920/Im920Writer.hpp>
#    include <udon/com/im920/Im920sl.hpp>
#endif