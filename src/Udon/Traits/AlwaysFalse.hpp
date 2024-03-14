#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>

namespace Udon
{
    namespace Traits
    {

        /// @brief static_assert 失敗遅延用の型
        /// @note C++20 までは static_assert に false を渡すとテンプレートの実体化する前にエラーになるため、
        ///         この型を用いて実体化時点で static_assert が失敗するようにする。
        ///
        ///         例:
        ///         
        ///         template <typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
        ///         void f()  // <- Tが整数であるとき、f() は実体化されてはいけないとする
        ///         {
        ///             static_assert(Udon::Traits::AlwaysFalse<T>::value, "T is integral.");
        ///         }
        ///
        ///         template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
        ///         void f()  // <- 実体化されるべき関数
        ///         {
        ///         }
        ///         
        ///         f<int>();    // <- 実体化された時点で static_assert が失敗する
        ///         f<float>();  // <- int型で実体化されていないので static_assert は失敗しない
        ///         
        template <typename T>
        struct AlwaysFalse : std::false_type
        {
        };

    }    // namespace Traits
}    // namespace Udon