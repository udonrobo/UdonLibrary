#include <Arduino.h>
#include <Udon/Com/Can.hpp>

#include <Udon/Traits/ReaderWriterTraits.hpp>

#if defined(TEENSYDUINO)

inline void testBus()
{
    Udon::CanBusTeensy<CAN1> a;
    Udon::CanBusTeensy<CAN1> bus{
        {
            .transmitInterval = 5,
            .transmitTimeout = 100,
            .receiveTimeout = 100,
            .canBaudrate = 1'000'000,
        }
    };

    bus.begin();
    bus.end();
    bus.update();
    bus.operator bool();
    bus.show();

    Udon::CanTxNode nodeTx{ 0, { 0 }, 0 };
    Udon::CanRxNode nodeRx{ 0, { 0 }, nullptr, nullptr, 0 };

    bus.createTx(0, 0);
    bus.createRx(0, 0);
}

#elif defined(ARDUINO_ARCH_RP2040)

inline void testBus()
{
    Udon::CanBusSpi{
        {
            .channel = spi0,
            .cs = 1,
            .interrupt = 2,
            .mosi = 3,
            .miso = 4,
            .sck = 5,
            .spiClock = 1'000'000,
            .transmitInterval = 5,
            .transmitTimeout = 100,
            .receiveTimeout = 100,
            .canBaudrate = CAN_1000KBPS,
            .mcpClock = MCP_16MHZ,
        }
    };

    Udon::CanBusSpi bus{
        {
            .channel = spi0,
            .cs = 1,
            .interrupt = 2,
        },
    };

    bus.begin();
    bus.end();
    bus.update();
    bus.operator bool();
    bus.show();

    Udon::CanTxNode nodeTx{ 0, { 0 }, 0 };
    Udon::CanRxNode nodeRx{ 0, { 0 }, nullptr, nullptr, 0 };
    bus.createTx(0, 0);
    bus.createRx(0, 0);
}

#endif

inline void testIsReaderWriter()
{
    static_assert(Udon::Traits::IsReader<Udon::CanReader>::value, "");
    static_assert(Udon::Traits::IsWriter<Udon::CanWriter>::value, "");
}

struct DummyBus
    : Udon::ICanBus
{
    explicit operator bool() const { return true; }
    Udon::CanTxNode* createTx(uint32_t, size_t) override { return nullptr; }
    Udon::CanRxNode* createRx(uint32_t, size_t) override { return nullptr; }
};

inline void testReader()
{
    DummyBus bus;
    Udon::CanReader<int> reader{ bus, 0x00 };
    reader.getMessage();
    reader.show();
    reader.showRaw();
}

inline void testWriter()
{
    DummyBus bus;
    Udon::CanWriter<int> writer{ bus, 0x00 };
    writer.setMessage(0);
    writer.show();
    writer.showRaw();
}
