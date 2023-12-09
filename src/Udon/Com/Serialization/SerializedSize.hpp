//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>
#include <utility>
#include <limits.h>

#include <Udon/Algorithm/Math.hpp>
#include <Udon/Algorithm/CRC.hpp>
#include <Udon/Types/Float.hpp>
#include <Udon/Traits/Parsable.hpp>
#include <Udon/Traits/Typedef.hpp>
#include <Udon/Traits/HasMemberFunction.hpp>

#if CHAR_BIT != 8
#    error "1byte is must be 8bit"
#endif

namespace Udon
{
    namespace Impl
    {
        using namespace Udon::Traits;

        struct IsSerializedSizableImpl
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

            // 配列型は要素のサイズを取得可能な場合、サイズ取得可能
            template <typename Array>
            struct Test<Array, EnableIfVoidT<IsArray<Array>::value>>
            {
                template <typename T>
                static constexpr bool test(const IsSerializedSizableImpl& self, T&& array)
                {
                    return Test<typename std::remove_extent<Array>::type>::test(self, *array);    // 要素のサイズが取得可能である場合
                }
            };

            // enumerate
            template <typename Enumerable>
            struct Test<Enumerable, EnableIfVoidT<HasMemberFunctionEnumerate<Enumerable>::value>>
            {
                template <typename T>
                static constexpr bool test(const IsSerializedSizableImpl& tester, T&& e)
                {
                    return e.enumerate(tester);    // enumerate 関数が true を返した場合
                }
            };
        };

        struct SerializedBitSizeImpl
        {
        public:
            using ResultType = size_t;

            template <typename... Args>
            constexpr ResultType operator()(Args&&... args) const noexcept
            {
                return argsUnpack(std::forward<Args>(args)...);
            }

        private:
            /// @brief 可変長引数展開
            template <typename Head, typename... Tail>
            constexpr ResultType argsUnpack(Head&& head, Tail&&... tail) const noexcept
            {
                return Sizeof<Head&&>::value(*this, std::forward<Head>(head)) +
                       argsUnpack(std::forward<Tail>(tail)...);
            }

            /// @brief 可変長引数展開 (終端)
            constexpr ResultType argsUnpack() const noexcept { return 0; }

        private:
            /// @brief T のビット数を取得する
            /// @remark サイズを取得できないオブジェクトが渡された場合を検出し、コンパイルエラーを発生させるために部分特殊化を用いる
            template <typename T, typename = void>
            struct Sizeof
            {
                static constexpr ResultType value(...) noexcept
                {
                    static_assert(AlwaysFalse<T>::value, "T is not sizable!");    // サイズを取得できないオブジェクトが渡された場合、コンパイルエラーを発生させる
                    return 0;
                }
            };

            /// @brief T が bool 型の場合の特殊化
            template <typename Bool>
            struct Sizeof<Bool, EnableIfVoidT<IsBool<RemoveReferenceT<Bool>>::value>>
            {
                static constexpr ResultType value(...) noexcept { return 1; }
            };

            /// @brief T が算術型の場合の特殊化
            template <typename Integral>
            struct Sizeof<Integral, EnableIfVoidT<IsIntegralNotBool<RemoveReferenceT<Integral>>::value>>
            {
                static constexpr ResultType value(...) noexcept { return sizeof(Integral) * CHAR_BIT; }
            };

            /// @brief T が浮動小数点型の場合の特殊化
            template <typename Float>
            struct Sizeof<Float, EnableIfVoidT<IsFloatingPoint<RemoveReferenceT<Float>>::value>>
            {
                static constexpr ResultType value(...) noexcept { return sizeof(Udon::float32_t) * CHAR_BIT; }
            };

            /// @brief T が列挙型の場合の特殊化
            template <typename Enum>
            struct Sizeof<Enum, EnableIfVoidT<IsEnum<RemoveReferenceT<Enum>>::value>>
            {
                static constexpr ResultType value(...) noexcept { return sizeof(Enum) * CHAR_BIT; }
            };

            /// @brief T が配列型の場合の特殊化
            template <typename Array>
            struct Sizeof<Array, EnableIfVoidT<IsArray<RemoveReferenceT<Array>>::value>>
            {
                static constexpr ResultType value(const SerializedBitSizeImpl& self, Array&& array) noexcept
                {
                    using ElementT = typename std::remove_extent<RemoveReferenceT<Array>>::type;
                    return Sizeof<ElementT>::value(self, std::forward<ElementT>(*array)) * std::extent<RemoveReferenceT<Array>>::value;
                }
            };

            /// @brief T が enumerate 関数を持つ場合の特殊化
            template <typename Enumerable>
            struct Sizeof<Enumerable, EnableIfVoidT<HasMemberFunctionEnumerate<RemoveReferenceT<Enumerable>>::value>>
            {
                static constexpr ResultType value(const SerializedBitSizeImpl& self, Enumerable&& e) noexcept
                {
                    return e.enumerate(self);
                }
            };
        };
    }    // namespace Impl

    namespace Traits
    {
        /// @brief T が シリアライズ後のサイズを取得可能か判定する
        template <typename T>
        struct IsSerializedSizable : std::integral_constant<bool, Impl::IsSerializedSizableImpl{}(RemoveReferenceT<T>{})>
        {
        };

        template <typename T>
        using IsSerializable = IsSerializedSizable<T>;

        template <typename T>
        using IsDeserializable = IsSerializedSizable<T>;

    }    // namespace Traits

    /// @brief Tをシリアライズした際のバイト列の要素数を取得する
    /// @tparam T シリアライズ対象の型
    /// @details シリアライズ後のサイズは、チェックサムのサイズを含む
    /// @details 実行時にサイズを取得する必要がある場合、この関数を使用する
    template <typename T>
    constexpr size_t SerializedSize() noexcept
    {
        static_assert(Traits::IsSerializedSizable<T>::value, "T is not packed sizable");    // T は "シリアライズ後のサイズを取得可能な型" である必要がある

        return Udon::Ceil(Impl::SerializedBitSizeImpl{}(T{}) / static_cast<double>(CHAR_BIT)) + Udon::CRC8_SIZE;
    }

}    // namespace Udon
