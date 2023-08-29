#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>

namespace Udon
{

    namespace Traits
    {

        /// @brief 送信クラスであるか判定するトレイト
        /// @details 送信クラスの要件を満たすには、以下の条件を満たす必要がある。
        /// - `MessageType` というメンバ型名を持つ
        /// - `getMessage()` メンバ関数を持ち、戻り値が `MessageType`、引数が存在しない
        template <template <typename> typename, typename = void>
        struct IsReader : std::false_type
        {
        };

        template <template <typename> typename T>
        struct IsReader<T, std::void_t<
                               typename std::is_same<
                                   decltype(std::declval<T<int>>().getMessage()), typename T<int>::MessageType>::type>> : std::true_type
        {
        };

    }    // namespace Traits

}    // namespace Udon