//-----------------------------------------------
//
//  UdonLibrary
//
//  Copyright (c) 2022-2023 Okawa Yusuke
//  Copyright (c) 2022-2023 udonrobo
//
//  Licensed under the MIT License.
//
//-----------------------------------------------
//
//  I2c スレーブ側送信クラス
//
//-----------------------------------------------

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
            bus.onRequest(
                []()
                {
                    self->bus.write(self->buffer, Size);
                });
        }

        /// @brief 送信するメッセージを設定
        /// @param message 送信するメッセージ
        void setMessage(const Message& message)
        {
            udon::Pack(message, buffer);
        }

        /// @brief 送信内容を表示
        /// @param gap 区切り文字 (default: '\t')
        void show(char gap = '\t') const
        {
            if (const auto message = udon::Unpack<Message>(buffer))
            {
                udon::Show(*message, gap);
            }
            else
            {
                udon::Show(F("unpack failed!"));    // 通常はここには到達しない
            }
        }

        /// @brief 送信バッファを表示
        /// @param gap 区切り文字 (default: ' ')
        void showRaw(char gap = ' ') const
        {
            udon::Show(buffer, gap);
        }
    };

    template <class Message>
    I2cSlaveWriter<Message>* I2cSlaveWriter<Message>::self;

}    // namespace udon
