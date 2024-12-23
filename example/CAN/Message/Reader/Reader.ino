#include <Udon.hpp>

static Udon::CanBusTeensy<CAN1> bus;

static Udon::CanReader<Udon::Message::Motor> motor{ bus, 0x001 };
static Udon::CanReader<Udon::Message::Servo> servo{ bus, 0x002 };

void setup()
{
    bus.begin();
}

void loop()
{
    bus.update();

    if (const auto message = motor.getMessage())
    {
        int16_t power = message->power;
        Serial.println(power);
    }
    else
    {
        Serial.println("receive failed !!");
    }

    if (const auto message = servo.getMessage())
    {
        uint16_t angle = message->angle;
        Serial.println(angle);
    }
    else
    {
        Serial.println("receive failed !!");
    }
}
