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

        uint32_t             transmitMs;

    public:
        Ros2UartReader(serial::Serial& bus)
            : serial(bus)
            , buffer(Size)
        {
        }

        explicit operator bool() const
        {
            return millis() - transmitMs < 1000;
        }

        void update()
        {
            if (not serial.isOpen())
            {
                serial.open();
                continue;
            }

            if (serial.available() >= static_cast<int>(Size))
            {
                std::vector<uint8_t> temp;
                if (serial.read(temp) && temp.size() == Size)
                {
                    buffer = std::move(temp);
                }
                transmitMs = millis();
                serial.flushInput();
            }
        }

        Udon::Optional<Message> getMessage() const
        {
            if (operator bool())
            {
                return Udon::Unpack<Message>(buffer);
            }
            else
            {
                return Udon::nullopt;
            }
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