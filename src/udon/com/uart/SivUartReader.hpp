#pragma once

#include <Siv3D/Serial.hpp>
#include <udon/com/serialization/Serializer.hpp>
#include <udon/utility/Show.hpp>

namespace udon
{
    template <typename Message>
    class SivUartReader
    {
        static constexpr size_t Size = udon::CapacityWithChecksum<Message>();

        s3d::Serial& serial;

        uint8_t buffer[Size];

        std::thread thread;

        bool isRunning;    // thread stop token

    public:
        SivUartReader(s3d::Serial& bus)
            : serial(bus)
            , buffer()
            , thread([this]()
                     {
                        while(isRunning)
                        {
                            if (not serial) continue;
                            if (serial.available() < Size) continue;

                            Array<uint8> d;
                            if(serial.readBytes(d) && d.size() == Size)
                            {
                                std::copy(d.begin(), d.end(), buffer);
                            }

                            serial.clearInput();
                        } })
            , isRunning(true)
        {
        }

        ~SivUartReader()
        {
            isRunning = false;
            thread.join();
        }

        udon::optional<Message> getMessage() const
        {
            return udon::Unpack<Message>(buffer);
        }

        void show(char gap = '\t') const
        {
            if (const auto message = getMessage())
            {
                udon::Show(*message, gap);
                Print.writeln();
            }
            else
            {
                Print << U"receive failed!";
            }
        }

        void showRaw() const
        {
            Print << buffer;
        }
    };
}    // namespace udon
