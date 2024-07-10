//
//    UART 送信クラス
//
//    Copyright (c) 2022-2024 udonrobo
//
//    Sender --[UART]--> Receiver
//    ^^^^^^
//

#pragma once

#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Utility/Show.hpp>

namespace Udon
{

    /// @brief UART 送信クラス
    /// @tparam Message メッセージ型
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
        void show() const
        {
            Udon::Show(message);
        }

        /// @brief 生の送信内容を表示
        void showRaw() const
        {
            for (auto&& it : Udon::Serialize(message))
            {
                Serial.print(it);
                Serial.print(' ');
            }
        }
    };

}    // namespace Udon
