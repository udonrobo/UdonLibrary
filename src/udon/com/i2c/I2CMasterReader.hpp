#pragma once

#include <udon\com\i2c\I2cBus.hpp>

#include <udon\com\serializer\serializer.hpp>

namespace udon
{

	template<class Message>
	class I2cMasterReader
	{

	public:

		static constexpr size_t Size = Message::PackedSize;

	private:

		udon::I2cBus& bus;

		uint8_t address;

		uint8_t buffer[Size];

		uint32_t errorCount;

	public:

		I2cMasterReader(udon::I2cBus& bus, uint8_t address)
			: bus(bus)
			, address(address)
			, buffer()
		{}

		void update()
		{
    		bus.requestFrom(address, Size);
			while (bus.available())
			{
				for (auto&& it : buffer)
				{
					const auto d = bus.read();;
					if (d == -1)
					{
						++errorCount;
					}
					else
					{
						it = d;
					}
				}
			}
		}

		Message getMessage() const
		{
			return udon::Unpack<Message>({ std::begin(buffer), std::end(buffer) });
		}

		const uint8_t* data() const
		{
			return buffer;
		}

	};

} // namespace udon


