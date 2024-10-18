//
//    I2c スレーブ側送信クラス
//
//    Copyright (c) 2022-2024 udonrobo
//

//
//    master <--[I2C]-- slave
//                      ^^^^^
//

#pragma once

#include "I2cBus.hpp"

#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Utility/Show.hpp>

namespace Udon
{

    /// @brief I2c スレーブ側送信クラス
    /// @tparam Message 送信メッセージ型
    template <typename Message>
    class I2cSlaveWriter
    {
    public:
        /// @brief 受信メッセージ型
        using MessageType = Message;

        /// @brief 受信バッファサイズ
        static constexpr size_t Size = Udon::SerializedSize<MessageType>();

        /// @brief コンストラクタ
        /// @param bus I2cバス
        I2cSlaveWriter(Udon::II2cBus& bus)
            : bus(bus)
            , buffer()
        {
            self = this;
        }

        /// @brief コピーコンストラクタ
        I2cSlaveWriter(const I2cSlaveWriter&) = delete;
        
        /// @brief ムーブコンストラクタ
        I2cSlaveWriter(I2cSlaveWriter&& other)
            : bus(other.bus)
            , buffer()
        {
            self = this;
        }

        /// @brief 送信開始
        void begin()
        {
            bus.onRequest(
                []()
                {
                    self->bus.write(self->buffer, Size);
                });
        }

        /// @brief 送信するメッセージを設定
        /// @param message 送信するメッセージ
        void setMessage(const MessageType& message)
        {
            Udon::Serialize(message, buffer);
        }

        /// @brief 送信内容を表示
        void show() const
        {
            if (const auto message = Udon::Deserialize<MessageType>(buffer))
            {
                Udon::Show(*message);
            }
            else
            {
                Udon::Show(F("unpack failed!"));    // ここへ到達する: setMessage()で値を設定していない
            }
        }

        /// @brief 送信バッファを表示
        void showRaw() const
        {
            Udon::Show(buffer);
        }

    private:
        Udon::II2cBus& bus;

        uint8_t buffer[Size];

        static I2cSlaveWriter* self;
    };

    template <typename Message>
    I2cSlaveWriter<Message>* I2cSlaveWriter<Message>::self;

}    // namespace Udon
