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
//    I2c マスター側受信クラス
//
//    master <--[I2C]-- slave
//    ^^^^^^
//
//-------------------------------------------------------------------

#pragma once

#include <udon/com/i2c/I2cBus.hpp>
#include <udon/com/serialization/Serializer.hpp>
#include <udon/utility/Show.hpp>

namespace udon
{

    template <class Message>
    class I2cMasterReader
    {

        static constexpr size_t Size = udon::CapacityWithChecksum<Message>();

        udon::II2cBus& bus;

        uint8_t address;

        uint8_t buffer[Size];

    public:
        /// @brief コンストラクタ
        /// @param bus I2cバス
        /// @param address スレーブアドレス
        I2cMasterReader(udon::II2cBus& bus, uint8_t address)
            : bus(bus)
            , address(address)
            , buffer()
        {
        }

        /// @brief 更新
        void update()
        {
            bus.requestFrom(address, Size);
            while (bus.available())
            {
                for (auto&& it : buffer)
                {
                    it = bus.read();
                }
            }
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

        /// @brief 受信内容を表示
        /// @param gap 区切り文字 (default: '\t')
        void show(char gap = '\t') const
        {
            if (const auto message = getMessage())
            {
                udon::Show(*message, gap);
            }
            else
            {
                udon::Show(F("receive failed!"));
            }
        }

        /// @brief 受信バッファを表示
        /// @param gap 区切り文字 (default: ' ')
        void showRaw(char gap = ' ') const
        {
            udon::Show(buffer, gap);
        }
    };

}    // namespace udon
