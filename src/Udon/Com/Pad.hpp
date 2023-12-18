#pragma once

// Arduino
#ifdef ARDUINO

#    include "Can.hpp"
#    include "I2c.hpp"
#    include "Uart.hpp"
#    include "Pad/PadPS5.hpp"

namespace Udon
{
    using CanPadPS5  = PadPS5<CanReader>;
    using I2cPadPS5  = PadPS5<I2cMasterReader>;
    using UartPadPS5 = PadPS5<UartReader>;
}    // namespace Udon
#endif

// OpenSiv3D
#ifdef SIV3D_INCLUDED

#    include "Pad/SivPadPS5.hpp"

namespace Udon
{
    using SivPadPS5 = PadPS5<SivPadPS5Reader>;
}    // namespace Udon

#endif