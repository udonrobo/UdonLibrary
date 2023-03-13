# Can Bus

バス管理クラス

バス管理クラス単体ではデータの読み取り、書き込みはできません。

`CanNodeInfo` 構造体と組み合わせて、通信データの読み取り書き込みを行います。

## Common

<details>
<summary>CanNodeInfo</summary>

各ノードを管理する構造体です。以下のメンバを持ちます。

```cpp
uint32_t id
uint8_t* buffer
size_t   length
uint32_t timestampUs
```

`id` : 自身の ID もしくは監視する ID

`buffer` : 送信、受信バッファをさすポインタ

`length` : 送信、受信バッファ長

`timestampUs` : 最後に送信、受信バッファにアクセスした時間[μs]

</details>

<details>
<summary>CanBusInterface</summary>

バス管理クラスを一律に使用できるようにするインターフェースクラスです。

バス管理クラスは全てこのクラスを継承し、仮想関数をオーバーライドします。

使用するマイコンによってバス管理クラス(型)が切り替わっても、このクラスが基底クラスであるため、型を変えずにバスオブジェクトを保持することができます。そのため、マイコンが異なっていても、同じコードで `CanNodeInfo` のラッパークラス等を作成することができます。

-   API

    ノードをバスに参加させる

    -   `virtual joinTX(CanNodeInfo& node)`
    -   `virtual joinRX(CanNodeInfo& node)`

    ノードをバスから切り離す

    -   `virtual detachTX(CanNodeInfo& node)`
    -   `virtual detachRX(CanNodeInfo& node)`

-   Sample

    ```cpp
    uint8_t buffer[10];
    CanNodeInfo node {
        /*uint32_t id         */ 0            ,
        /*uint8_t* buffer     */ buffer       ,
        /*size_t   length     */ sizeof buffer,
        /*uint32_t timestampUs*/ 0            ,
    };

    CanBus---- bus;

    void setup() {
        bus.joinTX(node);
    }
    void loop() {}
    ```

    ```cpp
    /// @brief 可変長データを送受信する CanNodeInfo ラッパークラス
    class CanWriterVLA
    {
            // 全バス管理クラスがCanBusInterfaceを継承しているので、このように全バス管理クラスの参照を一律に保持できます。
            CanBusInterface& bus ;
            CanNodeInfo      info;
        public:
            CanWriterVLA(CanBusInterface& bus, uint32_t id)
                : bus(bus)
                , info(
                    /*uint32_t id         */ id     ,
                    /*uint8_t* buffer     */ nullptr,
                    /*size_t   length     */ 0      ,
                    /*uint32_t timestampUs*/ 0      ,
                )
            {
                bus.joinTX(info);
            }
            ~CanWriterVLA()
            {
                bus.detachTX(info);
                delete[] info.buffer;
            }
            void resize(size_t n)
            {
                delete[] info.buffer;
                info.buffer = new uint8_t[n]();
                info.length = n;
                // バス管理クラスは CanNodeInfo インスタンスのポインタを内部に保持しています。そのためインスタンスを書き換えると、バス管理クラスにも書き換えが反映されます。
            }
    };

    CanBusTeensy<Can1> mainBus    ;
    CanBusSPI<1, 2>    subBus(SPI);

    CanWriterVLA writer0(mainBus, 0);
    CanWriterVLA writer1(subBus , 0);
    ```

</details>

## Bus Class

<details>
<summary>CanBusTeensy</summary>

teensy 内臓 Can コントローラーを使用して、 Can 通信を行うバス管理クラス

```mermaid
flowchart LR

	bus[Can BUS] --- |Can H/L|Canトランシーバ --- |Can TX/RX|lib

    subgraph Soft

        subgraph CanBusTeensy
            direction RL
            lib[FlexCan_T4]
            CanBusInterface
        end
        lib --- |transform|CanBusInterface

        CanBusInterface --- reader

        CanBusInterface --- writer

        subgraph Reader
            reader[CanNodeInfo]
        end
        subgraph Writer
            writer[CanNodeInfo]
        end

    end

```

