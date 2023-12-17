//
//    UART 受信クラス
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Sender --[UART]--> Receiver
//                       ^^^^^^^^
//

#pragma once

#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Common/Show.hpp>

namespace Udon
{

    template <typename Message>
    class UartReader
    {
        static constexpr size_t Size = Udon::SerializedSize<Message>();

        Stream& uart;

        uint8_t buffer[Size];

        uint32_t transmitMs;

    public:
        using MessageType = Message;
        
        UartReader(Stream& uart)
            : uart(uart)
            , buffer()
            , transmitMs()
        {
        }

        explicit operator bool() const
        {
            return millis() - transmitMs < 100;
        }

        void update()
        {
            if (uart.available() >= static_cast<int>(Size))
            {

                // バッファにデータを格納
                for (auto&& it : buffer)
                {
                    const auto d = uart.read();
                    if (d == -1)
                    {
                        break;
                    }
                    else
                    {
                        it = d;
                    }
                }

                // タイミングによってバッファにデータが残っている可能性があるためクリア
                while (uart.available())
                {
                    (void)uart.read();
                }

                transmitMs = millis();
            }
        }

        Udon::Optional<Message> getMessage() const
        {
            if (operator bool())
            {
                return Udon::Deserialize<Message>(buffer);
            }
            else
            {
                return Udon::nullopt;
            }
        }

        /// @brief 送信内容を表示
        void show() const
        {
            if (const auto message = getMessage())
            {
                Udon::Show(*message);
            }
            else
            {
                Udon::Show(F("unpack failed!"));
            }
        }

        /// @brief 送信内容を表示
        void showRaw() const
        {
            Udon::Show(buffer);
        }
    };

}    // namespace Udon
