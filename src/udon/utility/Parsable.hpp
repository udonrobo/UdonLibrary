#pragma once

/// @brief 自作クラスのパースを可能にする
/// @param ... パース可能なメンバー変数(,区切り)
/// @details
/// - パース可能なメンバー変数は、以下の条件を満たす必要がある
///   - メンバー変数の型が、以下のいずれかであること
///     - パース可能な型
///     - パース可能な型の配列
/// - このマクロを使用すると、以下のメンバー関数が定義される
///   - constexpr size_t capacity() const
///     - シリアライズ時に必要なバッファのバイトサイズを返す
///   - template <typename Acc> void accessor(Acc& acc)
///     - シリアライズ、デシリアライズ時に使用するアクセッサ
#define UDON_PARSABLE(...)                  \
    constexpr size_t capacity() const       \
    {                                       \
        return udon::Capacity(__VA_ARGS__); \
    }                                       \
    template <typename Acc>                 \
    void accessor(Acc& acc)                 \
    {                                       \
        acc(__VA_ARGS__);                   \
    }
