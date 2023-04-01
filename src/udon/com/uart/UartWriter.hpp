#pragma once

#include <udon\com\serializer\serializer.hpp>
#include <udon\com\uart\UartBus.hpp>

namespace udon
{

	template<class Message>
	class UartWriter
	{

			udon::UartBus& uart;

			Message message;

		public:

			UartWriter(udon::UartBus& uart) noexcept
				: uart(uart)
				, message()
			{}


			void setMessage(const Message& rhs)
			{
				message = rhs;
			}

			void update()
			{
				for (auto && it : udon::Pack(message))
				{
					uart.write(it);
				}
			}

			void show() const
			{
				message.show();
			}

	};

} // namespace udon
