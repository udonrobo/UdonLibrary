//
//    メンバ変数の列挙を可能にするマクロ
//
//    Copyright (c) 2022 udonrobo
//

#pragma once

/// @brief メンバ変数の列挙を可能にする
/// @param ... 列挙可能なメンバー変数(','区切り)
/// @note 使用例
/// ```cpp
/// struct Vector2
/// {
///     float x;
///     float y;
///
///     UDON_ENUMERABLE(x, y)
/// };
/// ```
/// @note enumerate_const と enumerate の2つの関数が生成される
///       - enumerate_const : constexpr 関数でコンパイル時にメンバ関数の情報へアクセスできる
///       - enumerate : enumerate_const は const な関数である必要があり、これではデシリアライザなどのメンバを書き換える操作ができない。そのため本関数は存在する。
#define UDON_ENUMERABLE(...)                                                                \
    template <typename Enumerator>                                                          \
    constexpr typename Enumerator::ResultType enumerateConstexpr(const Enumerator& e) const \
    {                                                                                       \
        return e.enumeration(__VA_ARGS__);                                                  \
    }                                                                                       \
                                                                                            \
    template <typename Enumerator>                                                          \
    typename Enumerator::ResultType enumerateMutable(Enumerator& e) const                   \
    {                                                                                       \
        return e.enumeration(__VA_ARGS__);                                                  \
    }                                                                                       \
                                                                                            \
    template <typename Enumerator>                                                          \
    typename Enumerator::ResultType enumerateImmutable(Enumerator& e)                       \
    {                                                                                       \
        return e.enumeration(__VA_ARGS__);                                                  \
    }
