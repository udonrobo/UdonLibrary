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
//    Udon::Pack(object) を使用してください。(Udon/Com/Serialization/Pack.hpp)
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
#include <Udon/Traits/Accessible.hpp>
#include <Udon/Common/Platform.hpp>

namespace Udon
{

    class Serializer
    {

        size_t  insertIndex     = 0;    // 次に挿入するインデックス(バッファの先端からのオオフセット)
        uint8_t boolCount       = 0;    // bool の挿入回数
        size_t  boolInsertIndex = 0;    // bool を挿入中であるインデックス(バッファの先端からのオオフセット)

        std::vector<uint8_t> buffer;

    public:
        Serializer(size_t capacity)
            : buffer(capacity)
        {
        }

        /// @brief シリアライズ
        /// @tparam ...Args
        /// @param ...args
        template <typename... Args>
        inline void operator()(Args&&... args)
        {
            argumentUnpack(std::forward<Args>(args)...);
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
        inline void argumentUnpack(const Head& head, const Tails&... tails)
        {
            argumentUnpack(head);
            argumentUnpack(tails...);
        }

        /// @brief 可変長引数展開
        template <typename T>
        inline void argumentUnpack(const T& rhs)
        {
            serialize(rhs);
        }

        /// @brief 可変長引数展開の終端
        inline void argumentUnpack()
        {
        }

        /// @brief bool型
        UDON_CONCEPT_BOOL
        inline void serialize(Bool rhs)
        {
            serializeBool(rhs);
        }

        /// @brief 整数型 && bool型以外
        UDON_CONCEPT_INTEGRAL_NOT_BOOL
        inline void serialize(IntegralNotBool rhs)
        {
            serializeArithmetic(rhs);
        }

        /// @brief 浮動小数点型
        UDON_CONCEPT_FLOATING_POINT
        inline void serialize(FloatingPoint rhs)
        {
            serializeArithmetic(static_cast<Udon::float32_t>(rhs));
        }

        /// @brief 配列型
        UDON_CONCEPT_ARRAY
        inline void serialize(const Array& rhs)
        {
            for (const auto& element : rhs)
            {
                serialize(element);
            }
        }

        /// @brief メンバ変数列挙用の関数が定義されている型
        UDON_CONCEPT_ACCESSIBLE
        inline void serialize(const Accessible& rhs)
        {
            Udon::Traits::InvokeAccessor(*this, const_cast<Accessible&>(rhs));
        }

        /// @brief シリアル化
        template <typename T>
        void serializeArithmetic(const T& rhs)
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

}    // namespace Udon
