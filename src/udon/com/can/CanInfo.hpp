#pragma once

#include <udon/stl/EnableSTL.hpp>

#include <stdint.h>
#include <vector>

namespace udon
{

    /// @brief CanBusクラス内のCanNodeのデータを参照するためのクラス
    struct CanNodeView
    {
        uint32_t              id;
        std::vector<uint8_t>* data;          // point to can bus data
        const uint32_t*       transmitMs;    // timestamp
    };

    struct CanBusErrorInfo
    {
        uint8_t  TXErrorCount;
        uint8_t  RXErrorCount;
        uint32_t transmitMs;

        explicit operator bool() const
        {
            return TXErrorCount < 100 &&
                   RXErrorCount < 100 &&
                   micros() - transmitMs < 50000;
        }
    };

}    // namespace udon