///   @file   I2cSlaveReader.hpp
///   @brief  I2cスレーブ送信用クラス
///   @author 大河 祐介
///   @date   2023/03/31

#pragma once

#include <udon/com/i2c/I2cBus.hpp>
#include <udon/com/serialization/Serializer.hpp>
#include <udon/utility/Show.hpp>

namespace udon
{

    template <class Message>
    class I2cSlaveWriter
    {
        static constexpr size_t Size = udon::CapacityWithChecksum<Message>();

        udon::II2cBus& bus;

        uint8_t buffer[Size];

        static I2cSlaveWriter* self;

    public:
        /// @brief コンストラクタ
        /// @param bus I2cバス
        I2cSlaveWriter(udon::II2cBus& bus)
            : bus(bus)
            , buffer()
        {
            self = this;
        }

        /// @brief 送信開始
        void begin()
        {
            const auto onRequest = []()
            {
                Serial.println("onRequest");
                self->bus.write(self->buffer, Size);
            };
            bus.onRequest(onRequest);
        }

        /// @brief 送信するメッセージを設定
        /// @param message 送信するメッセージ
        void setMessage(const Message& message)
        {
            udon::Pack(message, buffer);
        }

        /// @brief 送信バッファの参照を取得
        /// @return 送信バッファの参照
        uint8_t (&data())[Size]
        {
            return buffer;
        }

        /// @brief 送信内容を表示
        /// @param gap 区切り文字 (default: "\t")
        void show(const char* gap = "\t") const
        {
            if (const auto message = udon::Unpack<Message>(buffer))
            {
                udon::Show(*message, gap);
            }
            else
            {
                Serial.print(F("receive error!"));
            }
        }

        /// @brief 送信バッファを表示
        /// @param gap 区切り文字 (default: " ")
        void showRaw(const char* gap = " ") const
        {
            for (auto&& it : buffer)
            {
                Serial.print(it);
                Serial.print(gap);
            }
        }
    };

    template <class Message>
    I2cSlaveWriter<Message>* I2cSlaveWriter<Message>::self;

}    // namespace udon
