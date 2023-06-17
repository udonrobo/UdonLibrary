//
//    Library: libcrc
//    Author:  Lammert Bies
//
//    This file is licensed under the MIT License as stated below
//
//    Copyright (c) 1999-2016 Lammert Bies
//
//    Permission is hereby granted, free of charge, to any person obtaining a copy
//    of this software and associated documentation files (the "Software"), to deal
//    in the Software without restriction, including without limitation the rights
//    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//    copies of the Software, and to permit persons to whom the Software is
//    furnished to do so, subject to the following conditions:
//
//    The above copyright notice and this permission notice shall be included in all
//    copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//    SOFTWARE.
//

#pragma once

#include <stddef.h>

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

            for (uint8_t bit = 0; bit < 8 /* CHAR_BIT */ ; ++bit)
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
