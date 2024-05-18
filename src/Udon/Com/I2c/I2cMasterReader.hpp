//
//    I2c マスター側受信クラス
//
//    Copyright (c) 2022-2024 udonrobo
//

//
//    master <--[I2C]-- slave
//    ^^^^^^
//

#pragma once

#include "I2cBus.hpp"

#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Common/Show.hpp>

namespace Udon
{

    /// @brief I2c マスター側受信クラス
    /// @tparam Message 受信メッセージ型
    template <typename Message>
    class I2cMasterReader
    {
    public:
        /// @brief 受信メッセージ型
        using MessageType = Message;

        /// @brief 受信バッファサイズ
        static constexpr size_t Size = Udon::SerializedSize<Message>();

        /// @brief コンストラクタ
        /// @param bus I2cバス
        /// @param address スレーブアドレス
        I2cMasterReader(Udon::II2cBus& bus, uint8_t address)
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
        Udon::Optional<Message> getMessage() const
        {
            if (bus)
            {
                return Udon::Deserialize<Message>(buffer);
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

        uint8_t address;

        uint8_t buffer[Size];
    };

}    // namespace Udon
