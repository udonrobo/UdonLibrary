//
//    メッセージ配列受信クラス
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include <Udon/Stl/Optional.hpp>
#include <Udon/Traits/SerializerTraits.hpp>

namespace Udon
{
    template <typename Message>
    class ArrayElementReader
    {

        static_assert(Udon::Traits::IsSerializable<Message>::value, "Message must be parsable.");

    public:
        using MessageType = Message;

        ArrayElementReader(const MessageType& message, const bool& hasValue)
            : message(message)
            , hasValue(hasValue)
        {
        }

        Udon::Optional<MessageType> getMessage() const
        {
            if (hasValue)
            {
                return message;
            }
            else
            {
                return Udon::nullopt;
            }
        }

    private:
        const MessageType& message;
        const bool&        hasValue;
    };
}    // namespace Udon
