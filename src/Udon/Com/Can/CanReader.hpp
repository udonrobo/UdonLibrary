//
//    CAN通信 受信クラス
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include "ICanBus.hpp"
#include "CanNode.hpp"

#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Serializer/SerializerTraits.hpp>
#include <Udon/Common/Show.hpp>
#include <Udon/Common/Time.hpp>
#include <Udon/Common/Printf.hpp>

namespace Udon
{

    template <typename Message>
    class CanReader
    {

        static_assert(Traits::IsSerializable<Message>::value, "Message must be parsable.");

    public:
        /// @brief 受信メッセージ型
        using MessageType = Message;

        /// @brief 受信バッファサイズ
        static constexpr size_t Size = Udon::SerializedSize<Message>();

        /// @brief コンストラクタ
        /// @param bus I2cバス
        /// @param id 送信者のノードID
        CanReader(ICanBus& bus, const uint32_t id)
            : bus{ bus }
            , buffer{}
            , node{ id, buffer, Size, 0 }
        {
            joinBus();
        }

        /// @brief コピーコンストラクタ
        CanReader(const CanReader& other)
            : bus{ other.bus }
            , buffer{}
            , node{ other.node.id, buffer, Size, 0 }
        {
            joinBus();
        }

        /// @brief デストラクタ
        ~CanReader()
        {
            bus.leaveRx(node);
        }

        /// @brief 受信しているか
        /// @return 受信していればtrue
        explicit operator bool() const
        {
            return Millis() - node.transmitMs < 100;
        }

        /// @brief メッセージ構造体を取得
        /// @return メッセージ構造体(Optional)
        Udon::Optional<MessageType> getMessage() const
        {
            if (*this)
            {
                return message;
            }
            else
            {
                return Udon::nullopt;
            }
        }

        /// @brief 受信内容を表示
        void show() const
        {
            Udon::Printf("0x%03x ", node.id);
            if (const auto m = getMessage())
            {
                Udon::Show(*m);
            }
            else
            {
                Udon::Show(F("receive failed!"));
            }
        }

        /// @brief 受信バッファを表示
        void showRaw() const
        {
            Udon::Printf("0x%03x ", node.id);
            for (size_t i = 0; i < node.length; ++i)
            {
                Udon::Show(node.data[i]);
            }
        }

    private:
        void joinBus()
        {
            bus.joinRx(
                node,
                [](void* p)
                {
                    auto self     = static_cast<CanReader*>(p);
                    self->message = Udon::Deserialize<MessageType>({ self->node.data, self->node.length });
                },
                this);
        }

        ICanBus& bus;

        uint8_t buffer[Size];

        CanNode node;

        Udon::Optional<MessageType> message;
    };

}    // namespace Udon
