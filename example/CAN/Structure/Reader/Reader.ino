#include <Udon.hpp>

struct SteerModuleSchema
{
    double angle;
    double power;

    UDON_ENUMERABLE(angle, power)
};

static Udon::CanBusTeensy<CAN1> bus;

static Udon::CanReader<SteerModuleSchema> reader{ bus, 0x001 };

void setup()
{
    bus.begin();
}

void loop()
{
    bus.update();

    if (const Udon::Optional<SteerModuleSchema> message = reader.getMessage())
    {
        Serial.print(message->angle), Serial.print('\t');
        Serial.print(message->power), Serial.print('\n');
    }
    else
    {
        Serial.println("receive faild !!");
    }

    delay(10);
}
