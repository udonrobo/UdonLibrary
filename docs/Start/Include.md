# スタート

## インクルード

インクルードパスの設定か行えている場合 `UdonLibrary/src` 下のヘッダファイルをすべて使用できるようになります。

`Udon.hpp` をインクルードすることで、全ての機能を使用できるようになります。

```cpp
#include <Udon.hpp>

void setup() {}
void loop() {}
```

```cpp
#include <Udon.hpp>

int main() {}
```

必要なヘッダーだけ個別にインクルードしたい場合 `UdonFwd.hpp` をインクルードします。ArduinoIDE 以外で使用する場合 `UdonFwd.hpp` のインクルードは不要です。

```cpp
#include <UdonFwd.hpp>
#include <Udon/Types/Vector2D.hpp>  // Udon::Vec2

void setup() {}
void loop() {}
```

```cpp
#include <Udon/Types/Vector2D.hpp>  // Udon::Vec2

int main() {}
```

## 本ライブラリについて

本ライブラリは Arduino のコア部分に変更を加えたりすることはなく、拡張機能を関数やクラスとして提供しています。

全ての関数、クラスは `Udon` 名前空間に属しています。
