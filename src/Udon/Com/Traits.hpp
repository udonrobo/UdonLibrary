#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>

namespace Udon
{
    /// @brief 受信クラスであるか判定するトレイト
    /// @details 受信クラスの要件を満たすには、以下の条件を満たす必要がある。
    /// - MessageType というメンバ型名を持つ
    /// - getMessage() メンバ関数を持ち、戻り値型が Udon::Optional<MessageType>、引数型が void である
    template <template <typename> typename T>
    class IsReader
    {
        template <typename U>
        static auto check(U*) -> typename std::is_same<decltype(std::declval<U>().getMessage()), Udon::Optional<typename U::MessageType>>::type;
        template <typename>
        static auto check(...) -> std::false_type;

    public:
        static constexpr bool value = decltype(check<T<int>>(nullptr))::value;
    };

    /// @brief 送信クラスであるか判定するトレイト
    /// @details 送信クラスの要件を満たすには、以下の条件を満たす必要がある。
    /// - MessageType というメンバ型名を持つ
    /// - setMessage() メンバ関数を持ち、戻り値がvoid、引数型が MessageType である
    template <template <typename> typename T>
    class IsWriter
    {
        template <typename U>
        static auto check(U*) -> typename std::is_same<decltype(std::declval<U>().setMessage(std::declval<typename U::MessageType>())), void>::type;
        template <typename>
        static auto check(...) -> std::false_type;

    public:
        static constexpr bool value = decltype(check<T<int>>(nullptr))::value;
    };
}    // namespace Udon