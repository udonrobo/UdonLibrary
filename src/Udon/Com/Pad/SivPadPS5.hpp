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

#include <Udon/Com/Pad/IPadPS5.hpp>

namespace Udon
{

    class SivPadPS5
        : public IPadPS5
    {
        size_t index;

    public:
        SivPadPS5()
            : index()
        {
        }

        SivPadPS5(size_t index)
            : index(index)
        {
        }

        void update()
        {
            if (auto&& gamePad = s3d::Gamepad(index))
            {
                IPadPS5::updateFromRawData(
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
                    /* double analogLeftX  */ +gamePad.axes.at(2) * 255,
                    /* double analogLeftY  */ -gamePad.axes.at(5) * 255,
                    /* double analogRightX */ +gamePad.axes.at(0) * 255,
                    /* double analogRightY */ -gamePad.axes.at(1) * 255);
            }
            else
            {
                IPadPS5::clear();
            }
        }
    };
}    // namespace Udon
