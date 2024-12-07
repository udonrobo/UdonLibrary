#include <Udon.hpp>

struct SteerModuleSchema
{
    double angle;
    double power;

    UDON_ENUMERABLE(angle, power)
};

static Udon::CanBusTeensy<CAN1> bus;

static Udon::CanWriter<SteerModuleSchema> writer{ bus, 0x001 };

void setup() 
{
    bus.begin();
}

void loop()
{
    bus.update();
    writer.setMessage(SteerModuleSchema{ 3.14, 100 });

    delay(10);
}
