//
//    PS5コントローラー送信クラス スケッチ例 (I2C)
//
//    Copyright (c) 2022 udonrobo
//
//    Controller --[Bluetooth]--> USBHost --[SPI]--> Sender --> 
//                                                   ^^^^^^
//

#include <Udon.hpp>
#include <Udon/Com/Pad/PadPS5BT.hpp>

static Udon::I2cBus bus{ Wire };

static Udon::I2cSlaveWriter<Udon::Message::PadPS5> writer{ bus };

static Udon::PadPS5BT pad;

void setup()
{
    Serial.begin(115200);
    if (not pad.begin())
    {
        Serial.println(F("PS5 BT failed to start!"));
        for (;;)
            ;
    }
    bus.begin(6);
    writer.begin();
}

void loop()
{
    bus.update();
    pad.update();

    //   pad --[i2c<Message::PadPS5>]--> master
    writer.setMessage(pad.getMessage());

    pad.setLightBar({ 0x38b48b });
    pad.setMicLed(true);

}