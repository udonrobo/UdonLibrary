//-------------------------------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    シリアライザ
//
//    メンバ変数をスカラ型まで再帰的に解析しバイト列にシリアライズするクラスです。
//    このクラスはユーザーが直接使用することはありません。
//    Udon::Serialize(object) を使用してください。(Udon/Com/Serialization/Serializer.hpp)
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <algorithm>
#include <vector>
#include <limits.h>

#include <Udon/Algorithm/CRC.hpp>
#include <Udon/Algorithm/Bit.hpp>
#include <Udon/Types/Float.hpp>
#include <Udon/Traits/Typedef.hpp>
#include <Udon/Common/Platform.hpp>
#include <Udon/Types/ArrayView.hpp>

namespace Udon
{

    namespace Impl
    {
        using namespace Udon::Traits;

        class Serializer
        {

            size_t  pushIndex     = 0;    // 次に挿入するインデックス(バッファの先端からのオフセット)
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
                // 関数を constexpr にするため、enumerate の引数は "const Serializer& enumerator" になっている (一時オブジェクトを受けれるように)
                // enumerator (const *this) から serialize を呼び出すため、const_cast で const を外している

                argsUnpack(std::forward<Tails>(tails)...);
            }

            /// @brief 可変長引数展開 (終端)
            void argsUnpack() const {}

            /// @brief bool型
            template <typename Bool, EnableIfNullptrT<IsBool<RemoveReferenceT<Bool>>::value> = nullptr>
            void serialize(Bool rhs)
            {
                pushBool(rhs);
            }

            /// @brief 整数型 && bool型以外
            template <typename IntegralNotBool, EnableIfNullptrT<IsIntegralNotBool<RemoveReferenceT<IntegralNotBool>>::value> = nullptr>
            void serialize(IntegralNotBool rhs)
            {
                pushArithmetic(rhs);
            }

            /// @brief 浮動小数点型
            template <typename FloatingPoint, EnableIfNullptrT<IsFloatingPoint<RemoveReferenceT<FloatingPoint>>::value> = nullptr>
            void serialize(FloatingPoint rhs)
            {
                pushArithmetic(static_cast<Udon::float32_t>(rhs));
            }

            /// @brief 列挙型
            template <typename Enum, EnableIfNullptrT<IsEnum<RemoveReferenceT<Enum>>::value> = nullptr>
            void serialize(Enum rhs)
            {
                pushArithmetic(static_cast<typename std::underlying_type<Enum>::type>(rhs));
            }

            /// @brief 配列型
            template <typename Array, EnableIfNullptrT<IsArray<RemoveReferenceT<Array>>::value> = nullptr>
            void serialize(Array&& rhs)
            {
                for (auto&& element : rhs)
                {
                    serialize(element);
                }
            }

            /// @brief メンバ変数列挙用の関数が定義されている型
            template <typename Enumerable, EnableIfNullptrT<HasMemberFunctionEnumerate<RemoveReferenceT<Enumerable>>::value> = nullptr>
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
