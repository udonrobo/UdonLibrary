//
//    Lora 送信クラス
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
    class LoraWriter
    {
    public:
        static constexpr size_t Size = Udon::SerializedSize<Message>();

        using MessageType = Message;

    private:
        ILora& lora;

        uint8_t buffer[Size];

        LoraNode node;

    public:
        LoraWriter(ILora& lora)
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
            Udon::Serialize(message, buffer);
        }

        void setErrorMessage() noexcept
        {
            Udon::FailableSerialize({ buffer });
        }

        /// @brief 送信内容を表示
        void show() const
        {
            if (const auto message = Udon::Deserialize<Message>(buffer))
            {
                Udon::Show(*message);
            }
            else
            {
                Udon::Show(F("unpack failed!"));
                // ここへ到達する: setMessage()で値を設定していない
            }
        }

        /// @brief 送信バッファを表示
        void showRaw() const
        {
            Udon::Show(buffer);
        }
    };
}    // namespace Udon
