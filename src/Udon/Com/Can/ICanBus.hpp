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


        /// @brief 送信ノードを作成
        /// @note 既に同じIDのノードが存在する場合はそのノードを返す
        /// @param id メッセージID
        /// @param length データ長
        /// @return 送信ノードを指すポインタ
        virtual CanTxNode* createTx(uint32_t id, size_t length) = 0;


        /// @brief 受信ノードを作成
        /// @note 既に同じIDのノードが存在する場合はそのノードを返す (このときコールバック関数を登録した場合、上書きされる)
        /// @param id メッセージID
        /// @param length データ長
        /// @return 受信ノードを指すポインタ
        virtual CanRxNode* createRx(uint32_t id, size_t length) = 0;
    };
}    // namespace Udon