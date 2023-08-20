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
//    I2c スレーブ側受信クラス
//  
//    master --[I2C]--> slave
//                      ^^^^^
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Com/I2c/I2cBus.hpp>
#include <Udon/Com/Serialization.hpp>
#include <Udon/Utility/Show.hpp>

namespace Udon
{

    template <typename Message>
    class I2cSlaveReader
    {
        static constexpr size_t Size = Udon::CapacityWithChecksum<Message>();

        Udon::II2cBus& bus;

        uint8_t buffer[Size];

        static I2cSlaveReader* self;

    public:
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
        Udon::Optional<Message> getMessage() const
        {
            if (bus)
            {
                return Udon::Unpack<Message>(buffer);
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
    };

    template <typename Message>
    I2cSlaveReader<Message>* I2cSlaveReader<Message>::self;

}    // namespace Udon
