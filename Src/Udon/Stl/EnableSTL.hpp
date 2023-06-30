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
//    C++ STL 関係
//
//-------------------------------------------------------------------


#pragma once

// C++ STL を使用できるようにします

#undef abs

#if !__has_include(<vector>)
#    include <UdonArduinoSTL.hpp>
#endif
