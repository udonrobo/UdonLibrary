//
//    I2c スレーブ側送信クラス
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//

//
//    master <--[I2C]-- slave
//                      ^^^^^
//

#pragma once

#include "I2cBus.hpp"

#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Common/Show.hpp>

namespace Udon
{

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
        /// @param rhs
        I2cSlaveWriter(const I2cSlaveWriter& rhs)
            : bus(rhs.bus)
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
        /// @param gap 区切り文字 (default: '\t')
        void show(char gap = '\t') const
        {
            if (const auto message = Udon::Deserialize<MessageType>(buffer))
            {
                Udon::Show(*message, gap);
            }
            else
            {
                Udon::Show(F("unpack failed!"));    // ここへ到達する: setMessage()で値を設定していない
            }
        }

        /// @brief 送信バッファを表示
        /// @param gap 区切り文字 (default: ' ')
        void showRaw(char gap = ' ') const
        {
            Udon::Show(buffer, gap);
        }

    private:
        Udon::II2cBus& bus;

        uint8_t buffer[Size];

        static I2cSlaveWriter* self;
    };

    template <typename Message>
    I2cSlaveWriter<Message>* I2cSlaveWriter<Message>::self;

}    // namespace Udon
