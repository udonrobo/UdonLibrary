//
//    CANバス管理クラス インターフェース
//
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include "CanNode.hpp"

namespace Udon
{
    class ICanBus
    {
    public:
        virtual ~ICanBus() {}

        /// @brief 送信ノードを登録
        /// @param node
        virtual CanTxNode* createTx(uint32_t id , size_t length ) = 0;

        /// @brief 受信ノードを登録
        /// @param node
        /// @param onReceive 最終フレーム到達時に呼び出されるコールバック関数
        /// @param p コールバック関数の引数に渡すポインタ (thisなどを渡す)
        virtual CanRxNode* createRx(uint32_t id, size_t length) = 0;


    };
}    // namespace Udon