-   依存ライブラリ

    [FlexCan_T4](https://github.com/tonton81/FlexCan_T4)

    [IntervalTimer](https://github.com/loglow/IntervalTimer)

-   API

    コンストラクタ

    -   `template<Can_DEV_TABLE Bus> CanBusTeensy::CanBusTeensy()`

        `@tparam {Bus}` バス種類 (Can0,Can1,Can2,Can3) [ライブラリドキュメント](https://github.com/tonton81/FlexCan_T4) 参照

    通信開始

    -   `void CanBusTeensy::begin(uint32_t baudrate = 1000000)`

        `@param {baudrate}` Can 通信レート

    通信更新

    -   `void CanBusTeensy::update(uint32_t writeIntervalUs = 5000)`

        `@param {writeIntervalUs}` 送信間隔

        バスに大量のデータが流れないよう、ループ周期に関係なく設定間隔以下で送信されないようになっています。

-   メモリ

    CanNodeInfo のインスタンスを指すポインタをリストとしてヒープ領域に保持します。

-   Sample

    ノード ID0 が送信しているデータを取得する。

    ```cpp
    uint8_t buffer[10];
    CanNodeInfo node {
        /*uint32_t id         */ 0            ,
        /*uint8_t* buffer     */ buffer       ,
        /*size_t   length     */ sizeof buffer,
        /*uint32_t timestampUs*/ 0            ,
    };

    CanBusTeensy<Can1> bus;
    void setup()
    {
        bus.joinRX(node);  // ノードをバスに参加させる(複数参加させることもできます)
        bus.begin();       // 通信開始
        Serial.begin(115200);
    }

    void loop()
    {
        bus.update();
        for(size_t i = 0; i < node.length; ++i)
        {
            Serial.print(node.buffer[i]);
        }
        Serial.println();
    }
    ```

    生で `CanNodeInfo` を使うと少し複雑になります。そのため `CanNodeInfo` のラッパークラスである、`CanReader`, `CanWriter` クラス等を使います。ラッパークラス

-   開発者

    大河祐介

</details>

<details>
<summary>CanBusSPI</summary>
外付け Can コントローラーを使用して Can 通信を行うバス管理クラス

```mermaid
flowchart LR

	bus[Can BUS] --- |Can H/L|Canトランシーバ --- |Can TX/RX|Canコントローラ --- |SPI|lib

    subgraph Soft

        subgraph CanBusSPI
            direction RL
            lib[arduino-mcp2515]
            CanBusInterface
        end

        lib --- |transform|CanBusInterface

        CanBusInterface --- reader

        CanBusInterface --- writer

        subgraph Reader
            reader[CanNodeInfo]
        end
        subgraph Writer
            writer[CanNodeInfo]
        end

    end

```

-   依存ライブラリ

    [arduino-mcp2515](https://github.com/autowp/arduino-mcp2515)

-   API

    コンストラクタ

    -   `template<uint8_t CS, uint8_t Interrupt> CanBusSPI::CanBusSPI(SPIClass& SPI, uint32_t SPIClock = 10000000)`

        `@tparam {CS}` SPI chip select ピン

        `@tparam {Interrupt}` Can コントローラー Int 端子接続ピン

        `@param {SPI}` SPI クラスインスタンス

        `@param {SPIClock}` SPI クロック

    通信開始

    -   `void CanBusSPI::begin(Can_CLOCK CanClock = MCP_20MHZ, Can_SPEED baudrate = Can_1000KBPS)`

        `@param {baudrate}` Can 通信レート [ライブラリドキュメント](https://github.com/autowp/arduino-mcp2515) 参照

        `@param {CanClock}` Can コントローラー動作周波数 [ライブラリドキュメント](https://github.com/autowp/arduino-mcp2515) 参照

    通信更新

    -   `void CanBusTeensy::update(uint32_t writeIntervalUs = 5000)`

        `@param {writeIntervalUs}` 送信間隔

        バスに大量のデータが流れないよう、ループ周期に関係なく設定間隔以下で送信されないようになっています。

-   Sample

    ```cpp
    CanBusSPI<9, 2> bus(SPI);
    void setup() {
        bus.begin();
    }
    void loop() {
        bus.update();
    }
    ```

-   開発者

    大河祐介、、、

</details>

<details>
<summary>バス管理クラス開発について</summary>

今後マイコンが変わった際など、バス管理クラスを新たに作成することがあるかと思います。開発待ってます！！

-   留意点

    `CanBusInterface` クラスを継承してください。`CanNodeInfo` のラッパークラスは、バス管理クラスが`CanBusInterface` クラスを継承している前提で作られているためです。また`CanBusInterface` クラスの仮想関数をオーバーライドしてください。

    バス管理クラスごとにファイル分割して、`CanBus.hpp` から include してください。

    プリプロセッサーを使って、特定のマイコンの場合にバス管理クラスが有効になるようにしてください。

    ```cpp
    #if defined(__MK20DX256__) || defined(__MK64FX512__) ...
    class CanBus--- {
    };
    #endif
    ```

-   template

    コールバック関数型

    ```cpp

    #pragma once

    #if defined() || defined() || defined() || ...

    #	include "list.hpp"  // udon::std::list

    #	include "CanInfo.hpp"
    #	include "CanBusInterface.hpp"

    class CanBus : public CanBusInterface
    {

            using DataLine = udon::std::list<CanNodeInfo*>;
            DataLine        tx   ;
            DataLine        rx   ;
            CanBusErrorInfo error;

            static CanBus* self;

        public:

            CanBus()
                : bus   {}
                , tx    {}
                , rx    {}
                , error {}
            {
                self = this;
            }

            ~CanBus()
            {
                end();
            }

            /// @brief 通信開始
            /// @param {baudrate} 通信レート
            void begin(const uint32_t baudrate = 1000000)
            {
                if (rx.size() || tx.size())
                {
                    bus.begin();
                }
                if (rx.size())
                {
                    bus.onReceive(eventRX);
                }
            }

            /// @brief 通信終了
            void end()
            {
                bus.end();
            }

            /// @brief バス更新
            /// @param {writeIntervalUs} 送信間隔
            void update(uint32_t writeIntervalUs = 5000)
            {
                if (tx.size() && micros() - error.timestampUs >= writeIntervalUs)
                {
                    eventTX();
                }
            }

            /// @brief エラー情報取得
            /// @return CanBusErrorInfo構造体インスタンス
            CanBusErrorInfo getErrorInfo() const
            {
                return error;
            }

            /// @brief TXノードをバスに参加
            /// @param node
            void joinTX(CanNodeInfo& node) override
            {
                join(tx, node);
            }

            /// @brief RXノードをバスに参加
            /// @param node
            void joinRX(CanNodeInfo& node) override
            {
                join(rx, node);
            }

            /// @brief RXノードをバスから解放
            /// @param node
            void detachRX(CanNodeInfo& node) override
            {
                detach(rx, node);
            }

            /// @brief TXノードをバスから解放
            /// @param node
            void detachTX(CanNodeInfo& node) override
            {
                detach(tx, node);
            }

        private:

            /// @brief ライブラリクラスからバスのエラー情報を取得
            static CanBusErrorInfo getError()
            {
                const auto e = bus.getError();
                return {
                    e.TXErrorCounter,
                    e.RXErrorCounter,
                    micros()
                };
            }

            static void eventRX(const Can_message_t& msg)
            {
                const auto event = [&msg](CanNodeInfo * node)
                {
                    const uint8_t index = msg.buf[0];   // 先頭1バイト : パケット番号
                    for (uint8_t i = 0; i < 7; i++)     // 8バイト受信データをバイト列にデコード
                    {
                        const uint8_t bufIndex = i + index * 7;
                        if (bufIndex < node->length)
                            node->buffer[bufIndex] = msg.buf[i + 1];
                        else
                            break;
                    }
                    node->timestampUs = micros();
                };
                for (auto && it : self->rx)
                {
                    if (msg.id == it->id)
                    {
                        event(it);
                    }
                }
                self->error = getError();
            }

            static void eventTX()
            {
                const auto event = [](CanNodeInfo * node)
                {
                    // 一度に8バイトしか送れないため、パケットに分割し送信
                    for (size_t index = 0; index < ceil(node->length / 7.0); index++)
                    {
                        Can_message_t msg;
                        msg.id = node->id;
                        msg.buf[0] = index;  // 先頭1バイト : パケット番号
                        for (uint8_t i = 0; i < 7; i++)  // バイト列を8バイト受信データにエンコード
                        {
                            const uint8_t bufIndex = i + index * 7;
                            if (bufIndex < node->length)
                                msg.buf[i + 1] = node->buffer[bufIndex];
                            else
                                break;
                        }
                        self->bus.write(msg);
                    }
                    node->timestampUs = micros();
                };
                for (auto && it : self->tx) {
                    event(it);
                }
                self->error = getError();
            }

            void join(DataLine& line, CanNodeInfo& node)
            {
                for (auto && it : line)
                {
                    if (it == &node)  // インスタンスの重複を除外する
                    {
                        return;
                    }
                }
                line.push_back(&node);
            }

            void detach(DataLine& line, CanNodeInfo& node)
            {
                for (auto && it = line.begin(); it != line.end(); ++it)
                {
                    if (*it == &node)
                    {
                        line.erase(it);
                        break;
                    }
                }
            }

    };

    template<Can_DEV_TABLE Bus>
    CanBus<Bus>* CanBusTeensy<Bus>::self;

    #endif

    ```

</details>
