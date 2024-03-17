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
            .transmitTimeout  = 100,
            .receiveTimeout   = 100,
            .canBaudrate      = 1'000'000,
        }
    };

    bus.begin();
    bus.end();
    bus.update();
    bus.operator bool();
    bus.show();

    Udon::CanNode node{ 0, nullptr, 0, 0 };
    bus.joinTx(node);
    bus.joinRx(
        node, [](void*) {}, nullptr);
    bus.leaveRx(node);
    bus.leaveTx(node);
}

#elif defined(ARDUINO_ARCH_RP2040)

inline void testBus()
{
    Udon::CanBusSpi{
        {
            .channel          = spi0,
            .cs               = 1,
            .interrupt        = 2,
            .mosi             = 3,
            .miso             = 4,
            .sck              = 5,
            .spiClock         = 1'000'000,
            .transmitInterval = 5,
            .transmitTimeout  = 100,
            .receiveTimeout   = 100,
            .canBaudrate      = CAN_1000KBPS,
            .mcpClock         = MCP_16MHZ,
        }
    };

    Udon::CanBusSpi bus{
        {
            .channel   = spi0,
            .cs        = 1,
            .interrupt = 2,
        },
    };

    bus.begin();
    bus.end();
    bus.update();
    bus.operator bool();
    bus.show();

    Udon::CanNode node{ 0, nullptr, 0, 0 };
    bus.joinTx(node);
    bus.joinRx(
        node, [](void*) {}, nullptr);
    bus.leaveRx(node);
    bus.leaveTx(node);
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
    explicit operator bool() const override { return true; }
    void           joinTx(Udon::CanNode&) override {}
    void           joinRx(Udon::CanNode&, void (*)(void*), void*) override {}
    Udon::CanNode* findTx(uint32_t /*id*/, size_t /*length*/ = 0) override { return nullptr; }
    void           leaveTx(const Udon::CanNode&) override {}
    void           leaveRx(const Udon::CanNode&) override {}
};

inline void testReader()
{
    DummyBus             bus;
    Udon::CanReader<int> reader{ bus, 0x00 };
    reader.getMessage();
    reader.show();
    reader.showRaw();
}

inline void testWriter()
{
    DummyBus             bus;
    Udon::CanWriter<int> writer{ bus, 0x00 };
    writer.setMessage(0);
    writer.show();
    writer.showRaw();
}
