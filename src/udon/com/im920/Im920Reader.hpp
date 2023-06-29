//-------------------------------------------------------------------
//
//	  UdonLibrary
// 
//                  2018-2023 Watanabe Rui
//	  Copyright (c) 2022-2023 Okawa Yusuke
//	  Copyright (c) 2022-2023 udonrobo
//
//	  Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    IM920 受信クラス
//
//    Sender --[UART]--> IM920 ~~[920MHz]~~> IM920 --[UART]--> Receiver
//                                                             ^^^^^^^^
//
//-------------------------------------------------------------------


#pragma once

#include <udon/com/im920/IIm920.hpp>
#include <udon/com/serialization/Serializer.hpp>

namespace udon
{

    template <typename Message>
    class Im920Reader
    {
        static constexpr size_t Size = udon::CapacityWithChecksum<Message>();

        static_assert(Size <= 64, "The send buffer size for IM920 is limited to 64 bytes");

        IIm920& im920;

        const std::vector<uint8_t>& buffer;

    public:
        Im920Reader(IIm920& im920)
            : im920(im920)
            , buffer(im920.registerReceiver(Size))
        {
        }

        udon::optional<Message> getMessage() const
        {
            if (im920)
            {
                return udon::Unpack<Message>(buffer);
            }
            else
            {
                return udon::nullopt;
            }
        }

        /// @brief 送信内容を表示
        /// @param gap 区切り文字 (default: '\t')
        void show(char gap = '\t') const
        {
            if (const auto message = getMessage())
            {
                udon::Show(*message, gap);
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
}    // namespace udon
