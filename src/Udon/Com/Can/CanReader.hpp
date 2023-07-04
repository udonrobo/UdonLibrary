/// @file   CanReader.hpp
/// @date   2023/01/15
/// @brief  CAN 通信受信クラス
/// @author 大河 祐介

#pragma once

#ifdef ARDUINO

#    include <Udon/Com/Can/ICanBus.hpp>
#    include <Udon/Com/Serialization.hpp>
#    include <Udon/Utility/Show.hpp>

namespace Udon
{

    template <class Message>
    class CanReader
    {

        ICanBus& bus;

        CanNodeView node;

    public:
        /// @brief コンストラクタ
        /// @param bus I2cバス
        /// @param id 信号識別ID
        CanReader(ICanBus& bus, const uint32_t id)
            : bus{ bus }
            , node{ bus.createRxNode(id, Udon::CapacityWithChecksum<Message>()) }
        {
        }

        /// @brief 受信しているか
        /// @return 受信していればtrue
        explicit operator bool() const
        {
            return millis() - *node.transmitMs < 100;
        }

        /// @brief メッセージ構造体を取得
        /// @return メッセージ構造体(Optional)
        Udon::Optional<Message> getMessage() const
        {
            return Udon::Unpack<Message>(*node.data);
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
            for (auto&& it : *node.data)
            {
                Udon::Show(it, gap);
            }
        }
    };

}    // namespace Udon

#endif