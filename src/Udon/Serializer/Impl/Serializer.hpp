//
//    シリアライザ実装部
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <algorithm>
#include <vector>
#include <limits.h>

#include <Udon/Algorithm/CRC.hpp>
#include <Udon/Algorithm/Bit.hpp>
#include <Udon/Types/Float.hpp>
#include <Udon/Traits/Typedef.hpp>
#include <Udon/Serializer/SerializerTraits.hpp>
#include <Udon/Utility/Platform.hpp>
#include <Udon/Types/ArrayView.hpp>

namespace Udon
{

    namespace Impl
    {
        using namespace Udon::Traits;

        class Serializer
        {

            size_t  pushIndex       = 0;    // 次に挿入するインデックス(バッファの先端からのオフセット)
            uint8_t boolCount       = 0;    // bool の挿入回数
            size_t  boolInsertIndex = 0;    // bool を挿入中であるインデックス(バッファの先端からのオフセット)

            ArrayView<uint8_t> buffer;    // シリアライズしたデータを格納するバッファへの参照

        public:
            using ResultType = void;

            explicit Serializer(ArrayView<uint8_t> buffer)
                : buffer(buffer)
            {
            }

            /// @brief シリアライズ
            /// @tparam ...Args
            /// @param ...args
            template <typename... Args>
            void operator()(Args&&... args) const
            {
                argsUnpack(std::forward<Args>(args)...);
            }

        private:
            /// @brief 可変長引数展開
            template <typename Head, typename... Tails>
            void argsUnpack(Head&& head, Tails&&... tails) const
            {
                const_cast<Serializer&>(*this).serialize(std::forward<Head>(head));
                // enumerate 関数は引数に一時オブジェクトを受けられる、かつconstexprである必要があるため "const Serializer& enumerator" になっている
                // その関係で、operator()、本関数はconstな関数となり、thisポインタはconstなポインタになる。そのため const を外す。

                argsUnpack(std::forward<Tails>(tails)...);
            }

            /// @brief 可変長引数展開 (終端)
            void argsUnpack() const {}

            /// @brief bool型
            template <typename Bool, EnableIfNullptrT<IsBool<RemoveCVRefT<Bool>>::value> = nullptr>
            void serialize(Bool rhs)
            {
                pushBool(rhs);
            }

            /// @brief 整数型 && bool型以外
            template <typename IntegralNotBool, EnableIfNullptrT<IsIntegralNotBool<RemoveCVRefT<IntegralNotBool>>::value> = nullptr>
            void serialize(IntegralNotBool rhs)
            {
                pushArithmetic(rhs);
            }

            /// @brief 浮動小数点型
            template <typename FloatingPoint, EnableIfNullptrT<IsFloatingPoint<RemoveCVRefT<FloatingPoint>>::value> = nullptr>
            void serialize(FloatingPoint rhs)
            {
                pushArithmetic(static_cast<Udon::Float32>(rhs));
            }

            /// @brief 列挙型
            template <typename Enum, EnableIfNullptrT<IsEnum<RemoveCVRefT<Enum>>::value> = nullptr>
            void serialize(Enum rhs)
            {
                pushArithmetic(static_cast<typename std::underlying_type<Enum>::type>(rhs));
            }

            /// @brief 配列型
            template <typename Array, EnableIfNullptrT<IsArray<RemoveCVRefT<Array>>::value> = nullptr>
            void serialize(Array&& rhs)
            {
                for (auto&& element : rhs)
                {
                    serialize(element);
                }
            }

            /// @brief メンバ変数列挙用の関数が定義されている型
            template <typename Enumerable, EnableIfNullptrT<HasMemberFunctionEnumerate<RemoveCVRefT<Enumerable>>::value> = nullptr>
            void serialize(Enumerable&& rhs)
            {
                // static_assert(AlwaysFalse<Enumerable>::value, "Enumerable type must have a member function named 'enumerate'" __FUNCSIG__   );
                rhs.enumerate(*this);
            }

            /// @brief シリアル化
            template <typename Arithmetic>
            void pushArithmetic(Arithmetic arithmetic)
            {
                constexpr auto size = sizeof(Arithmetic);

                // バッファの後方に挿入
                // リトルエンディアン環境でのバイト列の並びを順とする
#if UDON_PLATFORM_ENDIANNESS == UDON_PLATFORM_LITTLE_ENDIAN

                // オブジェクトをバイト列として解釈
                memcpy(
                    buffer.data() + pushIndex,
                    std::addressof(arithmetic),
                    size);

#elif UDON_PLATFORM_ENDIANNESS == UDON_PLATFORM_BIG_ENDIAN

                // オブジェクトをバイト列として解釈
                uint8_t reversedBuffer[size];
                memcpy(
                    reversedBuffer,
                    std::addressof(arithmetic),
                    size);

                // リトルエンディアンが基準なので、逆順にしてバッファに挿入
                std::reverse_copy(
                    reversedBuffer,
                    reversedBuffer + size,
                    buffer.begin() + pushIndex);

#endif

                // 次に挿入するインデックスを更新
                pushIndex += size;
            }

            /// @brief bool値のシリアル化
            void pushBool(bool rhs)
            {
                if (boolCount == 0)
                {
                    boolInsertIndex = pushIndex++;
                }

                Udon::BitWrite(buffer.at(boolInsertIndex), boolCount, rhs);

                if (++boolCount >= CHAR_BIT)
                {
                    boolCount = 0;
                }
            }
        };
    }    // namespace Impl

}    // namespace Udon
