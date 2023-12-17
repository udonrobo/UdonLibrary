//
//    UART 送信クラス
//
//    Copyright (c) 2022-2023 Okawa Yusuke
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

        /// @brief 更新
        void update()
        {
            for (auto&& it : Udon::Serialize(message))
            {
                uart.write(it);
            }
        }

        /// @brief 送信内容を更新
        /// @param rhs 送信内容
        void setMessage(const Message& rhs)
        {
            message = rhs;
        }

        /// @brief 送信内容を表示
        void show() const
        {
            Udon::Show(message);
        }

        /// @brief 生の送信内容を表示
        void showRaw() const
        {
            uint8_t buffer[Udon::SerializedSize<Message>()];
            Udon::Serialize(message, buffer);
            Udon::Show(buffer);
        }
    };

}    // namespace Udon
