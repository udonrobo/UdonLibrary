#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>

namespace Udon
{
    namespace Traits
    {

        /// @brief 常に std::false_type を継承する
        /// @remark static_assert の失敗を遅延させるために使用する
        ///         例:
        ///         template <typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
        ///         void f()  // <- Tが整数であるとき、f() は実体化されてはいけないとする
        ///         {
        ///             static_assert(Udon::Traits::AlwaysFalse<T>::value, "T is integral.");
        ///         }
        ///         template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
        ///         void f()  // <- 実体化されるべき関数
        ///         {
        ///         }
        ///         f<int>();    // <- NG 実体化された時点で static_assert が失敗する
        ///         f<float>();  // <- OK int型で実体化されていないので static_assert は失敗しない
        template <class T>
        struct AlwaysFalse
            : std::false_type
        {
        };

    }    // namespace Traits
}    // namespace Udon