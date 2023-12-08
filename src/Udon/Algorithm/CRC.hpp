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
//    CRC チェックサム
//    ref: https://www.sunshine2k.de/coding/javascript/crc/crc_js.html
//         https://www.sunshine2k.de/articles/coding/crc/understanding_crc.html
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>
#include <iterator>
#include <stddef.h>
#include <stdint.h>

namespace Udon
{
    constexpr size_t CRC8_SIZE = 1;

    /// @brief CRC-8 checksum
    /// @tparam Iterator
    /// @param begin     begin iterator of source byte string
    /// @param end       end iterator of source byte string
    /// @return          checksum
    template <typename Iterator>
    inline uint8_t CRC8(Iterator begin, Iterator end)
    {
        static_assert(std::is_same<typename std::iterator_traits<Iterator>::value_type, uint8_t>::value, "Iterator type must be uint8_t.");

        uint8_t crc = 0xFF;    // initial value

        for (auto it = begin; it != end; ++it)
        {
            crc ^= *it;

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

    /// @brief CRC-8 checksum
    /// @param p         pointer of source byte string
    /// @param length    length of source byte string
    /// @return          checksum
    inline uint8_t CRC8(const uint8_t* p, size_t length)
    {
        return CRC8(p, p + length);
    }

}    // namespace Udon
