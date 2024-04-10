//
//    UART 送信クラス
//
//    Copyright (c) 2022-2023 udonrobo
//
//    Sender --[UART]--> Receiver
//    ^^^^^^
//

#pragma once

#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Common/Show.hpp>

namespace Udon
{

    template <typename Message>
    class UartWriter
    {

        Stream& uart;

        Message message;

    public:
        /// @brief コンストラクタ
        UartWriter(Stream& uart) noexcept
            : uart(uart)
            , message()
        {
        }

        /// @brief 送信内容を更新
        /// @param rhs 送信内容
        void setMessage(const Message& rhs)
        {
            message = rhs;
            for (auto&& it : Udon::Serialize(message))
            {
                uart.write(it);
            }
        }

        /// @brief 送信内容を表示
        /// @param gap 区切り文字 (default: '\t')
        void show(char gap = '\t') const
        {
            Udon::Show(message, gap);
        }

        /// @brief 生の送信内容を表示
        /// @param gap 区切り文字 (default: ' ')
        void showRaw(char gap = ' ') const
        {
            for (auto&& it : Udon::Serialize(message))
            {
                Serial.print(it);
                Serial.print(gap);
            }
        }
    };

}    // namespace Udon
