//
//    CANバス管理クラス インターフェース
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <vector>

namespace Udon
{

    /// @brief CAN送信ノード
    struct CanTxNode
    {
        const uint32_t id;    // メッセージID

        std::vector<uint8_t> data;    // 送信データ

        uint32_t transmitMs;    // 最終通信時刻
    };


    /// @brief CAN受信ノード
    struct CanRxNode
    {
        const uint32_t id;    // メッセージID

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


    /// @brief CANバス管理クラス インターフェース
    /// @note デバイスの差を吸収するためにインターフェース化
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