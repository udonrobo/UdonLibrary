# 二次元ベクトル

二次元ベクトルを扱うには `Udon::Vec2` クラスを使用します。

```cpp
#include <Udon/Types/Vector2D.hpp>
```

## 構造

二次元ベクトルを表現するためのクラスです。x 軸と y 軸の 2 つの要素からなる二次元ベクトルを格納し、ベクトル演算や座標変換などの操作を提供します。

```cpp
struct Vector2D
{
    double x;
    double y;
};
using Vec2 = Vector2D;
```

x 軸の右方向を正、y 軸の上方向を正とする直交座標系を使用します。

<img width="40%" src="https://user-images.githubusercontent.com/91818705/230623615-236c3294-c8c6-4685-95de-fa02692b3cc5.png">

## 構築

```cpp
Udon::Vec2 vector;
Udon::Vec2 vector ( 1234.5, 6.789 );
Udon::Vec2 vector { 1234.5, 6.789 };
Udon::Vec2 vector = { 1234.5, 6.789 };
auto vector = Udon::Vec2 { 1234.5, 6.789 };
```

## 要素へのアクセス

`Vec2` 型のインスタンスには、`x` と `y` という 2 つの要素があります。これらの要素にアクセスするには、メンバ変数へのドット演算子(`.`)を使用します。

書き込み

```cpp
Udon::Vec2 vector { 2, 3 };
vector.x = 1234.5;
vector.y = 5678;
```

読み取り

```cpp
Udon::Vec2 vector { 1234.5, 6.789 };
double x = vector.x;
double y = vector.y;
```

## ベクトル演算

`Vector2D` クラスにはベクトル演算を行うための各種演算子があります。

加算

```cpp
Udon::Vec2 vector1 { 2.0, 3.0 };
Udon::Vec2 vector2 { 1.0, -1.5 };

Udon::Vec2 result = vector1 + vector2;
// result = { 3.0, 1.5 }
```

減算

```cpp
Udon::Vec2 vector1 { 2.0, 3.0 };
Udon::Vec2 vector2 { 1.0, -1.5 };

Udon::Vec2 result = vector1 - vector2;
// result = { 1.0, 4.5 }
```

スカラ倍

```cpp
Udon::Vec2 vector { 2.0, 3.0 };
double scalar = 1.5;

Udon::Vec2 result = vector * scalar;
// result = { 3.0, 4.5 }
```

## シリアルモニターに送信する

### `show()`

`setup` 関数内で `Serial.begin()` を呼び出しシリアル通信が開始されている必要があります。

```cpp
void setup()
{
    Serial.begin(115200);

    Udon::Vec2 vector { 3.0, 4.0 };
    vector.show();

    //> x: 3.0  y: 4.0
}
```

## ベクトルの長さを求める

### `length() -> double`

三平方の定理を用いてベクトルの長さを求めます。

```cpp
const Udon::Vec2 vector { 3.0, 4.0 };

const double length = vector.length();
// length = 5.0
```

### `distanceFrom(Vec2 other) -> double`

他のベクトルとの距離を求めます。

```cpp
const Udon::Vec2 vector1 { 1.0, 2.0 };
const Udon::Vec2 vector2 { 4.0, 6.0 };

const double distance = vector1.distanceFrom(vector2);  // ベクトル間の距離
// distance = 5.0
```

## ベクトルの角度を求める

### `angleAt(Vec2 other) -> double`

指定された点からの角度を求めます。

### `angle() -> double`

原点からの時計回りの角度を求めます

## ベクトルの長さを変更する

### `scaledLength(double length) -> Vec2`

ベクトルの長さ（大きさ）を指定した値に設定したベクトルを返します。ベクトルの向きは変わらず、大きさのみが変更されます。

基になるベクトルは変更されません。

```cpp
const Udon::Vec2 vector(3.0, 4.0);
const auto newVector = vector.scaledLength(2.0);  // ベクトルの長さを設定
// newVector = { 1.2, 1.6 }  (大きさが2.0のベクトル)
```

### `scaleLength(double length) -> Vec2&`

ベクトルの長さ（大きさ）を指定した値に変更します。

基になるベクトルを変更します。

長さに 0 を渡すとゼロベクトルとなり方向成分を失うため、次回呼び出し以降の長さ変更ができなくなる点に注意してください。

```cpp
Udon::Vec2 vector(3.0, 4.0);
vector.scaleLength(2.0);  // ベクトルの長さを変更
// vector = { 1.2, 1.6 }  (大きさが2.0に変更されたベクトル)
```

## 原点を中心にベクトルを指定角度回転させる

### `rotated(double angle) -> Vec2`

回転行列を用いて、ベクトルを指定した角度回転させた新しいベクトルを返します。

回転の中心は原点、回転方向は時計回りです。

