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
//    Udon::Serialize(object) を使用してください。(Udon/Com/Serialization/Serialize.hpp)
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

namespace Udon
{

    namespace Impl
    {
        using namespace Udon::Traits;

        class Serializer
        {

            size_t  insertIndex     = 0;    // 次に挿入するインデックス(バッファの先端からのオフセット)
            uint8_t boolCount       = 0;    // bool の挿入回数
            size_t  boolInsertIndex = 0;    // bool を挿入中であるインデックス(バッファの先端からのオフセット)

            std::vector<uint8_t> buffer;

        public:
            using ResultType = void;

            explicit Serializer(size_t capacity)
                : buffer(capacity)
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

            /// @brief バッファを取得する
            /// @remark 取得後の内部バッファは無効になります
            std::vector<uint8_t> flush()
            {

#if UDON_PLATFORM_ENDIANNESS == UDON_PLATFORM_BIG_ENDIAN
                std::reverse(buffer.begin(), buffer.end() - Udon::CRC8_SIZE);
#endif

                // チェックサム挿入
                buffer.back() = Udon::CRC8(buffer.cbegin(), buffer.cend() - Udon::CRC8_SIZE);

                return buffer;
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
                serializeBool(rhs);
            }

            /// @brief 整数型 && bool型以外
            template <typename IntegralNotBool, EnableIfNullptrT<IsIntegralNotBool<RemoveReferenceT<IntegralNotBool>>::value> = nullptr>
            void serialize(IntegralNotBool rhs)
            {
                serializeArithmetic(rhs);
            }

            /// @brief 浮動小数点型
            template <typename FloatingPoint, EnableIfNullptrT<IsFloatingPoint<RemoveReferenceT<FloatingPoint>>::value> = nullptr>
            void serialize(FloatingPoint rhs)
            {
                serializeArithmetic(static_cast<Udon::float32_t>(rhs));
            }

            /// @brief 列挙型
            template <typename Enum, EnableIfNullptrT<IsEnum<RemoveReferenceT<Enum>>::value> = nullptr>
            void serialize(Enum rhs)
            {
                serializeArithmetic(static_cast<typename std::underlying_type<Enum>::type>(rhs));
            }

            /// @brief 配列型
            template <typename Array, EnableIfNullptrT<IsArray<RemoveReferenceT<Array>>::value> = nullptr>
            void serialize(Array&& rhs)
            {
                for (const auto& element : rhs)
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
            template <typename T>
            void serializeArithmetic(T&& rhs)
            {
                constexpr auto size = sizeof(T);

                // バッファの後方に挿入
#if UDON_PLATFORM_ENDIANNESS == UDON_PLATFORM_LITTLE_ENDIAN

                std::copy(
                    reinterpret_cast<const uint8_t*>(&rhs),
                    reinterpret_cast<const uint8_t*>(&rhs) + size,
                    buffer.begin() + insertIndex);

#elif UDON_PLATFORM_ENDIANNESS == UDON_PLATFORM_BIG_ENDIAN

                std::copy(
                    reinterpret_cast<const uint8_t*>(&rhs),
                    reinterpret_cast<const uint8_t*>(&rhs) + size,
                    buffer.rbegin() + insertIndex);

#endif

                // 次に挿入するインデックスを更新
                insertIndex += size;
            }

            /// @brief bool値のシリアル化
            void serializeBool(bool rhs)
            {
                if (boolCount == 0)
                {
                    boolInsertIndex = insertIndex++;
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
