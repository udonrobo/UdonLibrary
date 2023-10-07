#pragma once

#include "LLora.hpp"

#include <Udon/Com/Serialization.hpp>
#include <Udon/Common/Show.hpp>

namespace Udon
{

    template <typename Message>
    class LoraWriter
    {
    public:
        static constexpr size_t Size = Udon::CapacityWithChecksum<Message>();

        using MessageType = Message;

    private:
        LLora& lora;

        uint8_t buffer[Size];

        LoraNode node;

    public:
        LoraWriter(LLora& lora)
            : lora(lora)
            , buffer()
            , node{ buffer, Size, 0 }
        {
            lora.joinTx(node);
        }
        LoraWriter(const LoraWriter& other)
            : lora(other.lora)
            , buffer()
            , node{ buffer, Size, 0 }
        {
            lora.joinTx(node);
        }

        void setMessage(const Message& message)
        {
            Udon::Pack(message, buffer);
        }

        /// @brief 送信内容を表示
        /// @param gap 区切り文字 (default: '\t')
        void show(char gap = '\t') const
        {
            if (const auto message = Udon::Unpack<Message>(buffer))
            {
                Udon::Show(*message, gap);
            }
            else
            {
                Serial.print(F("unpack failed!"));
                // ここへ到達する: setMessage()で値を設定していない
            }
        }

        /// @brief 送信バッファを表示
        /// @param gap 区切り文字 (default: ' ')
        void showRaw(char gap = ' ') const
        {
            for (auto&& it : buffer)
            {
                Serial.print(it);
                Serial.print(gap);
            }
        }
    };
}    // namespace Udon
