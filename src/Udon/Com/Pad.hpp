#pragma once

#include "Pad/PadPS5.hpp"

#include "Can.hpp"
#include "I2c.hpp"
#include "Uart.hpp"

namespace Udon
{
    using CanPadPS5 = PadPS5<CanReader>;
    using I2cPadPS5 = PadPS5<I2cMasterReader>;
    using UartPadPS5 = PadPS5<UartReader>;
}
