# I2cSlaveReader

## Description

I2c 通信を用いて、マスターに送信するクラス

送信割り込みによって値更新がクラス内部で行われるため、 `update()` 等での更新は不要です

## Data

-   `@date` 2022/09/22
-   `@author` 大河 祐介

# Usage

## API

-   コンストラクタ

    -   `template<size_t N>I2cSlaveWriter(address, clock = 400000UL)`

        通信を開始する

        `@param N` 受信サイズ

        `@param address` アドレス

        `@param clock` 通信クロック

-   送信データセット

    -   `void setArrayData(array[])`

        ```cpp
        const uint8_t send[] = {1, 2, 3, 4};
        sender.setArrayData(send);
        ```

    -   `void setSingleData(index, value)`

    -   `void setByteData(index, value)`

    -   `void setBitData(byteIndex, bitIndex, value)`

    -   `uint8_t& operator[]`

    -   `void clear()`

-   デバッグ出力

    -   `show(end = {}, radix = DEC)`

        `@param end` 最後に出力される文字

        `@param radix` 基数

-   その他

    -   `size()`

        設定受信サイズを取得する

## Example

```cpp
#include "I2cSlaveWriter.h"

I2cSlaveWriter<4> sender(5);

void setup() {
}

void loop() {
	sender.setByteData(0, 123);
	delay(10);
}
```
