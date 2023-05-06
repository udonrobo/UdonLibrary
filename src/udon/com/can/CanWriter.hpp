/// @file   CanWriter.hpp
/// @date   2023/01/15
/// @brief  Can通信送信クラス
/// @author 大河 祐介

#pragma once

#include <udon/com/can/ICanBus.hpp>
#include <udon/com/serializer/Serializer.hpp>

namespace udon
{

    template <class Message>
    class CanWriter
    {

        ICanBus&    bus;
        CanNodeView node;

    public:
        /// @param id 信号識別ID
        CanWriter(ICanBus& bus, const uint32_t id)
            : bus{ bus }
            , node{ bus.createTxNode(id, udon::CapacityWithChecksum<Message>()) }
        {
        }

        /// @brief メッセージ構造体をセット
        void setMessage(const Message& message) noexcept
        {
            *node.data = udon::Pack(message);
        }

        /// @brief 受信内容を表示
        void show(char end = {}) const noexcept
        {
            Serial.print(end);
        }
    };

}    // namespace udon