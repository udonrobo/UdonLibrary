//
//    IM920 受信クラス
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
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
        void show() const
        {
            if (const auto message = getMessage())
            {
                Udon::Show(*message);
            }
            else
            {
                Udon::Show(F("receive failed!"));
            }
        }

        /// @brief 送信バッファを表示
        void showRaw() const
        {
            Udon::Show(buffer);
        }
    };
}    // namespace Udon
