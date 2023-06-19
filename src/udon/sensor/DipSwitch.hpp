#pragma once

#include <udon/stl/EnableSTL.hpp>

#include <stdint.h>
#include <vector>


#include <Arduino.h>

namespace udon
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

}    // namespace udon
