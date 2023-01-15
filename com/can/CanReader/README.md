# CanReader

## Description

CAN 通信構造体受信クラス

-   対応マイコン

    バスクラスに依存します。

-   制限

    ID 数 : 0~2048 (2^11)

    通信容量 : 0~1785byte (255\*7)

## Data

-   `@date` 2023/01/15
-   `@author` 大河 祐介

# Usage

`CanBusTeensy` バスを使用し、`Message::Motor` 構造体を使用してモーターの情報を受信する例

```cpp
#include "CanCommon.hpp"

CanBusTeensy<CAN1> bus;
CanReader<Message::Motor> reader(bus, 0);

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
	bus.update();
	delay(10);
}
```

## API

-   コンストラクタ

    -   `template<class MessageTy> CanReader::CanReader(BusTy& bus, uint32_t id)`

        `@tparam {MessageTy}` 受信する構造体

        `@param {id}` 監視するノード ID

-   通信監視(WDT)

    -   `CanReader::operator bool()`

        `@return` 通信しているか

-   データ取得

    -   `MessageTy CanReader::getMessage()`

        `@return` 受信したデータ

-   デバッグ出力

    -   `void CanReader::show(char end = {})`

        `@param end` 最後に出力される文字

        セットされた構造体に `show()` メンバが実装されている必要があります。

# Summary

-   data format

	[ id (11bit) ] + [ index (1byte) + data (7byte) ]

	送信量が 7byte を超える場合はパケットに分けて送信