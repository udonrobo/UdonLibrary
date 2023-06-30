/// @file   CanWriter.hpp
/// @date   2023/01/15
/// @brief  CAN 通信送信クラス
/// @author 大河 祐介

#pragma once

#include <Udon/Com/Can/ICanBus.hpp>
#include <Udon/Com/Serialization/Serializer.hpp>
#include <Udon/Utility/Show.hpp>

namespace Udon
{

    template <class Message>
    class CanWriter
    {

        ICanBus& bus;

        CanNodeView node;

    public:
        /// @param id 信号識別ID
        CanWriter(ICanBus& bus, const uint32_t id)
            : bus{ bus }
            , node{ bus.createTxNode(id, Udon::CapacityWithChecksum<Message>()) }
        {
        }

        /// @brief メッセージ構造体をセット
        void setMessage(const Message& message) noexcept
        {
            *node.data = Udon::Pack(message);
        }

        /// @brief 送信内容を表示
        /// @param gap 区切り文字 (default: '\t')
        void show(char gap = '\t') const
        {
            if (const auto message = Udon::Unpack<Message>(*node.data))
            {
                Udon::Show(*message, gap);
            }
            else
            {
                Udon::Show(F("unpack failed!"));    // 通常はここには到達しない
            }
        }

        /// @brief 送信バッファを表示
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