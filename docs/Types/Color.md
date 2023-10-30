# 色空間

色空間を表現するには `Udon::RGB`、`Udon::HSV` クラスを使用します。

```cpp
#include <Udon/Types/Color.hpp>
```

## RGB 色空間

赤緑青 の光の三原色で表現される色空間のことで、次のように定義されています。各要素には 0~255 の数値が入ります。

```cpp
struct RGB
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};
```

### 構築

```cpp
Udon::RGB rgb;
Udon::RGB rgb{ 0, 100, 255 };
Udon::RGB rgb{ 0xff00ff };  // カラーコードから
```

### カラーコードへの変換

```cpp
Udon::RGB rgb{ 0, 100, 255 };
const uint32_t color = rgb.to24bit();
```

### 他の色空間へ変換

```cpp
Udon::RGB rgb{ 0, 100, 255 };
const Udon::HSV hsv = rgb.toHSV();
```

## HSV 色空間

### 概要

色相 (Hue) 彩度(Saturation) 明度(Value) で表現される色空間のことで、次のように定義されています。各要素には 0~255 の数値が入ります。

色相の値を増やすだけで色が虹色に変化するので便利です。

```cpp
struct HSV
{
    uint8_t h;
    uint8_t s;
    uint8_t v;
};
```

### 構築

```cpp
Udon::HSV hsv;
Udon::HSV hsv{ 0, 100, 255 };
Udon::HSV hsv{ 0xff00ff };  // 24bit値から
```

### 他の色空間へ変換

```cpp
Udon::HSV hsv{ 0, 100, 255 };
const Udon::RGB rgb = hsv.toRGB();
```

### 24bit 値への変換

```cpp
Udon::HSV hsv{ 0, 100, 255 };
const uint32_t bits = hsv.to24bit();
```

## その他関数

### `Rainbow()`

時間を基に虹色に変化する RGB オブジェクトを取得できます。

```cpp
const Udon::RGB rainbow = Udon::Rainbow();
```
