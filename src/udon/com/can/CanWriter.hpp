/// @file   CanWriter.hpp
/// @date   2023/01/15
/// @brief  Can通信送信クラス
/// @author 大河 祐介

#pragma once

#include <udon/com/can/ICanBus.hpp>
#include <udon/com/serialization/Serializer.hpp>

namespace udon
{

    template <class Message>
    class CanWriter
    {

        ICanBus&    bus;
        CanNodeView node;

        Message message;

    public:
        /// @param id 信号識別ID
        CanWriter(ICanBus& bus, const uint32_t id)
            : bus{ bus }
            , node{ bus.createTxNode(id, udon::CapacityWithChecksum<Message>()) }
            , message{}
        {
        }

        /// @brief メッセージ構造体をセット
        void setMessage(const Message& message) noexcept
        {
            this->message = message;
            *node.data    = udon::Pack(message);
        }

        /// @brief 送信内容を表示
        void show(char end = {}) const noexcept
        {
            message.show();
            Serial.print(end);
        }
    };

}    // namespace udon