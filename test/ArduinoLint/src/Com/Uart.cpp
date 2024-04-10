#include <Arduino.h>
#include <Udon/Com/Uart.hpp>

inline void test()
{
    Udon::UartReader<int> reader{ Serial };
    reader.getMessage();
    reader.show();
    reader.showRaw();

    Udon::UartWriter<int> writer{ Serial };
    writer.setMessage(0);
    writer.show();
    writer.showRaw();
}
