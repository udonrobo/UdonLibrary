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
//    デシリアライザ
//
//    シリアライズされたバイト列をデシリアライズしオブジェクトに復元します。
//    このクラスはユーザーが直接使用することはありません。
//    Udon::Unpack<Type>(byte[]) を使用してください。(Udon/Com/Serialization/Unpack.hpp)
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <vector>
#include <limits.h>

#include <Udon/Algorithm/CRC.hpp>
#include <Udon/Algorithm/Endian.hpp>
#include <Udon/Algorithm/Bit.hpp>
#include <Udon/Stl/Optional.hpp>
#include <Udon/Types/Float.hpp>
#include <Udon/Utility/Parsable.hpp>
#include <Udon/Utility/Concept.hpp>

namespace Udon
{

    class Deserializer
    {

        size_t  popIndex     = 0;    // 次に抽出するインデックス(バッファの先端からのオオフセット)
        uint8_t boolCount    = 0;    // bool の抽出回数
        size_t  boolPopIndex = 0;    // bool を抽出中であるインデックス(バッファの先端からのオオフセット)

        std::vector<uint8_t> buffer;

        bool isChecksumSuccess;

    public:
        /// @brief コンストラクタ
        /// @param buffer デシリアライするバイト列
        Deserializer(const std::vector<uint8_t>& buf)
            : buffer(buf)
        {

            // チャックサム確認
            const auto checksum = Udon::CRC8(buffer.cbegin(), buffer.cend() - Udon::CRC8_SIZE);
            isChecksumSuccess   = buffer.back() == checksum;

            if (not isChecksumSuccess)
            {
                return;
            }

#ifdef UDON_BIG_ENDIAN
            std::reverse(buffer.begin(), buffer.end() - Udon::CRC8_SIZE);
#endif
        }

        /// @brief チェックサムが正しいか
        explicit operator bool() const
        {
            return isChecksumSuccess;
        }

        /// @brief bool型
        UDON_CONCEPT_BOOL
        inline void operator()(Bool rhs)
        {
            rhs = unpackBool();
        }

        /// @brief 整数型
        UDON_CONCEPT_INTEGRAL_NOT_BOOL
        inline void operator()(IntegralNotBool& rhs)
        {
            rhs = unpackScalar<IntegralNotBool>();
        }

        /// @brief 浮動小数点型
        UDON_CONCEPT_FLOATING_POINT
        inline void operator()(FloatingPoint rhs)
        {
            rhs = unpackScalar<Udon::float32_t>();
        }

        /// @brief 配列
        template <typename T, size_t N>
        inline void operator()(T (&rhs)[N])
        {
            for (auto&& it : rhs)
            {
                operator()(it);
            }
        }

        /// @brief メンバ関数 accessor<Acc>(Acc&) が存在する型
        template <typename T>
        inline auto operator()(T& rhs)
            -> decltype(std::declval<T>().accessor(*this), std::declval<void>())
        {
            rhs.accessor(*this);
        }

        /// @brief グローバル関数に Accessor<Acc>(Acc&, T&) が存在する型
        template <typename T>
        inline auto operator()(T& rhs)
            -> decltype(Accessor(*this, std::declval<T&>()), std::declval<void>())
        {
            Accessor(*this, rhs);
        }

        /// @brief 可変長引数展開の終端
        void operator()()
        {
        }

        /// @brief 可変長テンプレート引数再帰展開
        template <typename Head, typename... Tails>
        inline void operator()(Head& head, Tails&... tails)
        {
            operator()(head);
            operator()(tails...);
        }

    private:
        /// @brief 逆シリアル化
        template <class T>
        T unpackScalar()
        {
            static_assert(std::is_scalar<T>::value, "T must be scalar type.");

            T retval{};

            constexpr size_t size = sizeof(T);

            // 逆シリアル化されたオブジェクトをバッファから抽出
#if defined(UDON_LITTLE_ENDIAN)
            std::copy(
                buffer.cbegin() + popIndex,
                buffer.cbegin() + popIndex + size,
                reinterpret_cast<uint8_t*>(&retval));
#elif defined(UDON_BIG_ENDIAN)
            std::copy(
                buffer.crbegin() + popIndex,
                buffer.crbegin() + popIndex + size,
                reinterpret_cast<uint8_t*>(&retval));
#endif

            // 抽出したオブジェクトのバイト数分インデックスを進める
            popIndex += size;

            return retval;
        }

        /// @brief bool値の逆シリアル化
        bool unpackBool()
        {
            if (boolCount == 0)
            {
                boolPopIndex = popIndex++;
            }

            const auto retval = Udon::BitRead(buffer.at(boolPopIndex), boolCount);

            if (++boolCount >= CHAR_BIT)
            {
                boolCount = 0;
            }

            return retval;
        }
    };

}    // namespace Udon