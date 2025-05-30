//
//    メンバ関数の有無を判定し、存在する場合は呼び出す関数
//
//    Copyright (c) 2022 udonrobo
//

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>

#include "Typedef.hpp"

namespace Udon
{
    namespace Traits
    {
        /// @brief T に `T::begin` 関数が存在するか
        template <typename, typename = void>
        struct HasMemberFunctionBegin : std::false_type
        {
        };
        template <typename T>
        struct HasMemberFunctionBegin<T, std::void_t<decltype(&T::begin)>> : std::is_member_function_pointer<decltype(&T::begin)>
        {
        };

        /// @brief T に `T::begin` 関数が存在する場合呼び出す。それ以外の場合何もしない。
        template <typename HasMemberFunctionBegin, typename std::enable_if<Traits::HasMemberFunctionBegin<HasMemberFunctionBegin>::value, std::nullptr_t>::type = nullptr>
        void MaybeInvokeBegin(HasMemberFunctionBegin& rhs)
        {
            rhs.begin();
        }

        /// @brief T に `T::begin` 関数が存在する場合呼び出す。それ以外の場合何もしない。
        template <typename HasMemberFunctionBegin, typename std::enable_if<not Traits::HasMemberFunctionBegin<HasMemberFunctionBegin>::value, std::nullptr_t>::type = nullptr>
        void MaybeInvokeBegin(HasMemberFunctionBegin&)
        {
            // Do nothing.
        }
    }    // namespace Traits

    namespace Traits
    {
        /// @brief T に `T::show` 関数が存在するか
        template <typename, typename = void>
        struct HasMemberFunctionShow : std::false_type
        {
        };
        template <typename T>
        struct HasMemberFunctionShow<T, std::void_t<decltype(&T::show)>> : std::is_member_function_pointer<decltype(&T::show)>
        {
        };

        /// @brief T に `T::show const` 関数が存在する場合呼び出す。それ以外の場合何もしない。
        template <typename HasMemberFunctionShow, typename std::enable_if<Traits::HasMemberFunctionShow<HasMemberFunctionShow>::value, std::nullptr_t>::type = nullptr>
        void MaybeInvokeShow(const HasMemberFunctionShow& rhs)
        {
            rhs.show();
        }

        /// @brief T に `T::show const` 関数が存在する場合呼び出す。それ以外の場合何もしない。
        template <typename HasMemberFunctionShow, typename std::enable_if<not Traits::HasMemberFunctionShow<HasMemberFunctionShow>::value, std::nullptr_t>::type = nullptr>
        void MaybeInvokeShow(const HasMemberFunctionShow&)
        {
            // Do nothing.
        }
    }    // namespace Traits

    namespace Traits
    {
        /// @brief T に `T::showRaw` 関数が存在するか
        template <typename, typename = void>
        struct HasMemberFunctionShowRaw : std::false_type
        {
        };
        template <typename T>
        struct HasMemberFunctionShowRaw<T, std::void_t<decltype(&T::showRaw)>> : std::is_member_function_pointer<decltype(&T::showRaw)>
        {
        };

        /// @brief T に `T::showRaw const` 関数が存在する場合呼び出す。それ以外の場合何もしない。
        template <typename HasMemberFunctionShowRaw, typename std::enable_if<Traits::HasMemberFunctionShowRaw<HasMemberFunctionShowRaw>::value, std::nullptr_t>::type = nullptr>
        void MaybeInvokeShowRaw(const HasMemberFunctionShowRaw& rhs)
        {
            rhs.showRaw();
        }

        /// @brief T に `T::showRaw const` 関数が存在する場合呼び出す。それ以外の場合何もしない。
        template <typename HasMemberFunctionShowRaw, typename std::enable_if<not Traits::HasMemberFunctionShowRaw<HasMemberFunctionShowRaw>::value, std::nullptr_t>::type = nullptr>
        void MaybeInvokeShowRaw(const HasMemberFunctionShowRaw&)
        {
            // Do nothing.
        }
    }    // namespace Traits

    namespace Traits
    {
        /// @brief T に `T::update` 関数が存在するか
        template <typename, typename = void>
        struct HasMemberFunctionUpdate : std::false_type
        {
        };
        template <typename T>
        struct HasMemberFunctionUpdate<T, std::void_t<decltype(&T::update)>> : std::is_member_function_pointer<decltype(&T::update)>
        {
        };

        /// @brief T に `T::update` 関数が存在する場合呼び出す。それ以外の場合何もしない。
        template <typename HasMemberFunctionUpdate, typename std::enable_if<Traits::HasMemberFunctionUpdate<HasMemberFunctionUpdate>::value, std::nullptr_t>::type = nullptr>
        void MaybeInvokeUpdate(HasMemberFunctionUpdate& rhs)
        {
            rhs.update();
        }

        /// @brief T に `T::update` 関数が存在する場合呼び出す。それ以外の場合何もしない。
        template <typename HasMemberFunctionUpdate, typename std::enable_if<not Traits::HasMemberFunctionUpdate<HasMemberFunctionUpdate>::value, std::nullptr_t>::type = nullptr>
        void MaybeInvokeUpdate(HasMemberFunctionUpdate&)
        {
            // Do nothing.
        }
    }    // namespace Traits

    namespace Traits
    {
        template <typename OutputStream, typename T, typename = void>
        struct IsOutputStreamable : std::false_type
        {
        };

        template <typename OutputStream, typename T>
        struct IsOutputStreamable<OutputStream, T, std::void_t<decltype(std::declval<OutputStream&>() << std::declval<T>())>>    // operator<<(Stream&, T) が存在するか, かつ戻り値が Stream& かどうか
            : std::true_type
        {
        };
    }    // namespace Traits

    namespace Traits
    {

        struct DummyEnumerator
        {
            using ResultType = void;
        };

        /// @brief T が UDON_ENUMERABLE マクロによって列挙可能な型になっているかどうかを判定する
        template <typename T, typename = void>
        struct IsEnumerable : std::false_type
        {
        };

        template <typename T>
        struct IsEnumerable<T, typename std::enable_if<
                                   std::is_same<decltype(std::declval<T>().enumerateMutable(std::declval<DummyEnumerator&>())), typename DummyEnumerator::ResultType>::value    // enumerate(f) が存在するか, かつ戻り値が ResultType かどうか
                                   >::type>
            : std::true_type
        {
        };
    }    // namespace Traits

}    // namespace Udon
