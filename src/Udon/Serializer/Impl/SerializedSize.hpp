//
//    シリアライズ後のサイズを取得する
//
//    Copyright (c) 2022 udonrobo
//

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>
#include <utility>
#include <limits.h>

#include <Udon/Algorithm/Math.hpp>
#include <Udon/Algorithm/CRC.hpp>
#include <Udon/Types/Float.hpp>
#include <Udon/Traits/Typedef.hpp>
#include <Udon/Traits/AlwaysFalse.hpp>
#include <Udon/Serializer/SerializerTraits.hpp>
#include <Udon/Traits/HasMemberFunction.hpp>

#if CHAR_BIT != 8
#    error "1byte is must be 8bit"
#endif

namespace Udon
{
    namespace Impl
    {
        struct SerializedBitSizeImpl
        {
        public:
            using ResultType = size_t;

            template <typename... Args>
            constexpr ResultType enumeration(Args&&... args) const noexcept
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
            /// @note サイズを取得できないオブジェクトが渡された場合を検出し、コンパイルエラーを発生させるために部分特殊化を用いる
            template <typename T, typename = void>
            struct Sizeof
            {
                static constexpr ResultType value(...) noexcept
                {
#ifdef __FUNCTION__
                    static_assert(AlwaysFalse<T>::value, "T is not sizable!" __FUNCTION__);    // サイズを取得できないオブジェクトが渡された場合、コンパイルエラーになる
#else
                    static_assert(AlwaysFalse<T>::value, "T is not sizable!");    // サイズを取得できないオブジェクトが渡された場合、コンパイルエラーになる
#endif
                    return 0;
                }
            };

            /// @brief T が bool 型の場合の特殊化
            template <typename Bool>
            struct Sizeof<Bool, EnableIfVoidT<IsBool<RemoveCVRefT<Bool>>::value>>
            {
                static constexpr ResultType value(...) noexcept { return 1; }
            };

            /// @brief T が算術型の場合の特殊化
            template <typename Integral>
            struct Sizeof<Integral, EnableIfVoidT<IsIntegralNotBool<RemoveCVRefT<Integral>>::value>>
            {
                static constexpr ResultType value(...) noexcept { return sizeof(Integral) * CHAR_BIT; }
            };

            /// @brief T が浮動小数点型の場合の特殊化
            template <typename Float>
            struct Sizeof<Float, EnableIfVoidT<IsFloatingPoint<RemoveCVRefT<Float>>::value>>
            {
                static constexpr ResultType value(...) noexcept { return sizeof(Udon::Float32) * CHAR_BIT; }
            };

            /// @brief T が列挙型の場合の特殊化
            template <typename Enum>
            struct Sizeof<Enum, EnableIfVoidT<IsEnum<RemoveCVRefT<Enum>>::value>>
            {
                static constexpr ResultType value(...) noexcept { return sizeof(Enum) * CHAR_BIT; }
            };

            /// @brief T が配列型の場合の特殊化
            template <typename Array>
            struct Sizeof<Array, EnableIfVoidT<IsArray<RemoveCVRefT<Array>>::value>>
            {
                static constexpr ResultType value(const SerializedBitSizeImpl& self, Array&& array) noexcept
                {
                    using ElementT = typename std::remove_extent<RemoveReferenceT<Array>>::type;
                    return Sizeof<ElementT>::value(self, std::forward<ElementT>(*array)) * std::extent<RemoveReferenceT<Array>>::value;
                }
            };

            /// @brief T が enumerate 関数を持つ場合の特殊化
            template <typename Enumerable>
            struct Sizeof<Enumerable, EnableIfVoidT<IsEnumerable<RemoveReferenceT<Enumerable>>::value>>
            {
                static constexpr ResultType value(const SerializedBitSizeImpl& self, Enumerable&& e) noexcept
                {
                    return e.enumerateConstexpr(self);
                }
            };
        };
    }    // namespace Impl
}    // namespace Udon
