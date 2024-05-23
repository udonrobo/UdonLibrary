//
//    IM920 受信クラス
//
//    Copyright (c) 2022-2024 udonrobo
//

//
//    Sender --[UART]--> IM920 ~~[920MHz]~~> IM920 --[UART]--> Receiver
//                                                         　  ^^^^^^^^
//

#pragma once

#include "IIm920.hpp"

#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Common/Show.hpp>

namespace Udon
{

    /// @brief IM920受信クラス
    /// @tparam Message 受信するメッセージ型
    template <typename Message>
    class Im920Reader
    {
    public:
        static constexpr size_t Size = Udon::SerializedSize<Message>();

        using MessageType = Message;

    private:
        IIm920& im920;

        uint8_t buffer[Size];

        Im920Node node;

    public:
        Im920Reader(IIm920& im920)
            : im920(im920)
            , buffer()
            , node{ buffer, Size, 0 }
        {
            im920.joinRx(node);
        }
        Im920Reader(const Im920Reader& other)
            : im920(other.im920)
            , buffer()
            , node{ buffer, Size, 0 }
        {
            im920.joinRx(node);
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

        /// @brief 送信内容を表示
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
