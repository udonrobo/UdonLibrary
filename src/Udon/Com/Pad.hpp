#pragma once

// Arduino
#ifdef ARDUINO

#    include "Can.hpp"
#    include "I2c.hpp"
#    include "Uart.hpp"
#    include "Pad/PadPS5.hpp"
#    include "LoRa.hpp"


namespace Udon
{
    using CanPadPS5 = PadPS5<CanReader>;
    using I2cPadPS5 = PadPS5<I2cMasterReader>;
    using UartPadPS5 = PadPS5<UartReader>;

#    if __cplusplus >= 201402L
    // E220Reader は C++14 以上の機能を使用しているため、C++14 以上でのみ使用可能
    using E220PadPS5 = PadPS5<E220Reader>;
#    endif

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