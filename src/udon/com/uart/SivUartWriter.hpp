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
//    OpenSiv3D用 UART 送信クラス
//
//    Sender --[UART]--> Receiver
//    ^^^^^^
//
//-------------------------------------------------------------------


#pragma once

#include <Siv3D/Serial.hpp>
#include <udon/com/serialization/Serializer.hpp>
#include <udon/utility/Show.hpp>

namespace udon
{
    template <typename Message>
    class SivUartWriter
    {
        static constexpr size_t Size = udon::CapacityWithChecksum<Message>();

        s3d::Serial& serial;

    public:
        SivUartWriter(s3d::Serial& bus)
            : serial(bus)
        {
        }

        void setMessage(const Message& message)
        {
            uint8_t buffer[Size];
            udon::Pack(message, buffer);
            serial.write(buffer, Size);
        }
    };
}    // namespace udon
