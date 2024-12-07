#include <Udon.hpp>

static Udon::CanBusTeensy<CAN1> bus;

static Udon::CanWriter<int> writer1{ bus, 0x001 };
static Udon::CanWriter<int> writer2{ bus, 0x002 };

void setup()
{
    bus.begin();
}

void loop()
{
    bus.update();

    writer1.setMessage(100);
    writer2.setMessage(200);
}
