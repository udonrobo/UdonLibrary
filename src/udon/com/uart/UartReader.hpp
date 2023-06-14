#pragma once

#include <udon/com/serialization/Serializer.hpp>
#include <udon/com/uart/UartBus.hpp>

#include <udon/stl/EnableSTL.hpp>

#include <type_traits>

namespace udon
{

    template <class Message>
    class UartReader
    {

        udon::UartBus& uart;

        Message message;

    public:

        UartReader(udon::UartBus& uart) noexcept
            : uart(uart)
            , message()
        {
        }

        void update() noexcept
        {
            uint8_t buffer[udon::CapacityWithChecksum<Message>()];
            bool    isError = false;
            while (uart.available())
            {
                for (auto&& it : buffer)
                {
                    const auto d = uart.read();
                    if (d == -1)
                    {
                        isError = true;
                    }
                    else
                    {
                        it = d;
                    }
                }
            }

            if (isError == false)
            {
                if (const auto m = udon::Unpack<Message>(buffer))
                {
                    message = m.value();
                }
            }
        }

        Message getMessage() const noexcept
        {
            return message;
        }

    };

}    // namespace udon
