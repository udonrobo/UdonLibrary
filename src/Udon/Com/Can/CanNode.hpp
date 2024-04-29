//
//    CAN通信 送受信ノード
//
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <vector>

namespace Udon
{

    /// @brief CAN送信ノード
    struct CanTxNode
    {
        uint32_t id;    // メッセージID

        std::vector<uint8_t> data;    // 送信データ

        uint32_t transmitMs;    // 最終通信時刻
    };


    /// @brief CAN受信ノード
    struct CanRxNode
    {
        uint32_t id;    // メッセージID

        std::vector<uint8_t> data;    // 受信データ

        void (*onReceive)(void*);    // 受信時コールバック
        void* param;                 // コールバックパラメータ

        uint32_t transmitMs;    // 最終通信時刻

        void callback()
        {
            if (onReceive)
                onReceive(param);
        }
    };
}    // namespace Udon