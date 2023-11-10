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
//    DIPスイッチ
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <vector>
#include <stdint.h>

#ifdef ARDUINO

namespace Udon
{

    inline int DecodeDipSwitch(std::initializer_list<uint8_t> pins)
    {
        for (auto&& pin : pins)
        {
            pinMode(pin, INPUT_PULLUP);
        }

        int dec = 0;
        int i   = 0;

        for (auto&& pin : pins)
        {
            bitWrite(dec, i++, not digitalRead(pin));
            delayMicroseconds(1);  // チャタリング対策
        }

        return dec;
    }

}    // namespace Udon

#endif