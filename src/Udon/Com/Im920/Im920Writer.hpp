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
//    IM920 送信クラス
//
//    Sender --[UART]--> IM920 ~~[920MHz]~~> IM920 --[UART]--> Receiver
//    ^^^^^^
//
//-------------------------------------------------------------------

#pragma once

#include "IIm920.hpp"

#include <Udon/Com/Serialization.hpp>
#include <Udon/Utility/Show.hpp>

namespace Udon
{

    template <typename Message>
    class Im920Writer
    {
    public:
        static constexpr size_t Size = Udon::CapacityWithChecksum<Message>();

        using MessageType = Message;

    private:
        static_assert(Size <= 64, "The send buffer size for IM920 is limited to 64 bytes");

        IIm920& im920;

        std::vector<uint8_t>& buffer;

    public:
        Im920Writer(IIm920& im920)
            : im920(im920)
            , buffer(im920.registerSender(Size))
        {
        }

        Im920Writer(const Im920Writer&)
            : im920(im920)
            , buffer(im920.registerSender(Size))
        {
        }

        void setMessage(const Message& message)
        {
            buffer = Udon::Pack(message);
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
