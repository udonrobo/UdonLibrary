/// @file   CanWriter.hpp
/// @date   2023/01/15
/// @brief  Can通信送信クラス
/// @author 大河 祐介

#pragma once

#include <udon\com\can\CanBusInterface.hpp>
#include <udon\com\can\CanInfo.hpp>

namespace udon
{

    template <class MessageTy>
    class CanWriter
    {

        CanBusInterface& bus;
        CanNodeInfo      node;

        union
        {
            uint8_t   buffer[sizeof(MessageTy)];
            MessageTy message;
        } u;

    public:
        /// @param id 信号識別ID
        CanWriter(CanBusInterface& bus, const uint32_t id)
            : bus{ bus }
            , node{ id, u.buffer, sizeof(MessageTy), 0 }
            , u{}
        {
            bus.joinTX(node);
        }

        /// @param コピーコンストラクタ
        CanWriter(const CanWriter& rhs)
            : bus{ rhs.bus }
            , node{ rhs.node.id, u.buffer, sizeof(MessageTy), 0 }
            , u{}
        {
            bus.joinTX(node);
        }

        ~CanWriter()
        {
            bus.detachTX(node);
        }

        constexpr size_t length() const noexcept
        {
            return sizeof(MessageTy);
        }

        /// @brief 通信できているか
        operator bool() const noexcept
        {
            return micros() - node.timestampUs < 50000;
        }

        /// @brief メッセージ構造体をセット
        void setMessage(const MessageTy& message) noexcept
        {
            u.message = message;
        }

        void showBinary(char end = {}) const noexcept
        {
            for (auto&& it : u.buffer)
            {
                Serial.print(it);
                Serial.print('\t');
            }
            Serial.print(end);
        }

        /// @brief 受信内容を表示
        void show(char end = {}) const noexcept
        {
            u.message.show();
            Serial.print(end);
        }
    };

}    // namespace udon