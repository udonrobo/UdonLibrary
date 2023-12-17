#pragma once

#include <serial/serial.h>
#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Common/Show.hpp>

namespace Udon
{
    template <typename Message>
    class Ros2UartReader
    {
        static constexpr size_t Size = Udon::SerializedSize<Message>();

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
            return Udon::Deserialize<Message>(buffer);
        }

        void show() const
        {
            if (const auto message = getMessage())
            {
                Udon::Show(*message);
            }
            else
            {
                Udon::Show("receive failed!");
            }
        }

        void showRaw() const
        {
            Udon::Show(buffer);
        }
    };
}    // namespace Udon