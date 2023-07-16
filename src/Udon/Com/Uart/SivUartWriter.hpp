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
#include <Udon/Com/Serialization.hpp>
#include <Udon/Utility/Show.hpp>

namespace Udon
{
    template <typename Message>
    class SivUartWriter
    {
        static constexpr size_t Size = Udon::CapacityWithChecksum<Message>();

        s3d::Serial& serial;

    public:
        SivUartWriter(s3d::Serial& bus)
            : serial(bus)
        {
        }

        void setMessage(const Message& message)
        {
            uint8_t buffer[Size];
            Udon::Pack(message, buffer);
            serial.write(buffer, Size);
        }
    };
}    // namespace Udon
