#include <Arduino.h>
#include <Udon/Com/I2c.hpp>

#include <Udon/Traits/IsReader.hpp>
#include <Udon/Traits/IsWriter.hpp>

inline void testBus()
{
    Udon::I2cBus bus{ Wire };

    bus.update();
    bus.show();
    bus.begin();
    bus.begin(0x00);
    bus.end();
    bus.restart();
    bus.setClock(0);
    bus.beginTransmission(0x00);
    bus.endTransmission();
    bus.endTransmission(true);
    bus.requestFrom(0x00, 0);
    bus.requestFrom(0x00, 0, true);
    bus.write(0);
    bus.write(nullptr, 0);
    bus.available();
    bus.read();
    bus.peek();
    bus.flush();
    bus.onReceive(nullptr);
    bus.onRequest(nullptr);
}

inline void testIsReaderWriter()
{
    static_assert(Udon::Traits::IsReader<Udon::I2cMasterReader>::value, "");
    static_assert(Udon::Traits::IsWriter<Udon::I2cMasterWriter>::value, "");
    static_assert(Udon::Traits::IsReader<Udon::I2cSlaveReader>::value, "");
    static_assert(Udon::Traits::IsWriter<Udon::I2cSlaveWriter>::value, "");
}

inline void testMaster()
{
    Udon::I2cBus bus{ Wire };

    Udon::I2cMasterReader<int> reader{ bus, 0x00 };
    reader.update();
    reader.getMessage();
    reader.show();
    reader.showRaw();

    Udon::I2cMasterWriter<int> writer{ bus, 0x00 };
    writer.update();
    writer.setMessage(0);
    writer.show();
    writer.showRaw();
}

inline void testSlave()
{
    Udon::I2cBus bus{ Wire };

    Udon::I2cSlaveReader<int> reader{ bus };
    reader.getMessage();
    reader.show();
    reader.showRaw();

    Udon::I2cSlaveWriter<int> writer{ bus };
    writer.setMessage(0);
    writer.show();
    writer.showRaw();
}

inline void testArrayElementReader()
{
    Udon::I2cBus bus{ Wire };

    Udon::I2cMasterReader<int[10]> reader{ bus, 0x00 };
    Udon::ArrayElementReader<int> arrayElementReader{ reader.at(0) };

    arrayElementReader.getMessage();
}

inline void testArrayElementWriter()
{
    Udon::I2cBus bus{ Wire };

    Udon::I2cMasterWriter<int[10]> writer{ bus, 0x00 };
    Udon::ArrayElementWriter<int> arrayElementWriter{ writer.at(0) };

    arrayElementWriter.setMessage(0);
}
