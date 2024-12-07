#include <Udon.hpp>

static Udon::CanBusTeensy<CAN1> bus;

static Udon::CanReader<int> reader1{ bus, 0x001 };
static Udon::CanReader<int> reader2{ bus, 0x002 };

void setup()
{
    bus.begin();
}

void loop()
{
    bus.update();

    if (const auto message = reader1.getMessage())
    {
        Serial.println(*message);
    }
    else
    {
        Serial.println("receive failed !!");
    }

    if (const auto message = reader2.getMessage())
    {
        Serial.println(*message);
    }
    else
    {
        Serial.println("receive failed !!");
    }

    delay(10);
}
