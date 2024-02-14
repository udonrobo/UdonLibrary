//
//    LoRa インターフェース
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <vector>
#include <stdint.h>

#include <Udon/Types/ArrayView.hpp>

namespace Udon
{

    struct LoRaNode
    {
        uint8_t* data;          // バッファを指すポインタ
        uint8_t  size;          // バッファの長さ
        uint32_t transmitMs;    // 最終通信時刻
    };

    /// @brief LoRaのインターフェース
    /// @details
    ///     LoRaの通信を行うクラスはこのインターフェースを実装する
    class ILoRa
    {
    public:
        virtual ~ILoRa() {}

        /// @brief バスの有効性を取得
        virtual operator bool() const = 0;

        /// @brief 送信ノードを登録
        /// @param node
        virtual void joinTx(LoRaNode& node) = 0;

        /// @brief 受信ノードを登録
        virtual void joinRx(LoRaNode& node) = 0;
    };
}    // namespace Udon