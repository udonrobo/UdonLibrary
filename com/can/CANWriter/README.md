# CANWriter

## Description

CAN 通信構造体送信クラス

-   対応マイコン

    バスクラスに依存します。

-   制限

    ID 数 : 0~2048 (2^11)

    通信容量 : 0~1785byte (255\*7)

## Data

-   `@date` 2023/01/15
-   `@author` 大河 祐介

# Usage

`CANBusTeensy` バスを使用し、 `Message::Motor` 構造体を使用してモーターの情報を送信する例

```cpp
#include "CANCommon.hpp"

CANBusTeensy<CAN1> bus;
CANWriter<Message::Motor> writer(bus, 0);

void setup() {
	bus.begin();
}

void loop() {

	Message::Motor msg;
	msg.power = -123;
	writer.setMessage(msg);

	bus.update();
	delay(10);

}
```

## API

-   コンストラクタ

    -   `template<class MessageTy> CANWriter::CANWriter(BusTy& bus, uint32_t id)`

        `@tparam {MessageTy}` 送信する構造体

        `@param {id}` 自身のノード ID

-   通信監視(WDT)

    -   `CANWriter::operator bool()`

        `@return` 通信しているか

        戻り値が `false` である場合バスクラスの送信処理が行われていない可能性が考えられます。

-   データセット

    -   `void CANWriter::setMessage(MessageTy message)`

        `@param {message}` 送信するデータ

-   デバッグ出力

    -   `void CANWriter::show(char end = {})`

        `@param end` 最後に出力される文字

        セットされた構造体に `show()` メンバが実装されている必要があります。
