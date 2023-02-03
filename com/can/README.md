# CAN

# Summary

CAN 通信のコードを簡単に記述できるクラス群です。

-   送信者例

    ```cpp
	#include <CanCommon.hpp>

    CanBusTeensy<CAN1> bus;
    CanWriter<Message::Motor> writer(bus, 0);

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
	#include <CanCommon.hpp>

    CanBusTeensy<CAN1> bus;
    CanReader<Message::Motor> reader(bus, 0);

    void setup() {
    	bus.begin();
    }
    void loop() {
    	reader.show('\n');
    	bus.update();
    }
    ```

# Document

## [CanBus](./CanBus/README.md)

## [CanReader](./CanReader/README.md)

## [CanWriter](./CanWriter/README.md)
