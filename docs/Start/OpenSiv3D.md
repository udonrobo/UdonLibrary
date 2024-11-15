# OpenSiv3D との連携

`Siv3D.hpp` をインクルードした後、`Udon.hpp` をインクルードすることで連携できます。

```cpp
#include <Siv3D.hpp>
#include <Udon.hpp>

void Main()
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

```cpp
while (System::Update())
{
    Udon::Vec2 center = s3d::Vec2{ 1.0, 2.0 };
}
```

また

- シリアル通信 `Udon::SivSerialReader` `Udon::SivSerialWriter`
- コントローラー `Udon::SivPadPS5`
