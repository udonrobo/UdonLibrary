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

        std::thread thread;

        bool isRunning;    // thread stop token

    public:
        SivUartReader(serial::Serial& bus)
            : serial(bus)
            , buffer(Size)
            , thread(
                  [this]()
                  {
                      while (isRunning)
                      {
                          if (not serial.isOpen())
                              continue;
                          if (serial.available() < Size)
                              continue;

                          std::vector<uint8_t> temp;
                          if (serial.read(temp) && temp.size() == Size)
                          {
                              buffer = std::move(temp);
                          }

                          serial.flushInput();
                      }
                  })
            , isRunning(true)
        {
        }

        ~Ros2UartReader()
        {
            isRunning = false;
            thread.join();
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

        void showRaw() const
        {
            Udon::Show(buffer, gap);
        }
    };
}    // namespace Udon