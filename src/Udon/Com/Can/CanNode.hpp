//
//    CAN通信 送受信ノード
//
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include <stdint.h>
#include <stddef.h>

namespace Udon
{
    struct CanNode
    {
        uint32_t id;            // メッセージID
        uint8_t* data;          // バッファを指すポインタ
        size_t   length;        // バッファの長さ
        uint32_t transmitMs;    // 最終通信時刻
    };
}    // namespace Udon