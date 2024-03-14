//
//    CRC checksum
//
//    Copyright (c) 2022-2023 udonrobo
//

//
//    About CRC:
//    https://www.sunshine2k.de/coding/javascript/crc/crc_js.html
//    https://www.sunshine2k.de/articles/coding/crc/understanding_crc.html
//

#pragma once

#include <Udon/Types/ArrayView.hpp>

namespace Udon
{
    constexpr size_t CRC8_SIZE = 1;

    /// @brief CRC-8 checksum
    /// @param buffer チェックサムを計算するバッファ
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
