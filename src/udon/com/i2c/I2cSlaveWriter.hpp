///   @file   I2cSlaveReader.hpp
///   @brief  I2cスレーブ送信用クラス
///   @author 大河 祐介
///   @date   2023/03/31

#pragma once

#include <udon/com/i2c/I2cBus.hpp>
#include <udon/com/serializer/Serializer.hpp>

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
        /// @param address I2cアドレス
        /// @param clock   通信レート
        template <typename Bus>
        I2cSlaveWriter(Bus& bus)
            : bus(bus)
            , buffer()
        {
            self = this;
            bus.onRequest([]
                          { self->bus.write(self->buffer, Size); });
        }

        void setMessage(const Message& message)
        {
            udon::Pack(message, buffer);
        }

        /// @brief 送信内容を表示
        /// @param end   オプション [/n, /t ..]
        /// @param radix 基数      [BIN, HEX ..]
        void show(const char end = {}, const uint16_t radix = DEC) const noexcept
        {
            for (const auto& buffer : buffer)
                Serial.print(buffer, radix), Serial.print('\t');
            Serial.print(end);
        }
    };

    template <class Message>
    I2cSlaveWriter<Message>* I2cSlaveWriter<Message>::self;

}    // namespace udon
