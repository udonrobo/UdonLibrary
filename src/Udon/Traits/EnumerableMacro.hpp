#pragma once    

/// @brief メンバ変数の列挙を可能にする
/// @param ... 列挙可能なメンバー変数(','区切り)
/// @details
///
/// - 列挙可能な型は、以下の型を含む
///   - 整数型
///   - 浮動小数点型
///   - 列挙型
///   - 列挙可能な型の配列
///
/// - このマクロを使用すると、以下のメンバー関数が定義される
///
///   - constexpr bool parsable() const
///    - 全メンバ変数が列挙可能かどうかを返す
///
///   - constexpr size_t packedBitSize() const
///     - シリアライズ時に必要なバッファのビットサイズを返す
///
///   - template <typename Acc> void accessor(Acc& acc)
///     - メンバ変数の走査用
///
#define UDON_ENUMERABLE(...)                                                                  \
    template <typename Enumerator>                                                          \
    constexpr typename Enumerator::ResultType enumerate(const Enumerator& enumerator) const \
    {                                                                                       \
        return enumerator(__VA_ARGS__);                                                     \
    }

