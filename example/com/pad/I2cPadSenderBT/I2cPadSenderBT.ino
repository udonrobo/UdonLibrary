//-----------------------------------------------
//
//	UdonLibrary
//
//	Copyright (c) 2022-2023 Okawa Yusuke
//	Copyright (c) 2022-2023 udonrobo
//
//	Licensed under the MIT License.
//
//-----------------------------------------------
//
//  PS5コントローラー送信クラス スケッチ例 (I2C)
//
//  Controller --[Bluetooth]--> sender --[I2C]--> master
//                              ^^^^^^
//
//-----------------------------------------------

#include <Udon.hpp>
#include <udon/com/pad/PadPS5BT.hpp>

udon::I2cBus bus{ Wire };

udon::I2cSlaveWriter<udon::message::PadPS5> padWriter{ bus };

udon::I2cSlaveReader<udon::RGB> padColorReader{ bus };

udon::PadPS5BT pad;

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
    padWriter.begin();
    padColorReader.begin();
}

void loop()
{
    bus.update();
    pad.update();

    //   pad --[i2c<message::PadPS5>]--> master
    padWriter.setMessage(pad.getButtons());

    //   master --[i2c<RGB>]--> pad
    if (const auto color = padColorReader.getMessage())
    {
        pad.setColor(*color);
    }
    else
    {
        // default: jade green
        pad.setColor(udon::RGB{ 0x38b48b });
    }

    pad.setMicLed(true);

    // Serial.println(bus ? "I2c: OK" : "I2c: NG");
}