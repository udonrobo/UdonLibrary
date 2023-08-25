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
    Udon::CanBusSpi bus{ spi0, 1 };

    bus.begin(1, 2, 3, 4);
    bus.begin(1);
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
    void joinTx(Udon::CanNode&) override {}
    void joinRx(Udon::CanNode&, void (*)(void*), void*) override {}
    void leaveTx(const Udon::CanNode&) override {}
    void leaveRx(const Udon::CanNode&) override {}
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
