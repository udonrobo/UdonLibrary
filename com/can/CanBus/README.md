# Can Bus

## CanBusTeensy

teensy 内臓 CAN コントローラーを使用して CAN 通信を行うバスクラス

CanReader CanWriter クラスと組み合わせてデータのやり取りを行います。

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

    - Reader, Writer

        `CanReader`, `CanWriter` インスタンスをバスに参加させるには `join` メンバ関数を使用します。開放するには `detach` メンバ関数を使用します。

        - `void join(CanReader& r)`
        - `void join(CanWriter& r)`

        - `void detach(CanReader& r)`
        - `void detach(CanWriter& r)`

        ```cpp
        CanBusTeensy<CAN1> bus;
        CanWriter<Message::Motor> reader(0);

        void setup() {
            bus.join(reader);
        }
        ```

    -   通信更新

        -   `void CanBusTeensy::update(uint32_t writeIntervalUs = 5000)`

            `@param {writeIntervalUs}` 送信間隔

            バスに大量のデータが流れないよう、ループ周期に関係なく設定間隔以下で送信されないようになっています。

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

        -   `void CanBusSpi::begin(CAN_SPEED baudrate = CAN_1000KBPS, CAN_CLOCK canClock = MCP_20MHZ)`

            `@param {baudrate}` CAN 通信レート [ドキュメント](https://github.com/autowp/arduino-mcp2515) 参照

            `@param {canClock}` CAN 通信クロック周波数 [ドキュメント](https://github.com/autowp/arduino-mcp2515) 参照

    - Reader, Writer

        `CanReader`, `CanWriter` インスタンスをバスに参加させるには `join` メンバ関数を使用します。開放するには `detach` メンバ関数を使用します。

        - `void join(CanReader& r)`
        - `void join(CanWriter& r)`

        - `void detach(CanReader& r)`
        - `void detach(CanWriter& r)`

        ```cpp
        CanBusTeensy<CAN1> bus;
        CanWriter<Message::Motor> reader{ 0 };

        void setup() {
            bus.join(reader);
        }
        ```

    -   通信更新

        -   `void CanBusTeensy::update(uint32_t writeIntervalUs = 5000)`

            `@param {writeIntervalUs}` 送信間隔

            バスに大量のデータが流れないよう、ループ周期に関係なく設定間隔以下で送信されないようになっています。

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
