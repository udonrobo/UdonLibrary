//
//    Serial 受信クラス
//
//    Copyright (c) 2022-2024 udonrobo
//
//    Sender --[Serial]--> Receiver
//                       ^^^^^^^^
//

#pragma once

#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Utility/Show.hpp>

namespace Udon
{

    /// @brief Serial 受信クラス
    /// @tparam Message メッセージ型
    template <typename Message>
    class SerialReader
    {
        static constexpr size_t Size = Udon::SerializedSize<Message>();

        Stream& serial;

        uint8_t buffer[Size];

        uint32_t transmitMs;

    public:

        /// @brief メッセージ型
        using MessageType = Message;

        /// @brief コンストラクタ
        /// @param serial Serialバス
        SerialReader(Stream& serial)
            : serial(serial)
            , buffer()
            , transmitMs()
        {
        }

        /// @brief メッセージの取得
        /// @return メッセージ
        Udon::Optional<Message> getMessage()
        {
            update();
            if (millis() - transmitMs < 100)
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
            if (const auto message = Udon::Deserialize<Message>(buffer))
            {
                Udon::Show(*message);
            }
            else
            {
                Serial.print(F("receive failed!"));
            }
        }

        /// @brief 送信内容を表示
        void showRaw() const
        {
            for (auto&& it : buffer)
            {
                Serial.print(it);
                Serial.print(' ');
            }
        }

    private:
        void update()
        {
            if (serial.available() >= static_cast<int>(Size))
            {

                // バッファにデータを格納
                for (auto&& it : buffer)
                {
                    const auto d = serial.read();
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
                while (serial.available())
                {
                    (void)serial.read();
                }

                transmitMs = millis();
            }
        }
    };

}    // namespace Udon
