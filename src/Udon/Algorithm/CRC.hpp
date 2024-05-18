//
//    CRC
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#include <Udon/Types/ArrayView.hpp>

namespace Udon
{
    constexpr size_t CRC8_SIZE = 1;

    /// @brief バイト列から CRC-8 値を計算する
    /// @param buffer CRCの計算対象となるバッファ
    inline uint8_t CRC8(ArrayView<const uint8_t> buffer)
    {
        uint8_t crc = 0xFF;    // initial value

        for (const auto& it : buffer)
        {
            crc ^= it;

            for (uint8_t bit = 0; bit < 8 /* CHAR_BIT */; ++bit)
            {
                if (crc & 0x80)
                {
                    crc = (crc << 1) ^ 0x1D;    // polynomial
                }
                else
                {
                    crc <<= 1;
                }
            }
        }

        return crc ^ 0xFF;    // final XOR
    }

}    // namespace Udon
