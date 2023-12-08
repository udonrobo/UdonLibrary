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
#include <Udon/Common/Show.hpp>

namespace Udon
{
    template <typename Message>
    class SivUartWriter
    {
        static constexpr size_t Size = Udon::SerializedSize<Message>();

        s3d::Serial& serial;

    public:
        SivUartWriter(s3d::Serial& bus)
            : serial(bus)
        {
        }

        void setMessage(const Message& message)
        {
            if (serial.isOpen())
            {
                const auto packed = Udon::Serialize(message);
                serial.write(packed.data(), packed.size());
            }
        }
    };
}    // namespace Udon
