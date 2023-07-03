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
//    エンディアン
//
//-------------------------------------------------------------------

#pragma once

#include <stdint.h>

namespace Udon
{

    enum class Endian
    {
        Little,
        Big,
    };

    /// @brief アーキテクチャのエンディアンを取得する
    /// @return エンディアン
    inline Endian GetEndian()
    {
        static const uint16_t endian = 0x0100;

        const bool isBig = *reinterpret_cast<const uint8_t*>(&endian) == 0x01;

        return isBig ? Endian::Big : Endian::Little;
    }

}    // namespace Udon
