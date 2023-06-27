//-----------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-----------------------------------------------
//
//    PS5コントローラー受信クラス スケッチ例 (I2C)
//
//    Controller --[Bluetooth]--> sender --[I2C]--> master
//                                                  ^^^^^^
//
//-----------------------------------------------

#include <Udon.hpp>

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

    if (pad)
    {
        writer.setMessage({ 0xffffff });
        writer.update();
    }
    
    delay(10);
}
