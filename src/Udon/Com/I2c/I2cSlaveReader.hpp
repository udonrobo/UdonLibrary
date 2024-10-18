//
//    I2c スレーブ側受信クラス
//
//    Copyright (c) 2022-2024 udonrobo
//

//
//    master --[I2C]--> slave
//                      ^^^^^
//

#pragma once

#include "I2cBus.hpp"

#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Utility/Show.hpp>

namespace Udon
{

    /// @brief I2c スレーブ側受信クラス
    /// @tparam Message 受信メッセージ型
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
        /// @note この関数は明示的に削除されています。
        I2cSlaveReader(const I2cSlaveReader&) = delete;

        
        /// @brief ムーブコンストラクタ
        I2cSlaveReader(I2cSlaveReader&& other)
            : bus(other.bus)
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
        void show() const
        {
            if (const auto message = getMessage())
            {
                Udon::Show(*message);
            }
            else
            {
                Udon::Show(F("receive failed!"));
            }
        }

        /// @brief 受信バッファを表示
        void showRaw() const
        {
            Udon::Show(buffer);
        }

    private:
        Udon::II2cBus& bus;

        uint8_t buffer[Size];

        static I2cSlaveReader* self;
    };

    template <typename Message>
    I2cSlaveReader<Message>* I2cSlaveReader<Message>::self;

}    // namespace Udon
