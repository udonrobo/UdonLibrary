# I2cSlaveReader

## Description

I2c 通信を用いて、マスターから受信するクラス

## Data

-   `@date` 2022/09/22
-   `@author` 大河 祐介

# Usage

## API

-   コンストラクタ

    -   `template<size_t N>I2cSlaveReader(address, clock = 400000UL)`

        通信を開始する

        `@param N` 受信サイズ

        `@param address` アドレス

        `@param clock` 通信クロック

-   通信監視(WDT)

    -   `bool update(timeOutMs = 50)`

        通信遮断時に復帰させる

        `@param timeOutMs` タイムアウト限界

        `@return` 通信しているか

    -   `bool isConnected(timeOutMs = 50)`

        `@param timeOutMs` タイムアウト限界

        `@return` 通信しているか

    -   `operator bool`

        通信しているかを返す

        ```cpp
        void loop() {
        	if(receiver) {
        		/// 受信しているときの動作
        	}
        }
        ```

-   受信データ取得

    -   `uint8_t getSingleData(index)`

    -   `uint8_t getByteData(index)`

    -   `bool getBitData(byteIndex, bitIndex)`

    -   `int16_t getMotorData(index)`

    -   `const uint8_t& operator[]`

-   デバッグ出力

    -   `show(end = {}, radix = DEC)`

        `@param end` 最後に出力される文字

        `@param radix` 基数

    -   `showMotor(end = {})`

        `@param end` 最後に出力される文字

-   その他

    -   `size()`

        設定受信サイズを取得する

## Example

```cpp
#include "I2cSlaveReader.h"

I2cSlaveReader<4> receiver(5);

void setup() {
	Serial.begin(115200);
}

void loop() {
	if (receiver.update()) {
		Serial.println(receiver.getByteData(0));
	}
	else {
		Serial.println("通信切断");
	}
	delay(10);
}
```
