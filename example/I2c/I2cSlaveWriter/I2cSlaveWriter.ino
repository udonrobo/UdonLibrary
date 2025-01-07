//
//    I2C スレーブ側送信スケッチ例
//
//    Copyright (c) 2022 udonrobo
//

#include <Udon.hpp>

static Udon::I2cBus bus{ Wire };

static Udon::I2cSlaveWriter<Udon::Vec2> writer{ bus };

void setup()
{
    bus.begin(6);
    writer.begin();
}

void loop()
{
    bus.update();

    writer.setMessage({ millis(), micros() });

    delay(10);
}
