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
//    UART 受信クラス
//
//    Sender --[UART]--> Receiver
//                       ^^^^^^^^
//
//-------------------------------------------------------------------


#pragma once

#include <udon/com/serialization/Serializer.hpp>
#include <udon/utility/Show.hpp>

namespace udon
{

    template <class Message>
    class UartReader
    {
        static constexpr size_t Size = udon::CapacityWithChecksum<Message>();

        Stream& uart;

        uint8_t buffer[Size];

        uint32_t transmitMs;

    public:
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
            if (uart.available() >= Size)
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

        udon::optional<Message> getMessage() const
        {
            if (operator bool())
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
    };

}    // namespace udon
