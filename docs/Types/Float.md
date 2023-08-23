# 固定ビット長浮動小数点型

`double` `float` のような組み込み浮動小数点型のバイト数は環境によって異なります。

そこで環境によってサイズが変わらない浮動小数点型 `Udon::float~~_t` を提供します。

異なる環境で浮動小数点の通信をする際等に使用します。

## Usage

### インクルード

```cpp
#include <Udon/Types/Float.hpp>
```

### 定義される型

- `Udon::float16_t` 2 バイト浮動小数点型

- `Udon::float32_t` 4 バイト浮動小数点型

- `Udon::float64_t` 8 バイト浮動小数点型

- `Udon::float128_t` 16 バイト浮動小数点型

環境によって定義されないことがあります。定義されている場合 `UDON_HAS_FLOAT~~` マクロが定義されます。