#pragma once

#include <udon/com/i2c/I2cBus.hpp>
#include <udon/com/serializer/Serializer.hpp>

namespace udon
{

    template <class Message>
    class I2cMasterWriter
    {

        static constexpr size_t Size = udon::CapacityWithChecksum<Message>();

        udon::II2cBus& bus;

        uint8_t address;

        uint8_t buffer[Size];

        uint32_t errorCount;

    public:
        template <typename Bus>
        I2cMasterWriter(Bus& bus, uint8_t address)
            : bus(bus)
            , address(address)
            , buffer()
        {
        }

        void update()
        {
			bus.beginTransmission(address);
			bus.write(buffer, Size);
			bus.endTransmission();
        }

		void setMessage(const Message& message)
		{
			udon::Pack(message, buffer);
		}

        const uint8_t* data() const
        {
            return buffer;
        }
    };

}    // namespace udon
