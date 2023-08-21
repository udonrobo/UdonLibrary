# 色空間

## Usage

色空間を表現するには `Udon::RGB` `Udon::HSV` クラスを使用します。

### インクルード

```cpp
#include <Udon/Types/Color.hpp>
```

### RGB 色空間

赤緑青 の光の三原色で表現される色空間のことで、次のように定義されています。各要素には 0~255 の数値が入ります。

```cpp
struct RGB
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};
```

他の色空間へ変換

```cpp
const Udon::HSV hsv = rgb.toHSV();
```

### HSV 色空間

色相 (Hue) 彩度(Saturation) 明度(Value) で表現される色空間のことで、次のように定義されています。各要素には 0~255 の数値が入ります。

色相の値を増やすだけで色が虹色に変化するので便利です。

```cpp
struct RGB
{
    uint8_t h;
    uint8_t s;
    uint8_t v;
};
```

他の色空間へ変換

```cpp
const Udon::RGB rgb = hsv.toRGB();
```

### お便利な関数

- `Udon::Rainbow();`

  時間を基に虹色に変化する RGB オブジェクトを取得できます。

  ```cpp
  const Udon::RGB rainbow = Udon::Rainbow();
  ```
