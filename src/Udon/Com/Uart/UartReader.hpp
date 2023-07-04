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

 #include <Udon/Com/Serialization.hpp>
#include <Udon/Utility/Show.hpp>

namespace Udon
{

    template <class Message>
    class UartReader
    {
        static constexpr size_t Size = Udon::CapacityWithChecksum<Message>();

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

        Udon::Optional<Message> getMessage() const
        {
            if (operator bool())
            {
                return Udon::Unpack<Message>(buffer);
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
            if (const auto message = getMessage())
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
    };

}    // namespace Udon
