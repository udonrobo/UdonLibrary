/// @file   CanWriter.hpp
/// @date   2023/01/15
/// @brief  Can通信送信クラス
/// @author 大河 祐介

#pragma once

#include <udon/com/can/CanBusInterface.hpp>
#include <udon/com/can/CanInfo.hpp>

namespace udon
{

    template <class MessageTy>
    class CanWriter
    {

        CanBusInterface& bus;
        CanNodeInfo      node;

        static constexpr size_t capacity = udon::CapacityWithChecksum<MessageTy>();

        uint8_t buffer[capacity];

    public:
        /// @param id 信号識別ID
        CanWriter(CanBusInterface& bus, const uint32_t id)
            : bus{ bus }
            , node{ id, buffer, capacity, 0 }
            , buffer{}
        {
            bus.joinTX(node);
        }

        /// @param コピーコンストラクタ
        CanWriter(const CanWriter& rhs)
            : bus{ rhs.bus }
            , node{ rhs.node.id, buffer, capacity, 0 }
            , buffer{}
        {
            bus.joinTX(node);
        }

        ~CanWriter()
        {
            bus.detachTX(node);
        }

        /// @brief 通信できているか
        operator bool() const noexcept
        {
            return micros() - node.timestampUs < 50000;
        }

        /// @brief メッセージ構造体をセット
        void setMessage(const MessageTy& message) noexcept
        {
            udon::Pack(message, buffer);
        }

        void showBinary(char end = {}) const noexcept
        {
            for (auto&& it : buffer)
            {
                Serial.print(it);
                Serial.print('\t');
            }
            Serial.print(end);
        }

        /// @brief 受信内容を表示
        void show(char end = {}) const noexcept
        {
            Serial.print(end);
        }
    };

}    // namespace udon