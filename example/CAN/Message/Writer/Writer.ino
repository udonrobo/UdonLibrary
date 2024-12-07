#include <Udon.hpp>

static Udon::CanBusTeensy<CAN1> bus;

static Udon::CanWriter<Udon::Message::Motor> motor{ bus, 0x001 };
static Udon::CanWriter<Udon::Message::Servo> servo{ bus, 0x002 };

void setup()
{
    bus.begin();
}

void loop()
{
    bus.update();

    motor.setMessage({ 123 });
    servo.setMessage({ 120 });
}
