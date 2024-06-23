# OpenSiv3D との連携

`Siv3D.hpp` をインクルードた後、`Udon.hpp` をインクルードすることで連携できます。

```cpp
#include <Siv3D.hpp>
#include <Udon.hpp>

int main()
{
    while (System::Update())
    {
    }
}
```

## 連携機能

ユーザー定義型の相互変換が可能です。これは変換コンストラクタ、キャスト演算子の実装によるものです。以下のユーザー定義型の相互変換が可能です。

- 二次元ベクトル
- 三次元ベクトル
- クオータニオン
- シリアル通信
- コントローラー

```cpp
while (System::Update())
{
    Udon::Vec2 center = s3d::Vec2{ 1.0, 2.0 };
}
```
