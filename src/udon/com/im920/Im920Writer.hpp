// /*
//     @program IM920モジュールを用いてArduino間の無線通信を行うライブラリ(送信用)
//     @date 2018/10/25
//     @author Watanabe Rui
// */

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

        /// @brief 送信バッファの参照を取得
        /// @return 送信バッファの参照
        std::vector<uint8_t>& data()
        {
            return buffer;
        }

        /// @brief 送信内容を表示
        /// @param gap 区切り文字 (default: '\t')
        void show(char gap = '\t') const
        {
            if (const auto message = udon::Unpack<Message>(buffer))
            {
                udon::Show(*message);
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
