//
//    構文テスト
// 
//    Copyright (c) 2022-2024 udonrobo
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

inline void test()
{
    DummyBus bus;
    {
        Udon::RoboMasterC610 motor{ bus, 0x000, true };
        motor.setCurrent(0);
        motor.getAngle();
        motor.getVelocity();
        motor.getTorqueCurrent();
        motor.getTemperature();
    }

    {
        Udon::RoboMasterC620 motor{ bus, 0x000, true };
        motor.setCurrent(0);
        motor.getAngle();
        motor.getVelocity();
        motor.getTorqueCurrent();
        motor.getTemperature();
    }
}