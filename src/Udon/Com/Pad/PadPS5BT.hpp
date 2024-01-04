//
//    Bluetooth経由PS5コントローラークラス
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Controller --[Bluetooth]--> USBHost --[SPI]--> Sender -->
//                                                   ^^^^^^
//
//    ペアリング手順:
//        1. コンストラクタの引数 pair に PAIR(true) を渡して書き込む
//        2. コントローラーの \|/ボタン と PSボタン を同時に押し続ける
//        3. コントローラーのLEDが点滅し始める
//        4. ペアリングが完了するとLEDが点灯する
//
//    本ヘッダーはUSB Host Shield 2.0ライブラリを使用しており、ヘッダーの量が多いため、使用する際は個別でインクルードすること。
//    

#pragma once

#include <Udon/thirdparty/USB_Host_Shield_2.0/PS5BT.h>

#include <Udon/Com/Message/PadPS5.hpp>
#include <Udon/Stl/Optional.hpp>
#include <Udon/Types/RGB.hpp>

namespace Udon
{
    class PadPS5BT
    {
        USB usb;

        BTD btd;

        PS5BT pad;

        Message::PadPS5 buttons;

    public:
        /// @brief コンストラクタ
        /// @param pair ペアリングするかどうか
        /// @details ペアリングする場合は PAIR を渡す
        PadPS5BT(bool pair = false)
            : usb()
            , btd(&usb)
            , pad(&btd, pair)
        {
        }

        /// @brief コントローラーと通信開始
        /// @return 成功したら true
        bool begin()
        {
            if (usb.Init() == -1)
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        /// @brief 更新
        void update()
        {
            usb.Task();
            if (pad.connected())
            {
                if (pad.getButtonPress(ButtonEnum::PS))
                {
                    pad.disconnect();
                }

                buttons.isConnected = true;

                buttons.triangle = pad.getButtonPress(ButtonEnum::TRIANGLE);    // ▵
                buttons.circle   = pad.getButtonPress(ButtonEnum::CIRCLE);      // ○
                buttons.cross    = pad.getButtonPress(ButtonEnum::CROSS);       // ×
                buttons.square   = pad.getButtonPress(ButtonEnum::SQUARE);      // □

                buttons.up    = pad.getButtonPress(ButtonEnum::UP);       // ↑
                buttons.right = pad.getButtonPress(ButtonEnum::RIGHT);    // →
                buttons.down  = pad.getButtonPress(ButtonEnum::DOWN);     // ↓
                buttons.left  = pad.getButtonPress(ButtonEnum::LEFT);     // ←

                buttons.l1 = pad.getButtonPress(ButtonEnum::L1);
                buttons.r1 = pad.getButtonPress(ButtonEnum::R1);

                buttons.l2 = pad.getButtonPress(ButtonEnum::L2);
                buttons.r2 = pad.getButtonPress(ButtonEnum::R2);

                buttons.l3 = pad.getButtonPress(ButtonEnum::L3);    // 左スティック押し込み
                buttons.r3 = pad.getButtonPress(ButtonEnum::R3);    // 右スティック押し込み

                buttons.create = pad.getButtonPress(ButtonEnum::SHARE /*CREATE*/);    // 左上 \|/ ボタン
                buttons.option = pad.getButtonPress(ButtonEnum::START /*OPTION*/);    // 右上  ≡  ボタン

                buttons.touch = pad.getButtonPress(ButtonEnum::TOUCHPAD);      // タッチパッド
                buttons.mic   = pad.getButtonPress(ButtonEnum::MICROPHONE);    // ミュートボタン
                buttons.ps    = false;                                         // PSボタン(無効)

                buttons.analogRightX = +(pad.getAnalogHat(AnalogHatEnum::RightHatX) - 128);    // 0~255 -> -128~127
                buttons.analogRightY = -(pad.getAnalogHat(AnalogHatEnum::RightHatY) - 127);    // 0~255 -> -128~127

                buttons.analogLeftX = +(pad.getAnalogHat(AnalogHatEnum::LeftHatX) - 128);    // 0~255 -> -128~127
                buttons.analogLeftY = -(pad.getAnalogHat(AnalogHatEnum::LeftHatY) - 127);    // 0~255 -> -128~127
            }
            else
            {
                buttons.clear();
            }
        }

        /// @brief Message::PadPS5型のメッセージを取得する
        /// @return
        Message::PadPS5 getButtons() const
        {
            return buttons;
        }

        /// @brief ライトバーの色を設定する
        /// @param color
        void setLightBar(const Udon::RGB& color)
        {
            pad.setLed(color.r, color.g, color.b);
        }

        /// @brief 5つのプレイヤーのランプの点灯を設定する
        /// @param mask
        ///     0b001:  HIGH LOW  LOW  LOW  HIGH
        ///     0b010:  LOW  HIGH LOW  HIGH LOW
        ///     0b100:  LOW  LOW  HIGH LOW  LOW
        void setPlayerLamp(uint8_t mask = 0b011)
        {
            pad.setPlayerLed(mask);
        }

        /// @brief マイクのLEDの点灯を設定する
        /// @param on
        void setMicLed(bool on = true)
        {
            pad.setMicLed(on);
        }

        /// @brief バイブレーション
        /// @param big 大きい振動モーターの強さ 0~255
        /// @param small 小さい振動モーターの強さ 0~255
        void vibrate(uint8_t big = 100, uint8_t small = 100)
        {
            pad.setRumbleOn(big, small);
        }
    };
}    // namespace Udon
