# OpenSiv3D との連携

前提として、OpenSiv3D と本ライブラリが Visual Studio のプロジェクトで使えるようになっている必要があります。

`Siv3D.hpp` をインクルードした後、`Udon.hpp` をインクルードすることで連携できます。順序が逆だと連携機能が使えません。

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

ユーザー定義型の相互変換が可能です。これは変換コンストラクタ、キャスト演算子の実装によるものです。以下の型の相互変換が可能です。

- 二次元ベクトル `Udon::Vec2` - `s3d::Vec2`
- 三次元ベクトル `Udon::Vec3` - `s3d::Vec3`
- クオータニオン `Udon::Quat` - `s3d::Quaternion`

```cpp
while (System::Update())
{
    Udon::Vec2 center = s3d::Vec2{ 1.0, 2.0 };
}
```

また USB 経由シリアル通信、USB経由コントローラーの機能を使用可能です。

- シリアル通信 `Udon::SivSerialReader` `Udon::SivSerialWriter`
- コントローラー `Udon::SivPadPS5`
