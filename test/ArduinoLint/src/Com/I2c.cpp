//
//    構文テスト
// 
//    Copyright (c) 2022-2024 udonrobo
//

#include <Arduino.h>
#include <Udon/Com/I2c.hpp>

#include <Udon/Traits/ReaderWriterTraits.hpp>

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
