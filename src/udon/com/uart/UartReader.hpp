#pragma once

#include <udon\com\serializer\serializer.hpp>
#include <udon\com\uart\UartBus.hpp>

#include <udon\stl\EnableSTL.hpp>

#include <type_traits>
#include <udon\metapro\has_member.hpp>

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
            uint8_t buffer[Message::PackedSize];
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
                message = udon::Unpack<Message>({ std::begin(buffer), std::end(buffer) });
            }
        }

        Message getMessage() const noexcept
        {
			// static_assert(udon::has_show_v<T>, "");
            return message;
        }


		template<class T = Message>
        typename std::enable_if<udon::has_show_v<T>, void>::type
        show()
        {
            message.show();
        }


		template<class T = Message>
        typename std::enable_if<!udon::has_show_v<T>, void>::type
        show()
        {
            message.show();
        }
    };

}    // namespace udon
