//
//    CAN通信 送信クラス
//
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include "ICanBus.hpp"
#include "CanNode.hpp"

#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Common/Show.hpp>
#include <Udon/Serializer/SerializerTraits.hpp>
#include <Udon/Common/Printf.hpp>

namespace Udon
{

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
            , p{ bus.createTx(id, Size) }
        {
        }

        /// @brief コピーコンストラクタ
        CanWriter(const CanWriter& other)
            : bus{ other.bus }
            , p {other.p}
        {
        }

        /// @brief メッセージ構造体をセット
        void setMessage(const Message& message) noexcept
        {
            Udon::Serialize(message, { p->data, p->length });
        }

        /// @brief 送信内容を表示
        /// @param gap 区切り文字 (default: '\t')
        void show() const
        {
            Udon::Printf("0x%03x ", node.id);
            if (const auto message = Udon::Deserialize<Message>({ p->data, p->length }))
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
            Udon::Printf("0x%03x ", p->id);
            for (size_t i = 0; i < p->length; ++i)
            {
                Udon::Show(p->data[i]);
            }
        }

    private:
        ICanBus& bus;

        CanNode* p;
    };

}    // namespace Udon