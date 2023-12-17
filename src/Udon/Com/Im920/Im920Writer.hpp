//
//    IM920 送信クラス
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//

//
//    Sender --[UART]--> IM920 ~~[920MHz]~~> IM920 --[UART]--> Receiver
//    ^^^^^^
//

#pragma once

#include "IIm920.hpp"

#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Common/Show.hpp>

namespace Udon
{

    template <typename Message>
    class Im920Writer
    {
    public:
        static constexpr size_t Size = Udon::SerializedSize<Message>();

        using MessageType = Message;

    private:
        IIm920& im920;

        uint8_t buffer[Size];

        Im920Node node;

    public:
        Im920Writer(IIm920& im920)
            : im920(im920)
            , buffer()
            , node{ buffer, Size, 0 }
        {
            im920.joinTx(node);
        }
        Im920Writer(const Im920Writer& other)
            : im920(other.im920)
            , buffer()
            , node{ buffer, Size, 0 }
        {
            im920.joinTx(node);
        }

        void setMessage(const Message& message)
        {
            Udon::Serialize(message, buffer);
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
