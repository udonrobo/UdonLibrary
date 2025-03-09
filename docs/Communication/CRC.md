# CRC エラーチェック

## `Udon::CRC8`

1 バイトの CRC 値を求める関数です。以下の様に定義され、ArrayView を引数にとるため、通常の配列や配列を指すポインタ等、様々な種類の配列から CRC 値を求められます。

> 本ライブラリではシリアライザ内部で CRC を用いています。シリアライズされたデータには CRC 値が付与され、デシリアライズ時に CRC 値の照合が行われます。

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

## CRC の概要

CRC はチェックサムの強化版とも言える誤り検出符号です。

チェックサムとは誤り検出方法のひとつで、送信バイト列から求められた値と受信バイト列から求めた値を照合し一致するか確認するアルゴリズムです。簡易的なチェックサムを求める関数の例を以下に示します。動作は単純で、データの各要素を足し合わせた値を返します。

チェックサムは単純なアルゴリズムで実装可能ですが、エラー検出率が低くなってしまいます(全要素が 0 の場合チェックサムも 0 となる。エラー時に 0 を受信することはよくあるため誤って正常と判断してしまう)。そこで実用上は CRC 値等を用いた誤り検出符号を用いることが一般的です。

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

for (auto&& e : data)
{
    write(e);       // データを送信
}

uint8_t checksum = Checksum(data, sizeof data);

write(checksum);    // チェックサム値を送信
```

</details>

<details>
<summary> 受信側使用例 </summary>

```cpp
uint8_t data[n];

for (auto& e : data)
{
    e = read();  // データを受信
}

uint8_t checksum = read();   // チェックサム値を受信

if (checksum == Checksum(data, sizeof data))   // 受信したチェックサムと求めたチェックサムを比較
{
    // 正常
}
else
{
    // 受信エラー
}
```

</details>
