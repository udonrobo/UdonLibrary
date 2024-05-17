//
//    シリアライズ可能か判定するメタ関数
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#include <Udon/Traits/Typedef.hpp>
#include <Udon/Traits/HasMemberFunction.hpp>
#include <utility>

namespace Udon
{

    namespace Impl
    {
        using namespace Udon::Traits;

        struct IsSerializableImpl
        {
            using ResultType = bool;

            template <typename... Args>
            constexpr ResultType operator()(Args&&... args) const noexcept
            {
                return argsUnpack(std::forward<Args>(args)...);
            }

        private:
            // 可変長引数展開
            template <typename Head, typename... Tail>
            constexpr ResultType argsUnpack(Head&& head, Tail&&... tail) const noexcept
            {
                // 全メンバが出力可能か判定し、論理積を得る
                return Test<RemoveReferenceT<Head>>::test(*this, std::forward<Head>(head)) and
                       argsUnpack(std::forward<Tail>(tail)...);
            }

            // 可変長引数展開 (終端)
            constexpr ResultType argsUnpack() const noexcept { return true; }

        public:
            // T のビット数を取得できるか判定するテスト
            // 部分特殊化を用いる
            template <typename T, typename = void>
            struct Test
            {
                static constexpr bool test(...) { return false; }
            };

            // 算術型は可能
            template <typename Arithmetic>
            struct Test<Arithmetic, EnableIfVoidT<IsArithmetic<Arithmetic>::value>>
            {
                static constexpr bool test(...) { return true; }
            };

            // 列挙型は可能
            template <typename Enum>
            struct Test<Enum, EnableIfVoidT<IsEnum<Enum>::value>>
            {
                static constexpr bool test(...) { return true; }
            };

            // 配列型は要素がシリアライズ可能であれば可能
            template <typename Array>
            struct Test<Array, EnableIfVoidT<IsArray<Array>::value>>
            {
                template <typename T>
                static constexpr bool test(const IsSerializableImpl& self, T&& array)
                {
                    return Test<typename std::remove_extent<Array>::type>::test(self, *array);    // 要素のサイズが取得可能である場合
                }
            };

            // enumerate 関数を持つ型は enumerate 関数が true を返した場合可能
            template <typename Enumerable>
            struct Test<Enumerable, EnableIfVoidT<HasMemberFunctionEnumerate<Enumerable>::value>>
            {
                template <typename T>
                static constexpr bool test(const IsSerializableImpl& tester, T&& e)
                {
                    return e.enumerate(tester);
                }
            };
        };

    }    // namespace Impl

    namespace Traits
    {
        /// @brief T が シリアライズ可能か判定する
        template <typename T>
        struct IsSerializable
            : std::integral_constant<bool, Impl::IsSerializableImpl{}(RemoveReferenceT<T>{})>
        {
        };

    }    // namespace Traits
}    // namespace Udon