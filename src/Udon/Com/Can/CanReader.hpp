//
//    CAN通信 受信クラス
//
//    Copyright (c) 2022 udonrobo
//

#pragma once

#include "ICanBus.hpp"

#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Serializer/SerializerTraits.hpp>
#include <Udon/Utility/Show.hpp>
#include <Udon/Utility/Time.hpp>
#include <Udon/Utility/Printf.hpp>

namespace Udon
{

    /// @brief CAN通信 受信クラス
    /// @tparam Message 受信メッセージ型
    template <typename Message>
    class CanReader
    {

        static_assert(Traits::IsSerializable<Message>::value, "Message must be parsable.");

    public:
        /// @brief 受信メッセージ型
        using MessageType = Message;

        /// @brief 受信バッファサイズ
        static constexpr size_t Size = Udon::SerializedSize<Message>();

        /// @brief コンストラクタ
        /// @param bus CANバス
        /// @param id 送信者のノードID
        CanReader(ICanBus& bus, const uint32_t id)
            : node{ bus.createRx(id, Size) }
        {
            node->onReceive = [](void* p)
            {
                auto self = static_cast<CanReader*>(p);
                self->message = Udon::Deserialize<Message>({ self->node->data });
            };
            node->param = this;
        }

        /// @brief コピーコンストラクタ
        /// @note この関数は明示的に削除されています。
        CanReader(const CanReader&) = delete;

        /// @brief コピーコンストラクタ
        CanReader(CanReader&& other)
            : node{ other.node }
        {
            node->param = this;    // ここでotherに登録されていたthisポインタを上書きするため、コピーすることができない
        }

        /// @brief 受信しているか
        /// @return 受信していればtrue
        explicit operator bool() const
        {
            return Millis() - node->transmitMs < 100;
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
        void show() const
        {
            Udon::Printf("0x%03x ", node->id);
            if (const auto m = getMessage())
            {
                Udon::Show(*m);
            }
            else
            {
                Udon::Show(F("receive failed!"));
            }
        }

        /// @brief 受信バッファを表示
        void showRaw() const
        {
            Udon::Printf("0x%03x ", node->id);
            for (const auto n : node->data)
            {
                Udon::Show(n);
            }
        }

    private:
        CanRxNode* node;

        Udon::Optional<MessageType> message;
    };

}    // namespace Udon
