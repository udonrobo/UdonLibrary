# CANReader

## Description

CAN 通信構造体受信クラス

メッセージ構造体オブジェクトを受信できるように `CANNodeInfo` 構造体をラップしたクラスです。

-   対応マイコン

    バス管理クラスに依存します。

-   制限

    ID 数 : 0~2048 (2^11)

    通信容量 : 0~1785byte (255\*7)

## Data

-   `@date` 2023/01/15
-   `@author` 大河 祐介

# Usage

`CANBusTeensy` バスを使用し、`Message::Motor` 構造体を使用してモーターの情報を受信する例

```cpp
#include "CANCommon.hpp"

CANBusTeensy<CAN1> bus;
CANReader<Message::Motor> reader(bus, 0);

void setup() {
	bus.begin();
}

void loop() {
	if (reader)
	{
		Serial.println(reader.getMessage().power);
	}
	else
	{
		Serial.println("error");
	}
	delay(10);
}
```

## API

-   コンストラクタ

    -   `template<class MessageTy> CANReader::CANReader(BusTy& bus, uint32_t id)`

        `@tparam {MessageTy}` 受信する構造体の型

        `@param {id}` 監視するノード ID

-   通信監視(WDT)

    -   `CANReader::operator bool()`

        `@return` 通信しているか

-   データ取得

    -   `MessageTy CANReader::getMessage()`

        `@return` 受信したデータ

-   デバッグ出力

    -   `void CANReader::show(char end = {})`

        `@param end` 最後に出力される文字

        セットされた構造体に `show()` メンバが実装されている必要があります。
