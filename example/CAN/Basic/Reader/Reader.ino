#include <Udon.hpp>

static Udon::CanBusTeensy<CAN1> bus;

static Udon::CanReader<int> reader{ bus, 0x002 };

void setup()
{
    bus.begin();
}

void loop()
{
    bus.update();
    
    if (const Udon::Optional<int> message = reader.getMessage())
    {
        Serial.println(*message);
    }
    else
    {
        Serial.println("receive faild !!");
    }

    delay(10);
}
