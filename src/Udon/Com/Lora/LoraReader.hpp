
#pragma once

#include "ILora.hpp"

#include <Udon/Com/Serialization.hpp>
#include <Udon/Common/Show.hpp>

namespace Udon
{

    template <typename Message>
    class LoraReader
    {
    public:
        static constexpr size_t Size = Udon::SerializedSize<Message>();

        using MessageType = Message;

    private:
        ILora& lora;

        uint8_t buffer[Size];

        LoraNode node;

    public:
        LoraReader(ILora& lora)
            : lora(lora)
            , buffer()
            , node{ buffer, Size, 0 }
        {
            lora.joinRx(node);
        }
        LoraReader(const LoraReader& other)
            : lora(other.lora)
            , buffer()
            , node{ buffer, Size, 0 }
        {
            lora.joinRx(node);
        }

        Udon::Optional<Message> getMessage(uint32_t timeOut = 700) const
        {
            if (millis() - node.transmitMs > timeOut)
            {
                return Udon::nullopt;
            }
            else
            {
                return Udon::Deserialize<Message>(buffer);
            }
        }

        /// @brief 受信内容を表示
        /// @param gap 区切り文字 (default: '\t')
        void show(char gap = '\t') const
        {
            if (const auto message = getMessage())
            {
                Udon::Show(*message, gap);
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
            for (auto&& it : buffer)
            {
                Serial.print(it);
                Serial.print(gap);
            }
        }
    };
}    // namespace Udon
