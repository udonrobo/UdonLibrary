//
//    I2C マスター側送信スケッチ例
//
//    Copyright (c) 2022-2023 udonrobo
//

#include <Udon.hpp>

Udon::I2cBus bus{ Wire };

Udon::I2cMasterWriter<Udon::Vec2> writer{ bus, 6 };

void setup()
{
    bus.begin();
}

void loop()
{
    bus.update();

    writer.setMessage({ millis(), micros() });

    delay(10);
}
