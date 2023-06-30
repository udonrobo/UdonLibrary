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
//-------------------------------------------------------------------


#pragma once

#include <limits.h>

#include <Udon/Stl/EnableSTL.hpp>

#include <algorithm>
#include <vector>

#include <Udon/Algorithm/CRC8.hpp>
#include <Udon/Algorithm/Endian.hpp>
#include <Udon/Com/Serialization/Capacity.hpp>
#include <Udon/Math/Math.hpp>
#include <Udon/Types/Float.hpp>
#include <Udon/Utility/Parsable.hpp>

namespace Udon
{

    class Serializer
    {

        size_t  insertIndex     = 0;    // 次に挿入するインデックス(バッファの先端からのオオフセット)
        uint8_t boolCount       = 0;    // bool の挿入回数
        size_t  boolInsertIndex = 0;    // bool の挿入インデックス(バッファの先端からのオオフセット)

        std::vector<uint8_t> buffer;

    public:
        Serializer(size_t capacity)
            : buffer(capacity)
        {
        }

        template <typename Bool>
        inline auto operator()(const Bool& rhs)
            -> typename std::enable_if<std::is_same<Bool, bool>::value>::type
        {
            packBool(rhs);
        }

        /// @brief
        /// @tparam Integer 整数型
        /// @param rhs
        /// @return
        template <typename Integer>
        inline auto operator()(const Integer& rhs)
            -> typename std::enable_if<std::is_integral<Integer>::value && not std::is_same<Integer, bool>::value>::type
        {
            packScalar(rhs);
        }

        /// @brief
        /// @tparam Floating 浮動小数点型
        /// @param rhs
        /// @return
        template <typename Floating>
        inline auto operator()(const Floating& rhs)
            -> typename std::enable_if<std::is_floating_point<Floating>::value>::type
        {
            packScalar(static_cast<Udon::float32_t>(rhs));
        }

        /// @brief
        /// @tparam T 配列
        /// @param rhs
        /// @return
        template <typename T, size_t N>
        inline void operator()(const T (&rhs)[N])
        {
            for (auto&& it : rhs)
            {
                operator()(it);
            }
        }

        /// @brief
        /// @tparam T メンバにaccessorを持つ型
        /// @param rhs
        /// @return
        template <typename T>
        inline auto operator()(const T& rhs) -> typename std::enable_if<Udon::has_member_iterate_accessor<Serializer, T>::value>::type
        {
            // T::accessor が const なメンバ関数でない場合に const rhs から呼び出せないため、const_cast によって const を除去
            const_cast<T&>(rhs).accessor(*this);
        }

        /// @brief 可変長テンプレート引数再帰展開
        /// @tparam Head
        /// @tparam ...Tails
        /// @param head
        /// @param ...tails
        template <typename Head, typename... Tails>
        inline void operator()(const Head& head, const Tails&... tails)
        {
            operator()(head);
            operator()(tails...);
        }

        /// @brief バッファを取得する
        /// @remark 取得後のバッファは無効です。
        std::vector<uint8_t> flush()
        {
            buffer.back() = Udon::CRC8(buffer.data(), buffer.size() - Udon::CRC8_SIZE);
            if (Udon::GetEndian() == Endian::Big)
            {
                std::reverse(buffer.begin(), buffer.end());
            }
            return buffer;
        }

    private:
        /// @brief シリアル化
        /// @remark オブジェクトはスカラ型である必要があります
        /// @tparam Ty
        /// @param rhs シリアル化するオブジェクト
        template <typename T>
        void packScalar(const T& rhs)
        {
            static_assert(std::is_scalar<T>::value, "T must be scalar type.");

            constexpr size_t size = sizeof(T);

            // バッファの後方に挿入
            std::copy(
                reinterpret_cast<const uint8_t*>(&rhs),
                reinterpret_cast<const uint8_t*>(&rhs) + size,
                buffer.begin() + insertIndex);

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

    template <typename T>
    inline std::vector<uint8_t> Pack(const T& rhs)
    {
        
        static_assert(Udon::is_parsable<T>::value, "T must be parsable type.");   // Tはパース可能である必要があります。クラス内で UDON_PACKABLE マクロを使用することで、パース可能であることを宣言できます。

        Serializer serializer(Udon::CapacityWithChecksum(rhs));
        serializer(rhs);
        return serializer.flush();
    }

    /// @brief バッファにシリアル化する
    /// @tparam T シリアル化する型()
    /// @param rhs
    /// @param buffer
    /// @param size
    /// @remark バッファのサイズはCapacityWithChecksum関数で取得したサイズ以上である必要があります。
    /// @return シリアル化に成功したかどうか
    template <typename T>
    inline bool Pack(const T& rhs, uint8_t* buffer, size_t size)
    {
        static_assert(Udon::is_parsable<T>::value, "T must be parsable type.");   // Tはパース可能である必要があります。クラス内で UDON_PACKABLE マクロを使用することで、パース可能であることを宣言できます。

        if (size >= Udon::CapacityWithChecksum(rhs))
        {
            const auto vector = Pack(rhs);
            std::copy(vector.begin(), vector.end(), buffer);
            return true;
        }
        else
        {
            // サイズが足りない場合falseを返します。
            // この場合、bufferは変更されません。
            // CapacityWithChecksum関数を用いてバッファのサイズを指定しているか確認してください。
            // CapacityWithChecksum関数はチャックサムバイトを含めたバイト数を返します。
            return false;
        }
    }

    /// @brief バッファにシリアル化する
    /// @tparam T
    /// @tparam N
    /// @param rhs
    /// @param array
    /// @remark バッファのサイズはCapacityWithChecksum関数で取得したサイズ以上である必要があります。
    /// @return シリアル化に成功したかどうか
    template <typename T, size_t N>
    inline bool Pack(const T& rhs, uint8_t (&array)[N])
    {
        static_assert(Udon::is_parsable<T>::value, "T must be parsable type.");   // Tはパース可能である必要があります。クラス内で UDON_PACKABLE マクロを使用することで、パース可能であることを宣言できます。
        
        return Pack(rhs, array, N);
    }

}    // namespace Udon
