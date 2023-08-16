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
//    OpenSiv3D用 PS5コントローラー
//
//    Controller --> Siv3DApp
//                   ^^^^^^^^
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Com/Message/PadPS5.hpp>
#include <Udon/Stl/Optional.hpp>

namespace Udon
{

    template <typename DummyMessage>
    class SivPadPS5
    {

        Udon::Message::PadPS5 message = {};

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

            if (auto&& gamePad = Gamepad(index))
            {
                message.isConnected = gamePad.isConnected();

                message.triangle = gamePad.buttons.at(3).pressed();
                message.circle   = gamePad.buttons.at(2).pressed();
                message.cross    = gamePad.buttons.at(1).pressed();
                message.square   = gamePad.buttons.at(0).pressed();

                message.up    = gamePad.buttons.at(15).pressed();
                message.right = gamePad.buttons.at(16).pressed();
                message.down  = gamePad.buttons.at(17).pressed();
                message.left  = gamePad.buttons.at(18).pressed();

                message.l1 = gamePad.buttons.at(4).pressed();
                message.r1 = gamePad.buttons.at(5).pressed();
                message.l2 = gamePad.buttons.at(6).pressed();
                message.r2 = gamePad.buttons.at(7).pressed();
                message.l3 = gamePad.buttons.at(10).pressed();
                message.r3 = gamePad.buttons.at(11).pressed();

                message.create = gamePad.buttons.at(8).pressed();
                message.option = gamePad.buttons.at(9).pressed();
                message.touch  = gamePad.buttons.at(13).pressed();

                message.mic = gamePad.buttons.at(14).pressed();

                // -1.0 ~ 1.0 -> -127 ~ 127(int8_t)
                const auto encodeStick = [](double value) -> int8_t
                {
                    return static_cast<int8_t>(value * 127);
                };

                message.analogRightX = +encodeStick(gamePad.axes.at(2));
                message.analogRightY = -encodeStick(gamePad.axes.at(5));

                message.analogLeftX = +encodeStick(gamePad.axes.at(0));
                message.analogLeftY = -encodeStick(gamePad.axes.at(1));
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
            }
        }

        Udon::Optional<Message::PadPS5> getMessage() const
        {
            return message;
        }
    };
}    // namespace Udon
