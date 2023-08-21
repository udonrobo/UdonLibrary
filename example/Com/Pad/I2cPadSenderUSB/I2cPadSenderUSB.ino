//-------------------------------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    PS5コントローラー送信クラス スケッチ例 (I2C)
//
//    Controller --[USB]--> USBHost --[SPI]--> Sender --> 
//                                             ^^^^^^
//
//-------------------------------------------------------------------

#include <Udon.hpp>
#include <Udon/Com/Pad/PadPS5USB.hpp>

Udon::I2cBus bus{ Wire };

Udon::I2cSlaveWriter<Udon::Message::PadPS5> writer{ bus };

Udon::PadPS5USB pad;

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
    writer.begin();
}

void loop()
{
    bus.update();
    pad.update();

    //   pad --[i2c<Message::PadPS5>]--> master
    writer.setMessage(pad.getButtons());

    pad.setLightBar({ 0x38b48b });
    pad.setMicLed(true);

}