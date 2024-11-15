#include <Udon.hpp>

static Udon::CanBusTeensy<CAN1> bus;

static Udon::CanWriter<int> writer{ bus, 0x002 };

void setup()
{
    bus.begin();
}

void loop()
{
    bus.update();
    writer.setMessage(millis());
    delay(10);
}
