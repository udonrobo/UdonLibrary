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
//    CAN通信 送信クラス
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Com/Can/ICanBus.hpp>
#include <Udon/Com/Can/CanNode.hpp>

#include <Udon/Com/Serialization.hpp>
#include <Udon/Utility/Show.hpp>

namespace Udon
{

    template <typename Message>
    class CanWriter
    {
    public:
        /// @brief 受信メッセージ型
        using MessageType = Message;

        /// @brief 受信バッファサイズ
        static constexpr size_t Size = Udon::CapacityWithChecksum<MessageType>();

        /// @param id 信号識別ID
        CanWriter(ICanBus& bus, const uint32_t id)
            : bus{ bus }
            , node{ id, buffer, sizeof buffer, 0 }
        {
            bus.joinTx(node);
        }

        /// @brief コピーコンストラクタ
        CanWriter(const CanWriter& other)
        {
            bus.joinTx(node);
        }

        /// @brief デストラクタ
        ~CanWriter()
        {
            bus.leaveTx(node);
        }

        /// @brief メッセージ構造体をセット
        void setMessage(const Message& message) noexcept
        {
            Udon::Pack(message, node.data, node.length);
        }

        /// @brief 送信内容を表示
        /// @param gap 区切り文字 (default: '\t')
        void show(char gap = '\t') const
        {
            if (const auto message = Udon::Unpack<Message>(node.data, node.length))
            {
                Udon::Show(*message, gap);
            }
            else
            {
                Udon::Show(F("unpack failed!"));
                // ここへ到達する: setMessage()で値を設定していない
            }
        }

        /// @brief 送信バッファを表示
        /// @param gap 区切り文字 (default: ' ')
        void showRaw(char gap = ' ') const
        {
            for (size_t i = 0; i < node.length; ++i)
            {
                Udon::Show(node.data[i], gap);
            }
        }

    private:
        ICanBus& bus;

        uint8_t buffer[Size];

        CanNode node;
    };

}    // namespace Udon