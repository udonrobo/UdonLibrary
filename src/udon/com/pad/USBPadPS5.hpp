#pragma once

#include <udon/thirdparty/USB_Host_Shield_2.0/PS5BT.h>

#include <udon/com/message/PadPS5.hpp>
#include <udon/stl/optional.hpp>
#include <udon/types/Color.hpp>

namespace udon
{
    class PadPS5USB
    {
        USB usb;

        PS5USB pad;

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
        }

        void setColor(const Color& color)
        {
        }

        udon::optional<message::PadPS5> getMessage()
        {
        }
    };
}    // namespace udon
