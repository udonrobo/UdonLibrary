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
#include <Udon/Utility/Show.hpp>

namespace Udon
{

    template <typename Message>
    class Im920Reader
    {
    public:
        static constexpr size_t Size = Udon::CapacityWithChecksum<Message>();

        using MessageType = Message;

    private:
        static_assert(Size <= 64, "The send buffer size for IM920 is limited to 64 bytes");

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
        ~Im920Reader()
        {
            im920.leaveRx();
        }

        Udon::Optional<Message> getMessage() const
        {
            if (im920)
            {
                return Udon::Unpack<Message>(buffer);
            }
            else
            {
                return Udon::nullopt;
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
