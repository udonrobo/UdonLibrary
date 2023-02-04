# CAN

# Summary

CAN 通信のコードを簡単に記述できるクラス群です。

-   送信者例

    ```cpp
	#include <CANCommon.hpp>

    CANBusTeensy<CAN1> bus;
    CANWriter<Message::Motor> writer(bus, 0);

    void setup() {
    	bus.begin();
    }
    void loop() {
    	writer.setMessage({ -123 });
    	bus.update();
    }
    ```

-   受信例
    ```cpp
	#include <CANCommon.hpp>

    CANBusTeensy<CAN1> bus;
    CANReader<Message::Motor> reader(bus, 0);

    void setup() {
    	bus.begin();
    }
    void loop() {
    	Serial.println(reader.getMessage().power);
    	bus.update();
    }
    ```

# Document

## [CANBus](./CANBus/README.md)

## [CANReader](./CANReader/README.md)

## [CANWriter](./CANWriter/README.md)
