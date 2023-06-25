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

}    // namespace udon