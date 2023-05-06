/// @file   CanReader.hpp
/// @date   2023/01/15
/// @brief  Can通信受信クラス
/// @author 大河 祐介

#pragma once

#include <udon/com/can/ICanBus.hpp>
#include <udon/com/serializer/Serializer.hpp>

namespace udon
{

    template <class Message>
    class CanReader
    {

        ICanBus&    bus;
        CanNodeView node;

    public:
        /// @param id 信号識別ID
        CanReader(ICanBus& bus, const uint32_t id)
            : bus{ bus }
            , node{ bus.createRxNode(id, udon::CapacityWithChecksum<Message>()) }
        {
        }

        /// @brief メッセージ構造体を取得
        udon::optional<Message> getMessage() const noexcept
        {
            return udon::Unpack<Message>(*node.data);
        }

        /// @brief 受信内容を表示
        void show(char end = {}) const noexcept
        {
            getMessage().show();
            Serial.print(end);
        }
    };

}    // namespace udon