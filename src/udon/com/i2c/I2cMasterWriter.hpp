#pragma once

#include <udon/com/i2c/I2cBus.hpp>
#include <udon/utility/Show.hpp>
#include <udon/com/serialization/Serializer.hpp>

namespace udon
{

    template <class Message>
    class I2cMasterWriter
    {

        static constexpr size_t Size = udon::CapacityWithChecksum<Message>();

        udon::II2cBus& bus;

        uint8_t address;

        uint8_t buffer[Size];

    public:

        /// @brief コンストラクタ
        /// @param bus I2cバス
        /// @param address スレーブアドレス
        I2cMasterWriter(udon::II2cBus& bus, uint8_t address)
            : bus(bus)
            , address(address)
            , buffer()
        {
        }

        /// @brief 更新
        void update()
        {
            bus.beginTransmission(address);
            bus.write(buffer, Size);
            bus.endTransmission();
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

}    // namespace udon
