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
//    CAN通信 受信クラス
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Com/Can/ICanBus.hpp>
#include <Udon/Com/Can/CanNode.hpp>

#include <Udon/Com/Serialization.hpp>
#include <Udon/Utility/Show.hpp>

namespace Udon
{

    template <typename Message>
    class CanReader
    {
    public:
        /// @brief 受信メッセージ型
        using MessageType = Message;

        /// @brief 受信バッファサイズ
        static constexpr size_t Size = Udon::CapacityWithChecksum<Message>();

        /// @brief コンストラクタ
        /// @param bus I2cバス
        /// @param id 信号識別ID
        CanReader(ICanBus& bus, const uint32_t id)
            : bus{ bus }
            , node{ id, buffer, sizeof buffer, 0 }
        {
            joinBus();
        }

        /// @brief コピーコンストラクタ
        CanReader(const CanReader& other)
            : bus{ other.bus }
            , node{ other.node }
        {
            joinBus();
        }

        /// @brief デストラクタ
        ~CanReader()
        {
            bus.leaveRx(node);
        }

        /// @brief 受信しているか
        /// @return 受信していればtrue
        explicit operator bool() const
        {
            return millis() - node.transmitMs < 100;
        }

        /// @brief メッセージ構造体を取得
        /// @return メッセージ構造体(Optional)
        Udon::Optional<MessageType> getMessage() const
        {
            if (*this)
            {
                return message;
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
            for (size_t i = 0; i < node.length; ++i)
            {
                Udon::Show(node.data[i], gap);
            }
        }

    private:
        void joinBus()
        {
            bus.joinRx(
                node,
                [](void* p)
                {
                    auto self     = static_cast<CanReader*>(p);
                    self->message = Udon::Unpack<MessageType>(self->node.data, self->node.length);
                },
                this);
        }

        ICanBus& bus;

        uint8_t buffer[Size];

        CanNode node;

        Udon::Optional<MessageType> message;
    };

}    // namespace Udon