#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <Udon/Stl/Optional.hpp>
#include <type_traits>

#include "VoidT.hpp"

namespace Udon
{

    namespace Traits
    {

        /// @brief 送信クラスであるか判定するトレイト
        /// @details 送信クラスの要件を満たすには、以下の条件を満たす必要がある。
        /// - `MessageType` というメンバ型名を持つ
        /// - `getMessage()` メンバ関数を持ち、戻り値が `Udon::Optional<MessageType>`、引数が存在しない
        template <template <typename> typename, typename = void>
        struct IsReader
            : std::false_type
        {
        };

        template <template <typename> typename T>
        struct IsReader<T, typename std::enable_if<std::is_same<
                               decltype(std::declval<T<int>>().getMessage()), Udon::Optional<typename T<int>::MessageType>>::value>::type>
            : std::true_type
        {
        };

    }    // namespace Traits

}    // namespace Udon