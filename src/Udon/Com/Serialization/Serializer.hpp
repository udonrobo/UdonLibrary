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
#include <Udon/Algorithm/Endian.hpp>
#include <Udon/Com/Serialization/Capacity.hpp>
#include <Udon/Algorithm/Bit.hpp>
#include <Udon/Types/Float.hpp>
#include <Udon/Utility/Parsable.hpp>
#include <Udon/Utility/Concept.hpp>

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

        /// @brief bool型
        UDON_CONCEPT_BOOL
        inline void operator()(Bool rhs)
        {
            packBool(rhs);
        }

        /// @brief 整数型 && bool型以外
        UDON_CONCEPT_INTEGRAL_NOT_BOOL
        inline void operator()(IntegralNotBool rhs)
        {
            packScalar(rhs);
        }

        /// @brief 浮動小数点型
        UDON_CONCEPT_FLOATING_POINT
        inline void operator()(FloatingPoint rhs)
        {
            packScalar(static_cast<Udon::float32_t>(rhs));
        }

        /// @brief 配列
        template <typename T, size_t N>
        inline void operator()(const T (&rhs)[N])
        {
            for (auto&& it : rhs)
            {
                operator()(it);
            }
        }

        /// @brief メンバ関数 accessor<Acc>(Acc&) が存在する型
        template <typename T>
        inline auto operator()(const T& rhs)
            -> decltype(std::declval<T>().accessor(*this), std::declval<void>())
        {
            const_cast<T&>(rhs).accessor(*this);
        }

        /// @brief グローバル関数に Accessor<Acc>(Acc&, T&) が存在する型
        template <typename T>
        inline auto operator()(const T& rhs)
            -> decltype(Accessor(*this, std::declval<T&>()), std::declval<void>())
        {
            Accessor(*this, const_cast<T&>(rhs));
        }

        /// @brief 可変長引数展開の終端
        void operator()()
        {
        }

        /// @brief 可変長テンプレート引数再帰展開
        template <typename Head, typename... Tails>
        inline void operator()(const Head& head, const Tails&... tails)
        {
            operator()(head);
            operator()(tails...);
        }

        /// @brief バッファを取得する
        /// @remark 取得後の内部バッファは無効になります
        std::vector<uint8_t> flush()
        {

#ifdef UDON_BIG_ENDIAN
            std::reverse(buffer.begin(), buffer.end() - Udon::CRC8_SIZE);
#endif

            // チェックサム挿入
            buffer.back() = Udon::CRC8(buffer.cbegin(), buffer.cend() - Udon::CRC8_SIZE);

            return buffer;
        }

    private:
        /// @brief シリアル化
        template <typename T>
        void packScalar(const T& rhs)
        {
            static_assert(std::is_scalar<T>::value, "T must be scalar type.");

            constexpr size_t size = sizeof(T);

            // バッファの後方に挿入
#if defined(UDON_LITTLE_ENDIAN)
            std::copy(
                reinterpret_cast<const uint8_t*>(&rhs),
                reinterpret_cast<const uint8_t*>(&rhs) + size,
                buffer.begin() + insertIndex);
#elif defined(UDON_BIG_ENDIAN)
            std::copy(
                reinterpret_cast<const uint8_t*>(&rhs),
                reinterpret_cast<const uint8_t*>(&rhs) + size,
                buffer.rbegin() + insertIndex);
#endif

            // 次に挿入するインデックスを更新
            insertIndex += size;
        }

        void packBool(bool rhs)
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
