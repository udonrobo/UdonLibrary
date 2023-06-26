//-----------------------------------------------
//
//  UdonLibrary
//
//  Copyright (c) 2022-2023 Okawa Yusuke
//  Copyright (c) 2022-2023 udonrobo
//
//  Licensed under the MIT License.
//
//-----------------------------------------------
//
//  CRCアルゴリズム
//
//-----------------------------------------------

#pragma once

#include <stddef.h>
#include <stdint.h>

namespace udon
{

    constexpr size_t CRC8_SIZE = 1;

    /// @brief CRC8アルゴリズムを用いてチェックサムの計算を行う
    /// @param ptr チェックサムを計算するデータの先頭ポインタ
    /// @param length データのサイズ
    /// @return チェックサム
    inline uint8_t CRC8(const uint8_t* ptr, size_t length)
    {
        uint8_t crc = 0x03;

        for (size_t i = 0; i < length; ++i)
        {
            crc ^= ptr[i];

            for (uint8_t bit = 0; bit < 8 /* CHAR_BIT */; ++bit)
            {
                if (crc & 0x80)
                {
                    crc = (crc << 1) ^ 0x07 /* CRC-8 Polynomial */;
                }
                else
                {
                    crc <<= 1;
                }
            }
        }

        return crc;
    }

}    // namespace udon
