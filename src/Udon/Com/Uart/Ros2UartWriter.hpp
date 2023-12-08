#pragma once

#include <serial/serial.h>
#include <Udon/Com/Serialization.hpp>
#include <Udon/Common/Show.hpp>

namespace Udon
{
    template <typename Message>
    class Ros2UartWriter
    {
        static constexpr size_t Size = Udon::SerializedSize<Message>();

        serial::Serial& serial;

    public:
        Ros2UartWriter(serial::Serial& bus)
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
