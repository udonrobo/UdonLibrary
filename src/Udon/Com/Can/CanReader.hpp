//
//    CAN通信 受信クラス
//
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
        /// @param bus CANバス
        /// @param id 送信者のノードID
        CanReader(ICanBus& bus, const uint32_t id)
            : bus{ bus }
        {
            node = bus.createRx(id,Size);
            node->onReceive = [](void* p)
            {
                auto self = static_cast<CanReader*>(p);
                self->message = Udon::Deserialize<Message>({ self->node->data, self->node->length });
            };
            node->param = this;
        }

        /// @brief コピーコンストラクタ
        CanReader(const CanReader& other)
            : bus{ other.bus }
            , node{other.node}
        {
            node->param = this;
        }

        /// @brief デストラクタ
        ~CanReader()
        {
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
            Udon::Printf("0x%03x ", node->id);
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
            Udon::Printf("0x%03x ", node->id);
            for (size_t i = 0; i < node->length; ++i)
            {
                Udon::Show(node->data[i]);
            }
        }

    private:
        ICanBus& bus;

        CanRxNode* node;

        Udon::Optional<MessageType> message;
    };

}    // namespace Udon
