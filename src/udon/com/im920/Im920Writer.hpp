//-----------------------------------------------
//
//	UdonLibrary
// 
//                2018-2023 Watanabe Rui
//	Copyright (c) 2022-2023 Okawa Yusuke
//	Copyright (c) 2022-2023 udonrobo
//
//	Licensed under the MIT License.
//
//-----------------------------------------------
//
//  IM920 送信クラス
//
//-----------------------------------------------


#pragma once

#include <udon/com/im920/IIm920.hpp>
#include <udon/com/serialization/Serializer.hpp>

namespace udon
{

    template <typename Message>
    class Im920Writer
    {
        static constexpr size_t Size = udon::CapacityWithChecksum<Message>();

        static_assert(Size <= 64, "The send buffer size for IM920 is limited to 64 bytes");

        IIm920& im920;

        std::vector<uint8_t>& buffer;

    public:
        Im920Writer(IIm920& im920)
            : im920(im920)
            , buffer(im920.registerSender(Size))
        {
        }

        void setMessage(const Message& message)
        {
            buffer = udon::Pack(message);
        }

        /// @brief 送信内容を表示
        /// @param gap 区切り文字 (default: '\t')
        void show(char gap = '\t') const
        {
            if (const auto message = udon::Unpack<Message>(buffer))
            {
                udon::Show(*message, gap);
            }
            else
            {
                Serial.print(F("unpack failed!"));    // 通常来ない
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
