#pragma once

#include <serial/serial.h>
#include <Udon/Com/Serialization.hpp>
#include <Udon/Utility/Show.hpp>

namespace Udon
{
    template <typename Message>
    class Ros2UartReader
    {
        static constexpr size_t Size = Udon::CapacityWithChecksum<Message>();

        serial::Serial& serial;

        std::vector<uint8_t> buffer;

    public:
        Ros2UartReader(serial::Serial& bus)
            : serial(bus)
            , buffer(Size)
        {
        }

        void update()
        {
            if (not serial.isOpen())
            {
                serial.open();
            }
            if (serial.available() >= static_cast<int>(Size))
            {
                serial.read(buffer, Size);
                std::cout << "read" << std::endl;
                serial.flushInput();
            }
        }

        Udon::Optional<Message> getMessage() const
        {
            return Udon::Unpack<Message>(buffer);
        }

        void show(char gap = '\t') const
        {
            if (const auto message = getMessage())
            {
                Udon::Show(*message, gap);
            }
            else
            {
                Udon::Show("receive failed!", gap);
            }
        }

        void showRaw(char gap = '\t') const
        {
            Udon::Show(buffer, gap);
        }
    };
}    // namespace Udon