#include <Udon.hpp>
#include <Udon/Driver/RoboMasterMotor.hpp>

Udon::CanBusTeensy<CAN2> bus;
Udon::RoboMasterMotor motor{ bus, 1 };

void setup() {
    bus.begin();
}

void loop() {
    bus.update();

    motor.setCurrent(16000);
    const auto rpm = motor.getSpeed();
    
    Serial.println(rpm);
}
