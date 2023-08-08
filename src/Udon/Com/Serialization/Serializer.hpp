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
        template <typename Bool>
        inline auto operator()(const Bool& object)
            -> typename std::enable_if<std::is_same<Bool, bool>::value>::type
        {
            packBool(object);
        }

        /// @brief 整数型 && bool型以外
        template <typename Integer>
        inline auto operator()(const Integer& object)
            -> typename std::enable_if<std::is_integral<Integer>::value && not std::is_same<Integer, bool>::value>::type
        {
            packScalar(object);
        }

        /// @brief 浮動小数点型
        template <typename Floating>
        inline auto operator()(const Floating& object)
            -> typename std::enable_if<std::is_floating_point<Floating>::value>::type
        {
            packScalar(static_cast<Udon::float32_t>(object));
        }

        /// @brief 配列
        template <typename T, size_t N>
        inline void operator()(const T (&object)[N])
        {
            for (auto&& it : object)
            {
                operator()(it);
            }
        }

        /// @brief メンバにaccessorを持つ型
        template <typename T>
        inline auto operator()(const T& object)
            -> typename std::enable_if<Udon::has_member_iterate_accessor<Serializer, T>::value>::type
        {
            // T::accessor が const なメンバ関数でない場合に const object から呼び出せないため、const_cast によって const を除去
            const_cast<T&>(object).accessor(*this);
        }

        // template <typename T>
        // inline auto operator()(const T& object)
        //     -> typename std::enable_if<Udon::has_member_iterate_accessor<Serializer, T>::value == false>::type
        // {
        //     // T::accessor が存在しない場合は、T::accessor が存在しないことを示す static_assert が発生する
        //     static_assert(Udon::has_member_iterate_accessor<Serializer, T>::value, "T must have accessor member function.");
        // }

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
        void packScalar(const T& object)
        {
            static_assert(std::is_scalar<T>::value, "T must be scalar type.");

            constexpr size_t size = sizeof(T);

            // バッファの後方に挿入
#if defined(UDON_LITTLE_ENDIAN)
            std::copy(
                reinterpret_cast<const uint8_t*>(&object),
                reinterpret_cast<const uint8_t*>(&object) + size,
                buffer.begin() + insertIndex);
#elif defined(UDON_BIG_ENDIAN)
            std::copy(
                reinterpret_cast<const uint8_t*>(&object),
                reinterpret_cast<const uint8_t*>(&object) + size,
                buffer.rbegin() + insertIndex);
#endif

            // 次に挿入するインデックスを更新
            insertIndex += size;
        }

        void packBool(bool object)
        {
            if (boolCount == 0)
            {
                boolInsertIndex = insertIndex++;
            }

            Udon::BitWrite(buffer.at(boolInsertIndex), boolCount, object);

            if (++boolCount >= CHAR_BIT)
            {
                boolCount = 0;
            }
        }
    };

}    // namespace Udon
