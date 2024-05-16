//
//    PS5コントローラー受信クラス スケッチ例 (I2C)
//
//    Copyright (c) 2022-2024 udonrobo
//
//    Controller --[Bluetooth]--> USBHost --[SPI]--> Sender --> Master
//                                                              ^^^^^^
//

#include <UdonFwd.hpp>
#include <Udon/Com/I2c.hpp>

static Udon::I2cBus bus{ Wire };

static Udon::I2cPadPS5 pad{ bus, 6 };

void setup()
{
    bus.begin();
}

void loop()
{
    bus.update();
    pad.update();

    if (pad)
    {
        Serial.print(pad.getRightStick().x);    // 右スティックのx軸
        Serial.print(", ");
        Serial.print(pad.getRightStick().y);    // 右スティックのy軸
        Serial.print(", ");
        Serial.print(pad.getUp().press);    // 上ボタンの押下状態
        Serial.print(", ");
        Serial.println();
    }
    else
    {
        Serial.println("disconnected.");
    }

    delay(10);
}
