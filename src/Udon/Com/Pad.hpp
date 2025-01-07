//
//    コントローラー
//
//    Copyright (c) 2022 udonrobo
//

#pragma once

// Arduino
#ifdef ARDUINO

#    include "Can.hpp"
#    include "I2c.hpp"
#    include "Serial.hpp"
#    include "Pad/PadPS5.hpp"
#    include "LoRa.hpp"


namespace Udon
{
    using CanPadPS5 = PadPS5<CanReader>;
    using I2cPadPS5 = PadPS5<I2cMasterReader>;
    using SerialPadPS5 = PadPS5<SerialReader>;

    using E220PadPS5 = PadPS5<E220Reader>;
}    // namespace Udon

#endif


// OpenSiv3D
#ifdef SIV3D_INCLUDED

#    include "Pad/SivPadPS5.hpp"

namespace Udon
{
    using SivPadPS5 = PadPS5<Impl::SivPadPS5Reader>;
}    // namespace Udon

#endif