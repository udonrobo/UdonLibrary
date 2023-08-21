# オイラー角

3 次元空間で回転姿勢を表す場合に使用します。

2 つの回転軸が重なると、回転軸の自由度が落ちるシンバルロックが起こります。

`Udon::Euler` クラスは以下のメンバを持ちます。

```cpp
template <typename T>
struct Euler3D
{
    using value_type = T;

    value_type roll;
    value_type pitch;
    value_type yaw;

    constexpr Euler3D() noexcept;
    constexpr Euler3D(value_type roll, value_type pitch, value_type yaw) noexcept;

    Euler3D(const Euler3D&) = default;

    Euler3D& operator=(const Euler3D&) = default;

    constexpr Euler3D operator+(const Euler3D& rhs) const noexcept;
    constexpr Euler3D operator-(const Euler3D& rhs) const noexcept;
    constexpr Euler3D operator*(const Euler3D& rhs) const noexcept;
    constexpr Euler3D operator/(const Euler3D& rhs) const noexcept;
    constexpr Euler3D operator+(value_type rhs) const noexcept;
    constexpr Euler3D operator-(value_type rhs) const noexcept;
    constexpr Euler3D operator*(value_type rhs) const noexcept;
    constexpr Euler3D operator/(value_type rhs) const noexcept;

    Euler3D& operator+=(const Euler3D& rhs) noexcept;
    Euler3D& operator-=(const Euler3D& rhs) noexcept;
    Euler3D& operator*=(const Euler3D& rhs) noexcept;
    Euler3D& operator/=(const Euler3D& rhs) noexcept;
    Euler3D& operator+=(value_type rhs) noexcept;
    Euler3D& operator-=(value_type rhs) noexcept;
    Euler3D& operator*=(value_type rhs) noexcept;
    Euler3D& operator/=(value_type rhs) noexcept;

    constexpr bool operator==(const Euler3D& rhs) const noexcept;
    constexpr bool operator!=(const Euler3D& rhs) const noexcept;

    // 要素のいずれかに0以外の値があるかどうかを返す
    explicit constexpr operator bool() const noexcept;

    // 要素がゼロであるか返す
    constexpr bool isZero() const noexcept;

    // 正規化されたオイラー角を取得する
    Euler3D normalized(value_type min, value_type max) const;

    // 回転方向を修正したオイラー角を取得する
    Euler3D directionRevision(const Euler3D<bool>& direction) const noexcept;

    // クオータニオンへ変換
    Quaternion toQuaternion() const noexcept;

    // 値クリア
    void clear() noexcept;

    // デバッグ出力
    void show() const noexcept;
};

using Euler = Euler3D<double>;
```
