#pragma once

#include "Parsable.hpp"

#include <Udon/Com/Serialization/SerializedSize.hpp>

/// @brief メンバ変数のパースを可能にする
/// @param ... パース可能なメンバー変数(,区切り)
/// @details
///
/// - パース可能な型は、以下の型を含む
///   - 整数型
///   - 浮動小数点型
///   - 列挙型
///   - パース可能な型の配列
///
/// - このマクロを使用すると、以下のメンバー関数が定義される
///
///   - constexpr bool parsable() const
///    - 全メンバ変数がパース可能かどうかを返す
///
///   - constexpr size_t packedBitSize() const
///     - シリアライズ時に必要なバッファのビットサイズを返す
///
///   - template <typename Acc> void accessor(Acc& acc)
///     - メンバ変数の走査用
///
#define UDON_PARSABLE(...)                                                                  \
    constexpr bool parsable() const                                                         \
    {                                                                                       \
        return Udon::Traits::IsMemberParsable(__VA_ARGS__);                                 \
    }                                                                                       \
    constexpr size_t packedBitSize() const                                                  \
    {                                                                                       \
        return true;                                           \
    }                                                                                       \
    template <typename Acc>                                                                 \
    void accessor(Acc& acc)                                                                 \
    {                                                                                       \
        acc(__VA_ARGS__);                                                                   \
    }                                                                                       \
    template <typename Enumerator>                                                          \
    constexpr typename Enumerator::ResultType enumerate(const Enumerator& enumerator) const \
    {                                                                                       \
        return enumerator(__VA_ARGS__);                                                     \
    }

#define UDON_ENUMERABLE(...) UDON_PARSABLE(__VA_ARGS__)
