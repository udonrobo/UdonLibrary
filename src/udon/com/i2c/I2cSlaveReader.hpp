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
    class I2cSlaveReader
    {
        static constexpr size_t Size = udon::CapacityWithChecksum<Message>();

        udon::II2cBus& bus;

        uint8_t buffer[Size];

        static I2cSlaveReader* self;

    public:
        /// @brief コンストラクタ
        /// @param bus I2cバス
        I2cSlaveReader(udon::II2cBus& bus)
            : bus(bus)
            , buffer()
        {
            self = this;
        }

        /// @brief 受信開始
        void begin()
        {
            const auto onReceive = [](int)
            {
                for (auto& buf : self->buffer)
                {
                    buf = Wire.read();
                }
            };
            bus.onReceive(onReceive);
        }

        /// @brief 受信したメッセージを取得
        /// @return 受信したメッセージ
        udon::optional<Message> getMessage() const
        {
            if (bus)
            {
                return udon::Unpack<Message>(buffer);
            }
            else
            {
                return udon::nullopt;
            }
        }

        /// @brief 受信バッファの参照を取得
        /// @return 受信バッファの参照
        const uint8_t (&data() const)[Size]
        {
            return buffer;
        }

        /// @brief 受信内容を表示
        /// @param gap 区切り文字 (default: '\t')
        void show(char gap = '\t') const
        {
            if (const auto message = getMessage())
            {
                udon::Show(*message);
            }
            else
            {
                Serial.print(F("receive failed!"));
            }
        }

        /// @brief 受信バッファを表示
        /// @param gap 区切り文字 (default: ' ')
        void showRaw(char gap = ' ') const
        {
            for (auto&& it : buffer)
            {
                Serial.print(buffer);
                Serial.print(gap);
            }
        }
    };

    template <class Message>
    I2cSlaveReader<Message>* I2cSlaveReader<Message>::self;

}    // namespace udon
