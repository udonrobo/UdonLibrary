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
//    PS5コントローラー受信クラス スケッチ例 (I2C)
//
//    Controller --[Bluetooth]--> sender --[I2C]--> master
//                                                  ^^^^^^
//
//-------------------------------------------------------------------

#include <Udon.hpp>

Udon::I2cBus bus{ Wire };

Udon::PadPS5<Udon::I2cMasterReader> pad{ { bus, 6 } };

Udon::I2cMasterWriter<Udon::RGB> writer{ bus, 6 };

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
        writer.setMessage({ 0xffffff });
        writer.update();
    }
    
    delay(10);
}
