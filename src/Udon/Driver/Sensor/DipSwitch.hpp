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

    class DipSwitch
    {
        std::vector<uint8_t> pins;

    public:
        /// @brief constructor
        /// @param pins dipスイッチのピン(下位ビットから)
        DipSwitch(const std::vector<uint8_t>& pins)
            : pins(pins)
        {
            for (auto&& pin : pins)
            {
                pinMode(pin, INPUT_PULLUP);
            }
        }

        /// @brief  10進数にデコード
        /// @return 10進数
        int decode() const
        {
            int dec = 0;
            for (size_t i = 0; i < pins.size(); i++)
            {
                bitWrite(dec, i, not digitalRead(pins[i]));
                delayMicroseconds(1);
            }
            return dec;
        }
    };

}    // namespace Udon

#endif     