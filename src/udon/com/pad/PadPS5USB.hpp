#pragma once

#include <udon/thirdparty/USB_Host_Shield_2.0/PS5USB.h>

#include <udon/com/message/PadPS5.hpp>
#include <udon/stl/optional.hpp>
#include <udon/types/RGB.hpp>

namespace udon
{
    class PadPS5USB
    {
        USB usb;

        PS5USB pad;

        message::PadPS5 buttons;

    public:
        PadPS5USB()
            : usb()
            , pad(&usb)
        {
        }

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

        void update()
        {
            usb.Task();
            if (pad.connected())
            {
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
                buttons.ps    = pad.getButtonPress(ButtonEnum::PS);            // PSボタン

                buttons.analogRightX = +(pad.getAnalogHat(AnalogHatEnum::RightHatX) - 128);  // 0~255 -> -128~127
                buttons.analogRightY = -(pad.getAnalogHat(AnalogHatEnum::RightHatY) - 127);  // 0~255 -> -128~127

                buttons.analogLeftX = +(pad.getAnalogHat(AnalogHatEnum::LeftHatX) - 128);  // 0~255 -> -128~127
                buttons.analogLeftY = -(pad.getAnalogHat(AnalogHatEnum::LeftHatY) - 127);  // 0~255 -> -128~127
            }
            else
            {
                buttons.clear();
            }
        }

        message::PadPS5 getButtons() const
        {
            return buttons;
        }

        void setColor(const udon::RGB& color)
        {
            pad.setLed(color.r, color.g, color.b);
        }
    };
}    // namespace udon
