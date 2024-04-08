//
//    CAN通信 送信クラス
//
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include "ICanBus.hpp"
#include "CanNode.hpp"

#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Common/Show.hpp>
#include <Udon/Serializer/SerializerTraits.hpp>
#include <Udon/Common/Printf.hpp>

namespace Udon
{

    template <typename Message>
    class CanWriter
    {

        static_assert(Udon::Traits::IsSerializable<Message>::value, "Message must be parsable.");

    public:
        /// @brief 受信メッセージ型
        using MessageType = Message;

        /// @brief 受信バッファサイズ
        static constexpr size_t Size = Udon::SerializedSize<MessageType>();

        /// @brief コンストラクタ
        /// @param bus CANバス
        /// @param id  自身のノードID
        CanWriter(ICanBus& bus, const uint32_t id)
            : bus{ bus }
            , node{ id, buffer, sizeof buffer, 0 }
        {
            bus.joinTx(node);
        }

        /// @brief コピーコンストラクタ
        CanWriter(const CanWriter& other)
            : bus{ other.bus }
            , buffer{}
            , node{ other.node.id, buffer, Size, 0 }
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
            Udon::Serialize(message, { node.data, node.length });
        }

        /// @brief 送信内容を表示
        /// @param gap 区切り文字 (default: '\t')
        void show(char gap = '\t') const
        {
            Udon::Printf("0x%03x ", node.id);
            if (const auto message = Udon::Deserialize<Message>({ node.data, node.length }))
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
            Udon::Printf("0x%03x ", node.id);
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