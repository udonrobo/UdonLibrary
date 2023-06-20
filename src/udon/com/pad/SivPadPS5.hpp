//-----------------------------------------------
//
//  UdonLibrary
//
//  Copyright (c) 2022-2023 Okawa Yusuke
//  Copyright (c) 2022-2023 udonrobo
//
//  Licensed under the MIT License.
//
//-----------------------------------------------
//
//  OpenSiv3D用 PS5コントローラー
//
//-----------------------------------------------

#pragma once

#include <udon/com/message/PadPS5.hpp>

namespace udon
{

    template <typename DummyMessage>
    class SivPadPS5
    {

        udon::message::PadPS5 message = {};

        size_t index = 0;

    public:
        SivPadPS5() = default;

        SivPadPS5(size_t index)
            : index(index)
        {
        }

        explicit operator bool() const
        {
            return message.isConnected;
        }

        void update()
        {

            if (auto&& pad = Gamepad(index))
            {
                message.isConnected = pad.isConnected();

                message.triangle = pad.buttons.at(3).pressed();
                message.circle   = pad.buttons.at(2).pressed();
                message.cross    = pad.buttons.at(1).pressed();
                message.square   = pad.buttons.at(0).pressed();

                message.up    = pad.buttons.at(15).pressed();
                message.right = pad.buttons.at(16).pressed();
                message.down  = pad.buttons.at(17).pressed();
                message.left  = pad.buttons.at(18).pressed();

                message.l1 = pad.buttons.at(4).pressed();
                message.r1 = pad.buttons.at(5).pressed();
                message.l2 = pad.buttons.at(6).pressed();
                message.r2 = pad.buttons.at(7).pressed();
                message.l3 = pad.buttons.at(10).pressed();
                message.r3 = pad.buttons.at(11).pressed();

                message.create = pad.buttons.at(8).pressed();
                message.option = pad.buttons.at(9).pressed();
                message.touch  = pad.buttons.at(13).pressed();

                message.mic = pad.buttons.at(14).pressed();

                // -1.0 ~ 1.0 -> -127 ~ 127(int8_t)
                const auto encodeStick = [](double value) -> int8_t
                {
                    return static_cast<int8_t>(value * 127);
                };

                message.analogRightX = +encodeStick(pad.axes.at(2));
                message.analogRightY = -encodeStick(pad.axes.at(5));

                message.analogLeftX = +encodeStick(pad.axes.at(0));
                message.analogLeftY = -encodeStick(pad.axes.at(1));

                // -1.0 ~ 1.0 -> 0 ~ 255
                message.analogR2 = static_cast<int8_t>((pad.axes.at(4) + 1) * 255);
                message.analogL2 = static_cast<int8_t>((pad.axes.at(3) + 1) * 255);
            }
            else
            {
                message.isConnected = false;

                message.triangle = false;
                message.circle   = false;
                message.cross    = false;
                message.square   = false;

                message.up    = false;
                message.right = false;
                message.down  = false;
                message.left  = false;

                message.l1 = false;
                message.r1 = false;
                message.l2 = false;
                message.r2 = false;
                message.l3 = false;
                message.r3 = false;

                message.create = false;
                message.option = false;
                message.touch  = false;

                message.mic = false;

                message.analogRightX = 0;
                message.analogRightY = 0;
                message.analogLeftX  = 0;
                message.analogLeftY  = 0;
                message.analogR2     = 0;
                message.analogL2     = 0;
            }
        }

        const udon::message::PadPS5& getMessage() const
        {
            return message;
        }
    };
}    // namespace udon
