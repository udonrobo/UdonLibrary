#pragma once

#include <serial/serial.h>
#include <Udon/Com/Serialization.hpp>
#include <Udon/Utility/Show.hpp>

namespace Udon
{
    template <typename Message>
    class Ros2UartWriter
    {
        static constexpr size_t Size = Udon::CapacityWithChecksum<Message>();

        serial::serial& serial;

    public:
        Ros2UartWriter(serial::serial& bus)
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
