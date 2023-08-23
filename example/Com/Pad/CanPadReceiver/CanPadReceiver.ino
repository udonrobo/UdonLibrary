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
//    PS5コントローラー受信クラス スケッチ例 (CAN)
//
//    Controller --[Bluetooth]--> USBHost --[SPI]--> Sender --> Master
//                                                              ^^^^^^
//
//-------------------------------------------------------------------

#include <UdonFwd.hpp>
#include <Udon/Com/Can.hpp>

Udon::CanBusTeensy<CAN1> bus;

Udon::CanPadPS5 pad{ bus, 6 };

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