#ifndef ARDUINO_ARCH_RP2040

#    include <Udon.hpp>

udon::I2cBus bus{ Wire };

udon::PadPS5<udon::I2cMasterReader> pad{ { bus, 6 } };

udon::I2cMasterWriter<udon::RGB> writer{ bus, 6 };

void setup()
{
    bus.begin();
}

void loop()
{
    bus.update();

    pad.update();
    pad.show();

    Serial.println();

    if (pad)
    {
        writer.setMessage({ 0xffffff });
        writer.update();
    }
    delay(10);
}

#else
void setup() {}
void loop() {}
#endif