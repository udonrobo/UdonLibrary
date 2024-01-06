//
//    OpenSiv3D用 PS5コントローラー
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Controller --> Siv3DApp
//                   ^^^^^^^^
//

#include "PadPS5.hpp"

#pragma once

namespace Udon
{

    template <typename DummyMessage>
    class SivPadPS5Reader
    {
        size_t index;

    public:
        SivPadPS5Reader() noexcept
            : index()
        {
        }

        SivPadPS5Reader(size_t index) noexcept
            : index(index)
        {
        }

        Message::PadPS5 toMessage() const noexcept
        {
            if (auto&& gamePad = s3d::Gamepad(index))
            {
                return {
                    /* bool   isConnected  */ gamePad.isConnected(),
                    /* bool   triangle     */ gamePad.buttons.at(3).pressed(),
                    /* bool   circle       */ gamePad.buttons.at(2).pressed(),
                    /* bool   cross        */ gamePad.buttons.at(1).pressed(),
                    /* bool   square       */ gamePad.buttons.at(0).pressed(),
                    /* bool   up           */ gamePad.buttons.at(15).pressed(),
                    /* bool   right        */ gamePad.buttons.at(16).pressed(),
                    /* bool   down         */ gamePad.buttons.at(17).pressed(),
                    /* bool   left         */ gamePad.buttons.at(18).pressed(),
                    /* bool   l1           */ gamePad.buttons.at(4).pressed(),
                    /* bool   r1           */ gamePad.buttons.at(5).pressed(),
                    /* bool   l2           */ gamePad.buttons.at(6).pressed(),
                    /* bool   r2           */ gamePad.buttons.at(7).pressed(),
                    /* bool   l3           */ gamePad.buttons.at(10).pressed(),
                    /* bool   r3           */ gamePad.buttons.at(11).pressed(),
                    /* bool   create       */ gamePad.buttons.at(8).pressed(),
                    /* bool   option       */ gamePad.buttons.at(9).pressed(),
                    /* bool   touch        */ gamePad.buttons.at(13).pressed(),
                    /* bool   mic          */ gamePad.buttons.at(14).pressed(),
                    /* bool   ps           */ false,    // TODO: 未実装
                    /* double analogLeftX  */ Map(+gamePad.axes.at(0), -1, 1, -128, 127),
                    /* double analogLeftY  */ Map(-gamePad.axes.at(1), -1, 1, -128, 127),
                    /* double analogRightX */ Map(+gamePad.axes.at(2), -1, 1, -128, 127),
                    /* double analogRightY */ Map(-gamePad.axes.at(5), -1, 1, -128, 127),
                };
            }
            else
            {
                return {};
            }
        }
    };

    using SivPadPS5 = PadPS5<SivPadPS5Reader>;
}    // namespace Udon
