//
//    LoRa 送信クラス
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include "ILoRa.hpp"

#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Common/Show.hpp>

namespace Udon
{

    template <typename Message>
    class LoRaWriter
    {
    public:
        static constexpr size_t Size = Udon::SerializedSize<Message>();

        using MessageType = Message;

    private:
        ILoRa& lora;

        uint8_t buffer[Size];

        LoRaNode node;

    public:
        LoRaWriter(ILoRa& lora)
            : lora(lora)
            , buffer()
            , node{ buffer, Size, 0 }
        {
            lora.joinTx(node);
        }
        LoRaWriter(const LoRaWriter& other)
            : lora(other.lora)
            , buffer()
            , node{ buffer, Size, 0 }
        {
            lora.joinTx(node);
        }

        void setMessage(const Message& message)
        {
            Udon::Serialize(message, buffer);
        }

        void setErrorMessage() noexcept
        {
            Udon::FailableSerialize({ buffer });
        }

        /// @brief 送信内容を表示
        /// @param gap 区切り文字 (default: '\t')
        void show() const
        {
            if (const auto message = Udon::Deserialize<Message>(buffer))
            {
                Udon::ShowRaw(*message);
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
