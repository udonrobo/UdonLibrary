//-------------------------------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    I2c スレーブ側送信クラス
//
//    master <--[I2C]-- slave
//                      ^^^^^
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Com/I2c/I2cBus.hpp>
#include <Udon/Com/Serialization.hpp>
#include <Udon/Utility/Show.hpp>

namespace Udon
{

    template <class Message>
    class I2cSlaveWriter
    {
        static constexpr size_t Size = Udon::CapacityWithChecksum<Message>();

        Udon::II2cBus& bus;

        uint8_t buffer[Size];

        static I2cSlaveWriter* self;

    public:
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
        void setMessage(const Message& message)
        {
            Udon::Pack(message, buffer);
        }

        /// @brief 送信内容を表示
        /// @param gap 区切り文字 (default: '\t')
        void show(char gap = '\t') const
        {
            if (const auto message = Udon::Unpack<Message>(buffer))
            {
                Udon::Show(*message, gap);
            }
            else
            {
                Udon::Show(F("unpack failed!"));
                // ここへ到達する: setMessage()で値を設定していない
            }
        }

        /// @brief 送信バッファを表示
        /// @param gap 区切り文字 (default: ' ')
        void showRaw(char gap = ' ') const
        {
            Udon::Show(buffer, gap);
        }
    };

    template <class Message>
    I2cSlaveWriter<Message>* I2cSlaveWriter<Message>::self;

}    // namespace Udon
