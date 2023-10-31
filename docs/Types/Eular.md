# オイラー角

3 次元空間で回転姿勢を表す場合に使用します。

2 つの回転軸が重なると、回転軸の自由度が落ちるシンバルロックが起こります。

`Udon::Euler` クラスは以下のメンバを持ちます。

```cpp
template <typename T>
struct Euler
{
    using ValueType = T;

    ValueType roll;
    ValueType pitch;
    ValueType yaw;

    constexpr Euler() noexcept;
    constexpr Euler(ValueType roll, ValueType pitch, ValueType yaw) noexcept;

    Euler(const Euler&) = default;

    Euler& operator=(const Euler&) = default;

    constexpr Euler operator+(const Euler& rhs) const noexcept;
    constexpr Euler operator-(const Euler& rhs) const noexcept;
    constexpr Euler operator*(const Euler& rhs) const noexcept;
    constexpr Euler operator/(const Euler& rhs) const noexcept;
    constexpr Euler operator+(ValueType rhs) const noexcept;
    constexpr Euler operator-(ValueType rhs) const noexcept;
    constexpr Euler operator*(ValueType rhs) const noexcept;
    constexpr Euler operator/(ValueType rhs) const noexcept;

    Euler& operator+=(const Euler& rhs) noexcept;
    Euler& operator-=(const Euler& rhs) noexcept;
    Euler& operator*=(const Euler& rhs) noexcept;
    Euler& operator/=(const Euler& rhs) noexcept;
    Euler& operator+=(ValueType rhs) noexcept;
    Euler& operator-=(ValueType rhs) noexcept;
    Euler& operator*=(ValueType rhs) noexcept;
    Euler& operator/=(ValueType rhs) noexcept;

    constexpr bool operator==(const Euler& rhs) const noexcept;
    constexpr bool operator!=(const Euler& rhs) const noexcept;

    // 要素のいずれかに0以外の値があるかどうかを返す
    explicit constexpr operator bool() const noexcept;

    // 要素がゼロであるか返す
    constexpr bool isZero() const noexcept;

    // 正規化されたオイラー角を取得する
    Euler normalized(ValueType min, ValueType max) const;

    // 回転方向を修正したオイラー角を取得する
    constexpr Euler directionRevision(EulerDirection direction) const noexcept;

    // クオータニオンへ変換
    Quaternion toQuaternion() const noexcept;

    // 値クリア
    void clear() noexcept;

    // デバッグ出力
    void show() const noexcept;
};

using Euler = Euler<double>;
```
