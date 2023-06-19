#pragma once

#ifdef ARDUINO
#    include <udon/com/uart/UartReader.hpp>
#    include <udon/com/uart/UartWriter.hpp>
#endif

#ifdef SIV3D_INCLUDED
#    include <udon/com/uart/SivUartReader.hpp>
#    include <udon/com/uart/SivUartWriter.hpp>
#endif
