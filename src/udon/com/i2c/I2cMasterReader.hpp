#pragma once

#include <udon/com/i2c/I2cBus.hpp>
#include <udon/utility/Show.hpp>
#include <udon/com/serialization/Serializer.hpp>

namespace udon
{

    template <class Message>
    class I2cMasterReader
    {

        static constexpr size_t Size = udon::CapacityWithChecksum<Message>();

        udon::II2cBus& bus;

        uint8_t address;

        uint8_t buffer[Size];

        uint32_t errorCount;

    public:
        template <typename Bus>
        I2cMasterReader(Bus& bus, uint8_t address)
            : bus(bus)
            , address(address)
            , buffer()
            , errorCount()
        {
        }

        void update()
        {
            bus.requestFrom(address, Size);
            while (bus.available())
            {
                Serial.println("callll");
                for (auto&& it : buffer)
                {
                    switch (const auto d = bus.read())
                    {
                    case -1:    // error
                        errorCount += 5;
                        break;
                    default:
                        it = d;
                        --errorCount;
                        break;
                    }
                }
            }
        }

        explicit operator bool() const
        {
            return errorCount < 128;
        }

        udon::optional<Message> getMessage() const
        {
            return udon::Unpack<Message>(buffer);
        }

        void show() const
        {
            if (const auto message = getMessage())
            {
                udon::Show(*message);
            }
            else
            {
                Serial.print(F("receive error!"));
            }
        }

        void showRaw() const
        {
            for (auto&& it : buffer)
            {
                Serial.print(it, HEX);
                Serial.print(' ');
            }
        }

        const uint8_t* data() const
        {
            return buffer;
        }
    };

}    // namespace udon
