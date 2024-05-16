//
//    送受信者要件を満たすか判定する
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>

namespace Udon
{

    namespace Traits
    {

        /// @brief 送信クラスであるか判定
        /// @details 送信クラスの要件を満たすには、以下の条件を満たす必要がある。
        /// - `MessageType` というメンバ型名を持つ
        /// - `setMessage()` メンバ関数を持ち、戻り値が `void`、引数型が `const MessageType&` である
        template <template <typename> typename, typename = void>
        struct IsWriter
            : std::false_type
        {
        };

        template <template <typename> typename T>
        struct IsWriter<T, typename std::enable_if<std::is_same<
                               decltype(std::declval<T<int>>().setMessage(std::declval<const typename T<int>::MessageType&>())), void>::value>::type>
            : std::true_type
        {
        };

        /// @brief 送信クラスであるか判定
        /// @details 送信クラスの要件を満たすには、以下の条件を満たす必要がある。
        /// - `MessageType` というメンバ型名を持つ
        /// - `getMessage() const` メンバ関数を持ち、戻り値が `Udon::Optional<MessageType>`、引数が存在しない
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