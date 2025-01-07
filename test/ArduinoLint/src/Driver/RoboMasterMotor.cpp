//
//    構文テスト
//
//    Copyright (c) 2022 udonrobo
//

#include <Arduino.h>
#include <Udon/Driver/RoboMasterMotor.hpp>


struct DummyBus
    : Udon::ICanBus
{
    explicit operator bool() const { return true; }
    Udon::CanTxNode* createTx(uint32_t, size_t) override { return nullptr; }
    Udon::CanRxNode* createRx(uint32_t, size_t) override { return nullptr; }
};

__attribute__((unused)) static void test()
{
    DummyBus bus;
    {
        Udon::RoboMasterC610 motor{ bus, 0x000, Udon::Direction::Forward };
        motor.setCurrent(0);
        motor.getAngle();
        motor.getVelocity();
        motor.getTorqueCurrent();
        motor.getTemperature();
    }

    {
        Udon::RoboMasterC620 motor{ bus, 0x000, Udon::Direction::Backward };
        motor.setCurrent(0);
        motor.getAngle();
        motor.getVelocity();
        motor.getTorqueCurrent();
        motor.getTemperature();
    }
}