//
//    Lora 受信クラス
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include "ILora.hpp"

#include <Udon/Serializer/Serializer.hpp>
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
        void show() const
        {
            if (const auto message = getMessage())
            {
                Udon::Show(*message);
            }
            else
            {
                Udon::Show(F("unpack failed!"));
            }
        }

        /// @brief 受信バッファを表示
        void showRaw() const
        {
            Udon::Show(buffer);
        }
    };
}    // namespace Udon
