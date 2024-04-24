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
    struct CanTxNode
    {
        uint32_t id;            // メッセージID
        std::vector<uint8_t> data;
        uint32_t transmitMs;    // 最終通信時刻
    };
    struct CanRxNode
    {
        uint32_t id;            // メッセージID
        uint32_t transmitMs;    // 最終通信時刻

        std::vector<uint8_t> data;

        void (*onReceive)(void*);

        void* param;

        void callback()
        {
            if (onReceive)
                onReceive(param);
        }
    };
}    // namespace Udon