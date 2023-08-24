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
//    メッセージ配列受信クラス
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Stl/Optional.hpp>

namespace Udon
{
    template <typename Message>
    class ArrayElementReader
    {
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