基になるベクトルは変更されません。

```cpp
const Udon::Vec2 vector { 1.0, 0.0 };
const Udon::Vec2 rotated = vector.rotated(Udon::ToRadians(45.0));
// rotated = { 0.707, 0.707 }  (45度回転したベクトル)
```

### `rotate(double angle) -> Vec2&`

回転行列を用いて、ベクトルを指定した角度回転させます。

回転の中心は原点、回転方向は時計回りです。

基になるベクトルを変更します。

```cpp
Udon::Vec2 vector { 1.0, 0.0 };
vector.rotate(Udon::ToRadians(45));
// vector = { 0.707, 0.707 }  (45度回転したベクトル)
```

## 指定した点を中心にベクトルを指定角度回転させる

### `rotatedAt(Vec2 center, double angle) -> Vec2`

回転行列を用いて、指定した回転中心（中心点）を基準にベクトルを指定した角度（ラジアン）だけ回転させた新しいベクトルを返します。

回転方向は時計回りです。

基になるベクトルは変更されません。

```cpp
const Udon::Vec2 vector { 2.0, 1.0 };
const Udon::Vec2 center { 1.0, 1.0 };
const Udon::Vec2 rotated = vector.rotatedAt(center, Udon::ToRadians(90));  // ベクトルの回転
// rotated = { 1.0, 0.0 }  (中心を軸にして90度回転したベクトル)
```

### `rotateAt(Vec2 center, double angle) -> Vec2&`

回転行列を用いて、指定した回転中心（中心点）を基準にベクトルを指定した角度（ラジアン）だけ回転させます。

回転方向は時計回りです。

基になるベクトルを変更します。

```cpp
Udon::Vec2 vector { 2.0, 1.0 };
const Udon::Vec2 center { 1.0, 1.0 };
vector.rotateAt(center, Udon::ToRadians(90));  // ベクトルの回転
// vector = { 1.0, 0.0 }  (中心を軸にして90度回転したベクトル)
```

## 極座標系へ変換する

### `toPolar() -> Polar`

ベクトル(x, y)を極座標系(r, θ)に変換し返します。

```cpp
const Udon::Vec2 vector{ 100.0, 200.0 }
const Udon::Polar polar = vector.toPolar();
```

## ベクトルの各要素の値を一定の範囲に収める

### `clamped(double min, double max) -> Vec2`

各要素に constrain(Arduino)、 std::clamp(STL) を適用させたベクトルを返します。

基になるベクトルは変更されません。

```cpp
const Udon::Vec2 vector{ 100.0, 200.0 };
const Udon::Vec2 clamped = vector.clamped(130, 180);
// clamped = { 130, 180 }
```

### `clamp(double min, double max) -> Vec2&`

各要素に constrain(Arduino)、 std::clamp(STL) を適用させ代入します。

基になるベクトルを変更します。

```cpp
Udon::Vec2 vector{ 100.0, 200.0 };
vector.clamp(130, 180);
// vector = { 130, 180 }
```

## ベクトルをリマッピングする

### `mapped(double fromMin, double fromMax, double toMin, double toMax) -> Vec2`

各要素をある範囲から別の範囲に再マップしたベクトルを返します。

各要素に map(Arduino) を適用させたベクトルを返します。

基になるベクトルは変更されません。

```cpp
const Udon::Vec2 vector{ 100.0, 200.0 };
const Udon::Vec2 mapped = vector.mapped(0, 200, 0, 400);
// mapped = { 200.0, 400.0 }
```

### `map(double fromMin, double fromMax, double toMin, double toMax) -> Vec2&`

各要素をある範囲から別の範囲に再マップします。

各要素に map(Arduino) を適用させます。

基になるベクトルを変更します。

```cpp
Udon::Vec2 vector{ 100.0, 200.0 };
vector.map(0, 200, 0, 400);
// vector = { 200.0, 400.0 }
```

## OpenSiv3D との連携

`OpenSiv3D` が持つ `s3d::Vector2D<T>` と相互変換を行うことができます。

### `s3d::Vector2D<T>` -> `Udon::Vec2`

```cpp
s3d::Vec2 sivVector;
Udon::Vec2 udonVector = sivVector;   // 変換コンストラクタによる変換
```

### `Udon::Vec2` -> `s3d::Vector2D<T>`

```cpp
Udon::Vec2 udonVector;
s3d::Vec2 sivVector = udonVector;  // キャスト演算子 (operator s3d::Vector2D) による変換
```

## シリアライズ

このクラスはシリアライズ(バイト列化)に対応しています。したがって各通信用クラスに渡すことでマイコン同士でベクトルデータのやり取りができます。詳細は各通信用クラスのドキュメントを参照してください。

```cpp
Udon::Vec2 vector { 3.0, 4.0 };
writer.setMessage(vector);
```
