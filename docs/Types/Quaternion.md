# クオータニオン

3 次元空間で回転姿勢を表す場合に使用します。

クオータニオンは複素平面の三次元版であり、回転を積で表すことができるため処理が軽いです。またオイラー角特有のシンバルロックが起きません。

`Udon::Quaternion` クラスは以下のメンバを持ちます。

```cpp
struct Quaternion
{
    using value_type = double;

    value_type x;
    value_type y;
    value_type z;
    value_type w;

    constexpr Quaternion() noexcept;
    constexpr Quaternion(value_type x, value_type y, value_type z, value_type w) noexcept;

    Quaternion(const Quaternion&) = default;
    Quaternion& operator=(const Quaternion&) = default;

    // 回転を組合わせる
    constexpr Quaternion operator*(const Quaternion& rhs) const noexcept;
    Quaternion& operator*=(const Quaternion& rhs) noexcept;

    constexpr bool operator==(const Quaternion& rhs) const noexcept;
    constexpr bool operator!=(const Quaternion& rhs) const noexcept;

    // 要素のいずれかに0以外の値があるかどうかを返す
    explicit constexpr operator bool() const noexcept;

    // 逆クオータニオン
    constexpr Quaternion inverce() const noexcept;

    // 単位クオータニオン
    static Quaternion Identity() noexcept;

    // X軸回転クオータニオン
    static Quaternion RotateX(value_type angle) noexcept;

    // Y軸回転クオータニオン
    static Quaternion RotateY(value_type angle) noexcept;

    // Z軸回転クオータニオン
    static Quaternion RotateZ(value_type angle) noexcept;

    // 要素がゼロであるか
    constexpr bool isZero() const noexcept;

    // 値クリア
    void clear() noexcept;

    // オイラー角へ変換
    template <typename T = value_type>
    Euler3D<T> toEuler() const noexcept;

    // ヨー角を取得
    double toYaw() const noexcept;

    // ピッチ角を取得
    double toPitch() const noexcept;

    // ロール角を取得
    double toRoll() const noexcept;

    // デバッグ出力
    void show() const;
};
```
