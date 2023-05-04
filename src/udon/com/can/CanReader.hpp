/// @file   CanReader.hpp
/// @date   2023/01/15
/// @brief  Can通信受信クラス
/// @author 大河 祐介

#pragma once

#include <udon/com/can/CanBusInterface.hpp>
#include <udon/com/can/CanInfo.hpp>
#include <udon/com/serializer/Serializer.hpp>

namespace udon
{

    template <class Message>
    class CanReader
    {

        CanBusInterface& bus;
        CanNodeInfo      node;
        uint8_t          buffer[udon::CapacityWithChecksum<Message>()];

    public:
        /// @param id 信号識別ID
        CanReader(CanBusInterface& bus, const uint32_t id)
            : bus{ bus }
            , node{ id, buffer, sizeof(Message), 0 }
            , buffer{}
        {
            bus.joinRX(node);
        }

        /// @param コピーコンストラクタ
        CanReader(const CanReader& rhs)
            : bus{ rhs.bus }
            , node{ rhs.node.id, buffer, sizeof(Message), 0 }
            , buffer{}
        {
            bus.joinRX(node);
        }

        ~CanReader()
        {
            bus.detachRX(node);
        }

        constexpr size_t length() const noexcept
        {
            return sizeof(Message);
        }
        constexpr const uint8_t* data() const noexcept
        {
            return buffer;
        }

        /// @brief 通信できているか
        operator bool() const noexcept
        {
            return micros() - node.timestampUs < 50000;
        }

        /// @brief メッセージ構造体を取得
        Message getMessage() const noexcept
        {
            if(const auto unpacked = udon::Unpack<Message>(buffer))
            {
                return *unpacked;
            }
            return {};
        }

        /// @brief 受信内容を表示
        void show(char end = {}) const noexcept
        {
            getMessage().show();
            Serial.print(end);
        }
    };

}    // namespace udon