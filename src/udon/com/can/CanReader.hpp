/// @file   CanReader.hpp
/// @date   2023/01/15
/// @brief  Can通信受信クラス
/// @author 大河 祐介

#pragma once

#include <udon/com/can/ICanBus.hpp>
#include <udon/com/serialization/Serializer.hpp>

namespace udon
{

    template <class Message>
    class CanReader
    {

        ICanBus&    bus;
        CanNodeView node;

        Message message;

    public:
        /// @param id 信号識別ID
        CanReader(ICanBus& bus, const uint32_t id)
            : bus{ bus }
            , node{ bus.createRxNode(id, udon::CapacityWithChecksum<Message>()) }
            , message{}
        {
        }

        void update()
        {
            if (const auto unpacked = udon::Unpack<Message>(*node.data))
            {
                message = *unpacked;
            }
        }

        /// @brief メッセージ構造体を取得
        Message getMessage() const
        {
            return message;
        }

        /// @brief 受信内容を表示
        void show(char end = {}) const
        {
            message.show();
            Serial.print(end);
        }
    };

}    // namespace udon