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
//    Bluetooth経由PS5コントローラークラス
//
//    Controller --[Bluetooth]--> Sender --> Master
//                                ^^^^^^
//
//-------------------------------------------------------------------

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
        PadPS5BT(bool pair = false)
            : usb()
            , btd(&usb)
            , pad(&btd, pair)
        {
        }

        /// @brief コントローラーと通信開始
        /// @return
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

        /// @brief LEDバーの色を設定する
        /// @param color
        void setColor(const Udon::RGB& color)
        {
            pad.setLed(color.r, color.g, color.b);
        }

        /// @brief マイクのLEDの点灯を設定する
        /// @param on
        void setMicLed(bool on)
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
