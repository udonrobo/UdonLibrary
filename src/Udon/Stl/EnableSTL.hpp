//
//    C++ STL
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    C++ STL を使用できるようにします
//

#pragma once

#undef abs

#if !__has_include(<vector>)
#    include <UdonArduinoSTL.hpp>
#endif
