# 割り込み禁止制御

`ScopedInterruptLocker` クラスを用いてインスタンスが存在する間割り込みを禁止します。主に通信クラス内で使用されます。

```cpp
// 割り込み許可
{
    Udon::ScopedInterruptLocker lock;
    // 割り込み禁止
}
// 割り込み許可
```

割り込み禁止の解除忘れを防止できるため、割り込み関数内で使用するリソースを返す関数などに有用です。

> 割り込み関数内で使用するリソースにアクセスする時、同タイミングで割り込みが起こることが考えられ、この時アクセスによって得られた値は不正な値となることがあります。そのため割り込み禁止期間にアクセスする必要があります。

ScopedInterruptLocker を使わない場合

```cpp
int value;  // 割り込み関数内で使用するリソース

// 割り込みハンドラ
void onReceive()
{
    value = read();
}

int getValue()
{
    noInterrupts();
    const int copy = value;
    interrupts();
    return copy;
}
```

ScopedInterruptLocker を使う場合

```cpp
int value;  // 割り込み関数内で使用するリソース

// 割り込みハンドラ
void onReceive()
{
    value = read();
}

int getValue()
{
    Udon::ScopedInterruptLocker lock;
    return value;
}
```
