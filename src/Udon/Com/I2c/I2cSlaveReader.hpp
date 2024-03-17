//
//    I2c スレーブ側受信クラス
//
//    Copyright (c) 2022-2023 udonrobo
//

//
//    master --[I2C]--> slave
//                      ^^^^^
//

#pragma once

#include "I2cBus.hpp"

#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Common/Show.hpp>

namespace Udon
{

    template <typename Message>
    class I2cSlaveReader
    {
    public:
        /// @brief 受信メッセージ型
        using MessageType = Message;

        /// @brief 受信バッファサイズ
        static constexpr size_t Size = Udon::SerializedSize<MessageType>();

        /// @brief コンストラクタ
        /// @param bus I2cバス
        I2cSlaveReader(Udon::II2cBus& bus)
            : bus(bus)
            , buffer()
        {
            self = this;
        }

        /// @brief コピーコンストラクタ
        /// @param rhs
        I2cSlaveReader(const I2cSlaveReader& rhs)
            : bus(rhs.bus)
            , buffer()
        {
            self = this;
        }

        /// @brief 受信開始
        void begin()
        {
            bus.onReceive(
                [](int)
                {
                    for (auto& buf : self->buffer)
                    {
                        buf = Wire.read();
                    }
                });
        }

        /// @brief 受信したメッセージを取得
        /// @return 受信したメッセージ
        Udon::Optional<MessageType> getMessage() const
        {
            if (bus)
            {
                return Udon::Deserialize<MessageType>(buffer);
            }
            else
            {
                return Udon::nullopt;
            }
        }

        /// @brief 受信内容を表示
        /// @param gap 区切り文字 (default: '\t')
        void show(char gap = '\t') const
        {
            if (const auto message = getMessage())
            {
                Udon::Show(*message, gap);
            }
            else
            {
                Udon::Show(F("receive failed!"));
            }
        }

        /// @brief 受信バッファを表示
        /// @param gap 区切り文字 (default: ' ')
        void showRaw(char gap = ' ') const
        {
            Udon::Show(buffer, gap);
        }

    private:
        Udon::II2cBus& bus;

        uint8_t buffer[Size];

        static I2cSlaveReader* self;
    };

    template <typename Message>
    I2cSlaveReader<Message>* I2cSlaveReader<Message>::self;

}    // namespace Udon
