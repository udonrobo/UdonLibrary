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
//    CANバス管理クラス インターフェース
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Com/Can/CanNode.hpp>

namespace Udon
{

    class ICanBus
    {
    public:
        virtual ~ICanBus() {}

        /// @brief バスの有効性を取得
        virtual operator bool() const = 0;

        /// @brief 送信ノードを登録
        /// @param node
        virtual void joinTx(CanNode& node) = 0;

        /// @brief 受信ノードを登録
        /// @param node
        /// @param onReceive 最終フレーム到達時に呼び出されるコールバック関数
        /// @param p コールバック関数の引数に渡すポインタ (thisなどを渡す)
        virtual void joinRx(CanNode& node, void (*onReceive)(void*), void* p) = 0;

        /// @brief 送信ノードを登録解除
        virtual void leaveTx(CanNode& node) = 0;

        /// @brief 受信ノードを登録解除
        virtual void leaveRx(CanNode& node) = 0;
    };
}    // namespace Udon