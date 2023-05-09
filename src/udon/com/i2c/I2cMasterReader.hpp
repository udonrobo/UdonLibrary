#pragma once

#include <udon/com/i2c/I2cBus.hpp>
#include <udon/com/serializer/Serializer.hpp>

namespace udon
{

    template <class Message>
    class I2cMasterReader
    {

        /// @brief I2c バス
        udon::II2cBus& bus;

        /// @brief スレーブのアドレス
        uint8_t address;

        /// @brief メッセージ
        Message message;

        /// @brief エラーカウント
        uint32_t errorCount;

    public:
        I2cMasterReader(udon::II2cBus& bus, uint8_t address)
            : bus(bus)
            , address(address)
            , message()
            , errorCount()
        {
        }

        /// @brief 更新
        void update()
        {
            // メッセージのサイズを取得
            constexpr size_t size = udon::CapacityWithChecksum<Message>();

            // スレーブにリクエストを送信
            bus.requestFrom(address, size);

            uint8_t buffer[size];
            while (bus.available())
            {
                // バッファにデータを読み込む
                for (auto&& it : buffer)
                {
                    const auto d = bus.read();
                    if (d == -1)
                    {
                        errorCount += 5;
                    }
                    else
                    {
                        --errorCount;
                        it = d;
                    }
                }

                // エラーが多すぎる場合はバスをリセット
                if (errorCount > 255)
                {
                    bus.restart();
                    errorCount = 0;
                }
            }
            if (const auto unpacked = udon::Unpack<Message>(buffer))
            {
                message = *unpacked;
            }
        }

        /// @brief メッセージを取得
        /// @return メッセージ
        Message getMessage() const
        {
            return message;
        }

        void show() const
        {
            getMessage().show();
        }
    };

}    // namespace udon
