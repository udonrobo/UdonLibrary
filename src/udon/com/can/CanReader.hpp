/// @file   CanReader.hpp
/// @date   2023/01/15
/// @brief  CAN 通信受信クラス
/// @author 大河 祐介

#pragma once

#include <udon/com/can/ICanBus.hpp>
#include <udon/com/serialization/Serializer.hpp>
#include <udon/utility/Show.hpp>

namespace udon
{

    template <class Message>
    class CanReader
    {

        ICanBus& bus;

        CanNodeView node;

    public:
    
        /// @brief コンストラクタ
        /// @param bus I2cバス
        /// @param id 信号識別ID
        CanReader(ICanBus& bus, const uint32_t id)
            : bus{ bus }
            , node{ bus.createRxNode(id, udon::CapacityWithChecksum<Message>()) }
        {
        }

        /// @brief 受信しているか
        /// @return 受信していればtrue
        explicit operator bool() const
        {
            return millis() - *node.transmitMs < 100;
        }

        /// @brief メッセージ構造体を取得
        /// @return メッセージ構造体(optional)        
        udon::optional<Message> getMessage() const
        {
            return udon::Unpack<Message>(*node.data);
        }

        /// @brief 受信内容を表示
        /// @param gap 区切り文字 (default: '\t')
        void show(char gap = '\t') const
        {
            if (const auto message = getMessage())
            {
                udon::Show(*message, gap);
            }
            else
            {
                Serial.print(F("receive failed!"));
            }
        }

        /// @brief 受信バッファを表示
        /// @param gap 区切り文字 (default: ' ')
        void showRaw(char gap = ' ') const
        {
            for (auto&& it : *node.data)
            {
                Serial.print(it);
                Serial.print(gap);
            }
        }
    };

}    // namespace udon