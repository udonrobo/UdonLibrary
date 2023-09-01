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
//    メッセージ配列送信クラス
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Traits/Parsable.hpp>

namespace Udon
{
    template <typename Message>
    class ArrayElementWriter
    {

        static_assert(Udon::Traits::Parsable<Message>::value, "Message must be parsable.");

    public:
        using MessageType = Message;

        ArrayElementWriter(MessageType& message)
            : message(message)
        {
        }

        void setMessage(const MessageType& message)
        {
            this->message = message;
        }

    private:
        MessageType& message;
    };

}    // namespace Udon
