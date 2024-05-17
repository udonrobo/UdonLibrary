# PIO

Raspberry Pi Pico には 2 つの CPU コアがありますが、加えてステートマシンという補助的なコアが 8 つ内蔵されています。

ステートマシンは CPU とは独立して動作するため、常に入力ピンを監視するようなシビアな制御に有用です。通常のコアと異なり、機械語を用いて命令を書きます。[pioasm](https://wokwi.com/tools/pioasm) でアセンブラから機械語へのコンパイルを行えます。

8 つのステートマシンはそれぞれ個別の命令を実行できるわけではありません。ステートマシンは 2 つの PIO ブロックに分けられており、命令を格納するメモリが PIO ブロック単位にのみ存在するためです。

## PioClockBegin

PIO を用いて特定のピンからクロック信号を出力します。通信用コントローラーの外部クロックとしての使用を想定しています。

```cpp
#include <Udon.hpp>

void setup()
{
    Udon::PioClockBegin(2, 16'000'000);  // 2 ピンに 16MHz のクロックを出力
}

void loop() {}
```

戻り値から出力開始に成功したか取得できます。ステートマシンもしくは命令メモリの空きがない場合、失敗します。

## AllocateStateMachine

空きステートマシンを検索し、命令のロードを行う関数です。ライブラリ内部で使用する想定の関数です。

戻り値から割り当てられたステートマシンの情報を取得できます。

```cpp
#include <Udon.hpp>

// pioasm が生成
static const struct pio_program squarewave_program = {
    ...
};

void setup()
{
    if (const Udon::Optional<StateMachine> sm = Udon::Pio::AllocateStateMachine(squarewave_program))
    {

        ...  // クロック分周期などを設定

        pio_sm_set_enabled(sm->pio, sm->index, true);  // ステートマシン起動
    }
    else
    {
        // 割り当て失敗
    }
}

void loop() {}
```

`StateMachine` 構造体は以下のように定義されています。

```cpp
struct StateMachine
{
    PIO pio;       // pio0 or pio1
    int index;     // state machine index 0~3
    int offset;    // offset of the program
};
```

ステートマシンもしくは命令メモリの空きがない場合、`Udon::nullopt` を返し割り当てに失敗します。例えば以下のように 8 つより多くのステートマシンを使用しようとした場合失敗します。

```cpp
const auto result1 = AllocateStateMachine(squarewave_program);   // ステートマシン: 1   命令メモリ: PIO0
const auto result2 = AllocateStateMachine(squarewave_program);   // ステートマシン: 2   命令メモリ: PIO0
const auto result3 = AllocateStateMachine(squarewave_program);   // ステートマシン: 3   命令メモリ: PIO0
const auto result4 = AllocateStateMachine(squarewave_program);   // ステートマシン: 4   命令メモリ: PIO0
const auto result5 = AllocateStateMachine(squarewave_program);   // ステートマシン: 5   命令メモリ: PIO1
const auto result6 = AllocateStateMachine(squarewave_program);   // ステートマシン: 6   命令メモリ: PIO1
const auto result7 = AllocateStateMachine(squarewave_program);   // ステートマシン: 7   命令メモリ: PIO1
const auto result8 = AllocateStateMachine(squarewave_program);   // ステートマシン: 8   命令メモリ: PIO1
const auto result9 = AllocateStateMachine(squarewave_program);   // nullopt
```

同じ命令であれば同じ PIO 内のステートマシンに割り当てられますが、以下のように異なる命令と組み合わせる場合、使用できるステートマシンの数が大きく制限されます。

```cpp
const auto result1 = AllocateStateMachine(hogehoge_program);     // ステートマシン: 1   命令メモリ: PIO0
const auto result2 = AllocateStateMachine(squarewave_program);   // ステートマシン: 5   命令メモリ: PIO1
const auto result3 = AllocateStateMachine(squarewave_program);   // ステートマシン: 6   命令メモリ: PIO1
const auto result4 = AllocateStateMachine(squarewave_program);   // ステートマシン: 7   命令メモリ: PIO1
const auto result5 = AllocateStateMachine(squarewave_program);   // ステートマシン: 8   命令メモリ: PIO1
const auto result6 = AllocateStateMachine(squarewave_program);   // nullopt
```

```cpp
const auto result1 = AllocateStateMachine(hogehoge_program);     // ステートマシン: 1   命令メモリ: PIO0
const auto result2 = AllocateStateMachine(hugahuga_program);     // ステートマシン: 5   命令メモリ: PIO1
const auto result3 = AllocateStateMachine(squarewave_program);   // nullopt
```
