#include <Arduino.h>
#include <Udon/Com/Can.hpp>
#include <Udon/Com/Traits.hpp>

#if defined(CORE_TEENSY)

inline void testBus()
{
    Udon::CanBusTeensy<CAN1> bus;

    bus.begin();
    bus.begin(1000000);
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
    Udon::CanBusPico bus{ spi0, 1 };

    bus.begin();
    bus.begin(1000000);
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
    static_assert(Udon::IsReader<Udon::CanReader>::value, "");
    static_assert(Udon::IsWriter<Udon::CanWriter>::value, "");
}

struct DummyBus
    : Udon::ICanBus
{
    explicit operator bool() const override { return true; }
    void joinTx(Udon::CanNode& node) override {}
    void joinRx(Udon::CanNode& node, void (*onReceive)(void*), void* p) override {}
    void leaveTx(const Udon::CanNode& node) override {}
    void leaveRx(const Udon::CanNode& node) override {}
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
