//
//    UART 受信クラス
//
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

    public:
        using MessageType = Message;

        UartReader(Stream& uart)
            : uart(uart)
            , buffer()
            , transmitMs()
        {
        }

        Udon::Optional<Message> getMessage()
        {
            update();
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
        /// @param gap 区切り文字 (default: '\t')
        void show(char gap = '\t') const
        {
            if (const auto message = Udon::Deserialize<Message>(buffer))
            {
                Udon::Show(*message, gap);
            }
            else
            {
                Serial.print(F("receive failed!"));
            }
        }

        /// @brief 送信内容を表示
        /// @param gap 区切り文字 (default: ' ')
        void showRaw(char gap = ' ') const
        {
            for (auto&& it : buffer)
            {
                Serial.print(it);
                Serial.print(gap);
            }
        }

    private:
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
            }
        }
    };

}    // namespace Udon
