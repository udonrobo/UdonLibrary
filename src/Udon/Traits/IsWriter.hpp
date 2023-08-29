#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>

#include "VoidT.hpp"

namespace Udon
{

    namespace Traits
    {

        /// @brief 送信クラスであるか判定するトレイト
        /// @details 送信クラスの要件を満たすには、以下の条件を満たす必要がある。
        /// - `MessageType` というメンバ型名を持つ
        /// - `setMessage()` メンバ関数を持ち、戻り値が `void`、引数型が `const MessageType&` である
        template <template <typename> typename, typename = void>
        struct IsWriter : std::false_type
        {
        };

        template <template <typename> typename T>
        struct IsWriter<T, std::void_t<
                              typename std::is_same<
                                  decltype(std::declval<T<int>>().setMessage(std::declval<const typename T<int>::MessageType&>())), void>::type>> : std::true_type
        {
        };

    }    // namespace Traits

}    // namespace Udon