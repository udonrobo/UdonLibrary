///   @file   I2cSlaveReader.hpp
///   @brief  I2cスレーブ送信用クラス
///   @author 大河 祐介
///   @date   2023/03/31

#pragma once

#include <udon/com/i2c/I2cBus.hpp>
#include <udon/utility/Show.hpp>
#include <udon/com/serialization/Serializer.hpp>

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
                          { Serial.print("call"); self->bus.write(self->buffer, Size); });
        }

        void setMessage(const Message& message)
        {
            udon::Pack(message, buffer);
        }

        size_t size() const
        {
            return Size;
        }

        uint8_t* data()
        {
            return buffer;
        }

        /// @brief 送信内容を表示
        /// @param gap 区切り文字 (default: "\t")
        void show(const char* gap = "\t") const
        {
            if (const auto message = udon::Unpack<Message>(buffer))
            {
                udon::Show(*message);
            }
            else
            {
                Serial.print(F("receive error!"));
            }
        }

        void showRaw(const char* gap = "\t") const
        {
            for (const auto& buffer : buffer)
                Serial.print(buffer), Serial.print(' ');
        }
    };

    template <class Message>
    I2cSlaveWriter<Message>* I2cSlaveWriter<Message>::self;

}    // namespace udon
