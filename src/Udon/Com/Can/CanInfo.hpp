#pragma once

#include <Udon/Stl/EnableSTL.hpp>

#include <stdint.h>
#include <vector>

namespace Udon
{

    /// @brief CanBusクラス内のCanNodeのデータを参照するためのクラス
    struct CanNodeView
    {
        uint32_t              id;
        std::vector<uint8_t>* data;          // point to can bus data
        const uint32_t*       transmitMs;    // timestamp
    };

}    // namespace Udon