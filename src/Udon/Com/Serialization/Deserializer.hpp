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
        template <typename Bool>
        inline auto operator()(Bool& rhs)
            -> typename std::enable_if<std::is_same<Bool, bool>::value>::type
        {
            rhs = unpackBool();
        }

        /// @brief 整数型
        template <typename Integer>
        inline auto operator()(Integer& rhs)
            -> typename std::enable_if<std::is_integral<Integer>::value && not std::is_same<Integer, bool>::value>::type
        {
            rhs = unpackScalar<Integer>();
        }

        /// @brief 浮動小数点型
        template <typename Floating>
        inline auto operator()(Floating& rhs)
            -> typename std::enable_if<std::is_floating_point<Floating>::value>::type
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

        /// @brief メンバにaccessorを持つ型
        template <typename T>
        inline auto operator()(T& rhs) -> typename std::enable_if<Udon::has_member_iterate_accessor<Deserializer, T>::value>::type
        {
            // T::accessor が const なメンバ関数でない場合に const rhs から呼び出せないため、const_cast によって const を除去
            const_cast<T&>(rhs).accessor(*this);
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