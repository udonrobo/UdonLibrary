#pragma once

#include "Parsable.hpp"

#include <Udon/Com/Serialization/Capacity.hpp>

/// @brief メンバ変数のパースを可能にする
/// @param ... パース可能なメンバー変数(,区切り)
/// @Details
/// - パース可能なメンバー変数は、以下の条件を満たす必要がある
///   - メンバー変数の型が、以下のいずれかであること
///     - パース可能な型
///     - パース可能な型の配列
/// - このマクロを使用すると、以下のメンバー関数が定義される
///   - constexpr size_t capacityBits() const
///     - シリアライズ時に必要なバッファのビットサイズを返す
///   - template <typename Acc> void accessor(Acc& acc)
///     - シリアライズ、デシリアライズ時に使用するアクセッサ
#define UDON_PARSABLE(...)                                  \
    constexpr bool parsable() const                         \
    {                                                       \
        return Udon::Traits::IsMemberParsable(__VA_ARGS__); \
    }                                                       \
    constexpr size_t capacityBits() const                   \
    {                                                       \
        return Udon::CapacityBits(__VA_ARGS__);             \
    }                                                       \
    template <typename Acc>                                 \
    void accessor(Acc& acc)                                 \
    {                                                       \
        acc(__VA_ARGS__);                                   \
    }
