//
//    CAN通信 送信クラス
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#include "ICanBus.hpp"

#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Utility/Show.hpp>
#include <Udon/Serializer/SerializerTraits.hpp>
#include <Udon/Utility/Printf.hpp>

namespace Udon
{

    /// @brief CAN通信 送信クラス
    /// @tparam Message 受信メッセージ型
    template <typename Message>
    class CanWriter
    {

        static_assert(Udon::Traits::IsSerializable<Message>::value, "Message must be parsable.");

    public:
        /// @brief 受信メッセージ型
        using MessageType = Message;

        /// @brief 受信バッファサイズ
        static constexpr size_t Size = Udon::SerializedSize<MessageType>();

        /// @brief コンストラクタ
        /// @param bus CANバス
        /// @param id  自身のノードID
        CanWriter(ICanBus& bus, const uint32_t id)
            : bus{ bus }
            , node{ bus.createTx(id, Size) }
        {
        }

        /// @brief コピーコンストラクタ
        /// @note この関数は明示的に削除されています。
        CanWriter(const CanWriter& other) = delete;

        /// @brief ムーブコンストラクタ
        CanWriter(CanWriter&& other)
            : bus{ other.bus }
            , node{ other.node }
        {
        }

        /// @brief メッセージ構造体をセット
        void setMessage(const Message& message) noexcept
        {
            Udon::Serialize(message, { node->data });
        }

        /// @brief 送信内容を表示
        void show() const
        {
            Udon::Printf("0x%03x ", node->id);
            if (const auto message = Udon::Deserialize<Message>({ node->data }))
            {
                Udon::Show(*message);
            }
            else
            {
                Udon::Show(F("unpack failed!"));
                // ここへ到達する: setMessage()で値を設定していない
            }
        }

        /// @brief 送信バッファを表示
        void showRaw() const
        {
            Udon::Printf("0x%03x ", node->id);
            for (const auto& n : node->data)
            {
                Udon::Show(n);
            }
        }

    private:
        ICanBus& bus;

        CanTxNode* node;
    };

}    // namespace Udon