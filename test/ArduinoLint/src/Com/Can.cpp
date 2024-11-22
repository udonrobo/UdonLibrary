//
//    構文テスト
//
//    Copyright (c) 2022-2024 udonrobo
//

#include <Arduino.h>
#include <Udon/Com/Can.hpp>

#include <Udon/Traits/ReaderWriterTraits.hpp>

#if defined(TEENSYDUINO)

__attribute__((unused)) static void testBus()
{

#    if defined(ARDUINO_TEENSY41) || defined(ARDUINO_TEENSY40) || defined(ARDUINO_TEENSY36)
    constexpr auto BUS = CAN1;
#    elif defined(ARDUINO_TEENSY35)
    constexpr auto BUS = CAN0;
#    endif

    Udon::CanBusTeensy<BUS> a;
    Udon::CanBusTeensy<BUS> bus{
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

__attribute__((unused)) static void testBus()
{
    Udon::CanBusSpi{
        {
            .channel = spi0,
            .cs = 1,
            .mosi = 3,
            .miso = 4,
            .sck = 5,
            .interrupt = 2,
            .spiClock = 1'000'000,
            .transmitInterval = 5,
            .transmitTimeout = 100,
            .receiveTimeout = 100,
            .canBaudrate = CAN_1000KBPS,
            .mcpClock = MCP_16MHZ,
        }
    };

    Udon::CanBusSpi bus;

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

__attribute__((unused)) static void testIsReaderWriter()
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

__attribute__((unused)) static void testReader()
{
    DummyBus bus;
    Udon::CanReader<int> reader{ bus, 0x00 };
    reader.getMessage();
    reader.show();
    reader.showRaw();
}

__attribute__((unused)) static void testWriter()
{
    DummyBus bus;
    Udon::CanWriter<int> writer{ bus, 0x00 };
    writer.setMessage(0);
    writer.show();
    writer.showRaw();
}
