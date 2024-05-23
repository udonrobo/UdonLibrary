# ディップスイッチ

`DecodeDipSwitch` 関数を用いてディップスイッチの値を読み取ります。特にロータリースイッチの値は二進数として取得されるので、十進数に変換する処理を行います。正論理である必要があります。

|                                                      DIP スイッチ                                                       |                                                 DIP ロータリースイッチ                                                  |
| :---------------------------------------------------------------------------------------------------------------------: | :---------------------------------------------------------------------------------------------------------------------: |
| <img src="https://github.com/udonrobo/UdonLibrary/assets/91818705/819152d6-d216-451b-85ac-3223579f4aea" width="200px"/> | <img src="https://github.com/udonrobo/UdonLibrary/assets/91818705/c4ebb0ea-1e42-400c-a9e7-2fd20ec8991d" width="200px"/> |

## 個別インクルード

```cpp
#include <Udon/Driver/DipSwitch.hpp>
```

## 値読み取り

`DecodeDipSwitch` の引数はピン番号を指定します。DIP スイッチの下位ビット端子から順に指定します。初期化リストを引数にとるため桁数に上限はありません。

```cpp
#include <Udon.hpp>

void setup()
{
    const int value = Udon::DecodeDipSwitch({ 1, 2, 3, 4 });
}

void loop()
{
}
```
