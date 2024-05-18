# CRC エラーチェック

## チェックサムの概要

チェックサムとは誤り検出方法のひとつで、送信バイト列から求められた値と受信バイト列から求めた値を照合し一致するか確認するアルゴリズムです。簡易的なチェックサムを求める関数の例を以下に示します。例のようなデータの各要素を足す様なアルゴリズムではエラー検出率が低くなってしまいます(全要素が 0 の場合チェックサムも 0 となる。エラー時に 0 を受信することはよくあるため誤って正常と判断してしまう。)。そこで実用上は CRC値 という値を用いて照合します。

> 本ライブラリではシリアライザ内部で CRC を用いています。シリアライズされたデータには CRC 値が付与され、デシリアライズ時に CRC 値の照合が行われます。

```cpp
uint8_t Checksum(uint8_t* data, size_t n)
{
    uint8_t checksum;
    for (size_t i = 0; i < n; ++i)
    {
        checksum += data[i];
    }
    return checksum;
}
```

<details>
<summary> 送信側使用例 </summary>

```cpp
uint8_t data[n];
uint8_t checksum;

for (auto&& e : data)
{
    write(e);       // データを送信
}

write(Checksum(data, sizeof data));    // チェックサム値を送信
```

</details>

<details>
<summary> 受信側使用例 </summary>

```cpp
uint8_t data[n];
uint8_t checksum;

for (auto& e : data)
{
    e = read();  // データを受信
}

uint8_t receivedChecksum = read();   // チェックサム値を受信

if (receivedChecksum == Checksum(data, sizeof data))   // 受信したチェックサムと求めたチェックサムを比較
{
    // 正常
}
else
{
    // 受信エラー
}
```

</details>

## `Udon::CRC8`

1 バイトの CRC 値を求める関数です。以下の様に定義され、ArrayView を引数にとるため、通常の配列や配列を指すポインタ等、様々な種類の配列から CRC 値を求められます。

```cpp
uint8_t CRC8(ArrayView<const uint8_t> buffer)
```

通常の配列は ArrayView コンストラクタによる暗黙の型変換によって、そのまま引数に渡せます。

```cpp
uint8_t data[100];
uint8_t checksum = Udon::CRC8(data);
```

ポインタからの場合 ArrayView コンストラクタを明示的に呼び出す必要があります。

```cpp
uint8_t* data = new uint8_t[100];
uint8_t checksum = Udon::CRC8({ data, 100 });
```
