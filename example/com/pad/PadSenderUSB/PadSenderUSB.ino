#ifndef ARDUINO_ARCH_RP2040

#    include <Udon.hpp>
#    include <udon/com/pad/PadPS5USB.hpp>

udon::I2cBus bus{ Wire };

udon::I2cSlaveWriter<udon::message::PadPS5> padWriter{ bus };

udon::I2cSlaveReader<udon::RGB> padColorReader{ bus };

udon::PadPS5USB pad;

void setup()
{
    Serial.begin(115200);
    if (not pad.begin())
    {
        Serial.println(F("PS5USB failed to start!"));
        for (;;)
            ;
    }
    bus.begin(6);
    padWriter.begin();
    padColorReader.begin();
}

void loop()
{
    bus.update();
    bus.show();

    pad.update();

    // pad -> i2c<message::PadPS5>
    padWriter.setMessage(pad.getButtons());

    // i2c<Color> -> pad color
    if (const auto color = padColorReader.getMessage())
    {
        pad.setColor(*color);
    }
    else
    {
        pad.setColor(udon::RGB{ 0x38b48b });    // default: jade green
    }

    Serial.println();
    delay(10);
}

#else
void setup() {}
void loop() {}
#endif