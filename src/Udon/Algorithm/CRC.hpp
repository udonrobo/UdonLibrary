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
    namespace ChecksumAlgorithm
    {

        struct CRC8
        {
            uint8_t polynomial;
            uint8_t initial;
            uint8_t finalXOR;
        };

        constexpr CRC8 CRC8_CCITT          = { 0x07, 0x00, 0x00 };
        constexpr CRC8 CRC8_DALLAS         = { 0x31, 0x00, 0x00 };
        constexpr CRC8 CRC8_SAE_J1850      = { 0x1D, 0xFF, 0xFF };
        constexpr CRC8 CRC8_SAE_J1850_ZERO = { 0x1D, 0xFF, 0x00 };
        constexpr CRC8 CRC8_WCDMA          = { 0x9B, 0x00, 0x00 };
        constexpr CRC8 CRC8_MAXIM          = { 0x31, 0x00, 0x00 };
        constexpr CRC8 CRC8_ROHC           = { 0x07, 0xFF, 0x00 };
        constexpr CRC8 CRC8_ITU            = { 0x07, 0x00, 0x55 };
        constexpr CRC8 CRC8_GSM_A          = { 0x1D, 0x00, 0x00 };
        constexpr CRC8 CRC8_GSM_B          = { 0x49, 0x00, 0xFF };
        constexpr CRC8 CRC8_AUTOSAR        = { 0x2F, 0xFF, 0xFF };
        constexpr CRC8 CRC8_BLUETOOTH      = { 0xA7, 0x00, 0x00 };
        constexpr CRC8 CRC8_LTE            = { 0x9B, 0x00, 0x00 };
        constexpr CRC8 CRC8_MAXIM_DOW      = { 0x31, 0x00, 0xFF };
        constexpr CRC8 CRC8_RFC_1662       = { 0x07, 0xFF, 0x00 };
        constexpr CRC8 CRC8_OPENSAFETY     = { 0x2F, 0x00, 0x00 };
        constexpr CRC8 CRC8_CDMA2000       = { 0x9B, 0xFF, 0x00 };
        constexpr CRC8 CRC8_DARC           = { 0x39, 0x00, 0x00 };
        constexpr CRC8 CRC8_DVB_S2         = { 0xD5, 0x00, 0x00 };
        constexpr CRC8 CRC8_EBU            = { 0x1D, 0xFF, 0x00 };
        constexpr CRC8 CRC8_ICODE          = { 0x1D, 0xFD, 0x00 };

    }    // namespace ChecksumAlgorithm

    constexpr size_t CRC8_SIZE = 1;

    /// @brief CRC-8 checksum
    /// @tparam Iterator
    /// @param begin     begin iterator of source byte string
    /// @param end       end iterator of source byte string
    /// @param algorithm checksum algorithm
    /// @return          checksum
    template <typename Iterator>
    inline uint8_t CRC8(Iterator begin, Iterator end, ChecksumAlgorithm::CRC8 algorithm = ChecksumAlgorithm::CRC8_SAE_J1850)
    {
        static_assert(std::is_same<typename std::iterator_traits<Iterator>::value_type, uint8_t>::value, "Iterator type must be uint8_t.");

        uint8_t crc = algorithm.initial;

        for (auto it = begin; it != end; ++it)
        {
            crc ^= *it;

            for (uint8_t bit = 0; bit < 8 /* CHAR_BIT */; ++bit)
            {
                if (crc & 0x80)
                {
                    crc = (crc << 1) ^ algorithm.polynomial;
                }
                else
                {
                    crc <<= 1;
                }
            }
        }

        return crc ^ algorithm.finalXOR;
    }

    /// @brief CRC-8 checksum
    /// @param p         pointer of source byte string
    /// @param length    length of source byte string
    /// @param algorithm checksum algorithm
    /// @return          checksum
    inline uint8_t CRC8(const uint8_t* p, size_t length, ChecksumAlgorithm::CRC8 algorithm = ChecksumAlgorithm::CRC8_SAE_J1850)
    {
        return CRC8(p, p + length);
    }

}    // namespace Udon
