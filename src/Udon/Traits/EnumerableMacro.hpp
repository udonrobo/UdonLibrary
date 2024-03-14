#pragma once

/// @brief メンバ変数の列挙を可能にする
/// @param ... 列挙可能なメンバー変数(','区切り)
#define UDON_ENUMERABLE(...)                                                                \
    template <typename Enumerator>                                                          \
    constexpr typename Enumerator::ResultType enumerate(const Enumerator& enumerator) const \
    {                                                                                       \
        return enumerator(__VA_ARGS__);                                                     \
    }
