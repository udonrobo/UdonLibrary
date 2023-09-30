//-------------------------------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    IM920 受信クラス
//
//    Sender --[UART]--> IM920 ~~[920MHz]~~> IM920 --[UART]--> Receiver
//                                                         　  ^^^^^^^^
//
//-------------------------------------------------------------------

#pragma once

#include "IIm920.hpp"

#include <Udon/Com/Serialization.hpp>
#include <Udon/Common/Show.hpp>

namespace Udon
{

    template <typename Message>
    class Im920Reader
    {
    public:
        static constexpr size_t Size = Udon::CapacityWithChecksum<Message>();

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
                return Udon::Unpack<Message>(buffer);
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
