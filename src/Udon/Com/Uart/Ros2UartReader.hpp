#pragma once

#include <serial/serial.h>
#include <Udon/Com/Serialization.hpp>
#include <Udon/Common/Show.hpp>

namespace Udon
{
    template <typename Message>
    class Ros2UartReader
    {
        static constexpr size_t Size = Udon::PackedSize<Message>();

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
                std::vector<uint8_t> buf;
                serial.read(buf, Size);
                buffer = std::move(buf);
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
            for (int i = 0; i < buffer.size(); i++)
            {
                std::cout << static_cast<int>(buffer[i]) << " ";
            }
            std::cout << std::endl;
        }
    };
}    // namespace Udon