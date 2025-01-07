//
//    STL を使用できない環境で STL を使用できるようにする
//
//    Copyright (c) 2022 udonrobo
//

#pragma once

#undef abs

#if !__has_include(<vector>)
#    include <UdonArduinoSTL.hpp>
#endif
