# Can Bus

## CanBusTeensy

teensy 内臓 CAN コントローラーを使用して CAN 通信を行うバスクラス

```mermaid
flowchart LR
	CanBusTeensy <--CAN TX/RX--> CANトランシーバー <--CAN H/L--> BUS
```

-   依存ライブラリ

    [FlexCAN_T4](https://github.com/tonton81/FlexCAN_T4)

    [IntervalTimer](https://github.com/loglow/IntervalTimer)

-   API

    -   コンストラクタ

        -   `template<CAN_DEV_TABLE Bus> CanBusTeensy::CanBusTeensy()`

            `@tparam {Bus}` バス種類 (CAN0,CAN1,CAN2,CAN3) [ドキュメント](https://github.com/tonton81/FlexCAN_T4) 参照

    -   通信開始

        -   `void CanBusTeensy::begin(uint32_t baudrate = 1000000)`

            `@param {baudrate}` CAN 通信レート

    -   通信更新

        `void CanBusTeensy::update()`

-   Sample

```cpp
CanBusTeensy<CAN1> bus;
void setup() {
    bus.begin();
}
void loop() {
    bus.update();
}
```

## CanBusSpi

外付け CAN コントローラーを使用して CAN 通信を行うバスクラス

```mermaid
flowchart LR
	CanBusSpi <--SPI--> CANコントローラー <--CAN TX/RX--> CANトランシーバー <--CAN H/L--> BUS
```

-   依存ライブラリ

    [arduino-mcp2515](https://github.com/autowp/arduino-mcp2515)

-   API

    -   コンストラクタ

        -   `template<uint8_t Cs, uint8_t Interrupt> CanBusSpi::CanBusSpi(SPIClass& spi, uint32_t spiClock = 10000000)`

            `@tparam {Cs}` SPI chip select ピン

            `@tparam {Interrupt}` CAN コントローラー Int 端子接続ピン

            `@param {spi}` SPI クラスインスタンス

            `@param {spiClock}` SPI クロック

    -   通信開始

        -   `void CanBusSpi::begin(CAN_SPEED baudrate = CAN_1000KBPS)`

            `@param {baudrate}` CAN 通信レート [ドキュメント](https://github.com/autowp/arduino-mcp2515) 参照

    -   通信更新

        `void CanBusSpi::update()`

-   Sample

```cpp
CanBusSpi<9, 2> bus(SPI);
void setup() {
    bus.begin();
}
void loop() {
    bus.update();
}
```
