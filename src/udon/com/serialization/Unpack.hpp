//-----------------------------------------------
//
//	UdonLibrary
// 
//	Copyright (c) 2022-2023 Okawa Yusuke
//	Copyright (c) 2022-2023 udonrobo
//
//	Licensed under the MIT License.
//
//-----------------------------------------------
//
//  デシリアライザ
//
//-----------------------------------------------


#pragma once

#include <limits.h>

#include <udon/stl/EnableSTL.hpp>

#include <vector>

#include <udon/algorithm/CRC8.hpp>
#include <udon/algorithm/Endian.hpp>
#include <udon/math/Math.hpp>
#include <udon/stl/optional.hpp>
#include <udon/types/Float.hpp>
#include <udon/utility/Parsable.hpp>

namespace udon
{

    class Deserializer
    {

        size_t  popIndex     = 0;    // 次に抽出するインデックス(バッファの先端からのオオフセット)
        uint8_t boolCount    = 0;    // bool の抽出回数
        size_t  boolPopIndex = 0;    // bool の抽出インデックス(バッファの先端からのオオフセット)

        std::vector<uint8_t> buffer;

        bool isChecksumSuccess;

    public:
        /// @brief コンストラクタ
        /// @param buffer デシリアライするバイト列
        Deserializer(const std::vector<uint8_t>& buf)
            : buffer(buf)
        {
            // エンディアン変換
            if (udon::GetEndian() == Endian::Big)
            {
                std::reverse(buffer.begin(), buffer.end());
            }

            // チャックサム確認
            const auto checksum = udon::CRC8(buffer.data(), buffer.size() - udon::CRC8_SIZE);
            isChecksumSuccess   = buffer.back() == checksum;
        }

        explicit operator bool() const
        {
            return isChecksumSuccess;
        }

        template <typename Bool>
        inline auto operator()(Bool& rhs)
            -> typename std::enable_if<std::is_same<Bool, bool>::value>::type
        {
            rhs = unpackBool();
        }

        /// @brief
        /// @tparam T 整数型
        /// @param rhs
        /// @return
        template <typename Integer>
        inline auto operator()(Integer& rhs)
            -> typename std::enable_if<std::is_integral<Integer>::value && not std::is_same<Integer, bool>::value>::type
        {
            rhs = unpackScalar<Integer>();
        }

        /// @brief
        /// @tparam T 浮動小数点型
        /// @param rhs
        /// @return
        template <typename Floating>
        inline auto operator()(Floating& rhs)
            -> typename std::enable_if<std::is_floating_point<Floating>::value>::type
        {
            rhs = unpackScalar<udon::float32_t>();
        }

        /// @brief
        /// @tparam T 配列
        /// @param rhs
        /// @return
        template <typename T, size_t N>
        inline void operator()(T (&rhs)[N])
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
        inline auto operator()(T& rhs) -> typename std::enable_if<udon::has_member_iterate_accessor<Deserializer, T>::value>::type
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
        inline void operator()(Head& head, Tails&... tails)
        {
            operator()(head);
            operator()(tails...);
        }

    private:
        /// @brief 逆シリアル化
        /// @remark オブジェクトはスカラ型である必要があります
        /// @tparam T
        /// @return 復元されたオブジェクト
        template <class T>
        T unpackScalar()
        {
            static_assert(std::is_scalar<T>::value, "T must be scalar type.");

            T retval{};

            constexpr size_t size = sizeof(T);

            // 逆シリアル化されたオブジェクトをバッファの前方から抽出
            std::copy(
                buffer.cbegin() + popIndex,
                buffer.cbegin() + popIndex + size,
                reinterpret_cast<uint8_t*>(&retval));

            // 抽出したオブジェクトのバイト数分インデックスを進める
            popIndex += size;

            return retval;
        }

        bool unpackBool()
        {
            if (boolCount == 0)
            {
                boolPopIndex = popIndex++;
            }

            const auto retval = udon::BitRead(buffer.at(boolPopIndex), boolCount);

            if (++boolCount >= CHAR_BIT)
            {
                boolCount = 0;
            }

            return retval;
        }
    };

    template <typename T>
    udon::optional<T> Unpack(const std::vector<uint8_t>& buffer)
    {
        // Tはパース可能である必要があります。クラス内で UDON_PACKABLE マクロを使用することで、パース可能であることを宣言できます。
        static_assert(udon::is_parsable<T>::value, "T must be parsable type.");
        
        if (buffer.size() != udon::CapacityWithChecksum<T>())
        {
            return udon::nullopt;
        }

        Deserializer deserializer(buffer);

        if (deserializer)
        {
            T retval;
            deserializer(retval);
            return retval;
        }
        else
        {
            return udon::nullopt;
        }
    }

    template <typename T>
    udon::optional<T> Unpack(const uint8_t* buffer, size_t size)
    {
        // Tはパース可能である必要があります。クラス内で UDON_PACKABLE マクロを使用することで、パース可能であることを宣言できます。
        static_assert(udon::is_parsable<T>::value, "T must be parsable type.");

        return Unpack<T>({ buffer, buffer + size });
    }

    template <typename T, size_t N>
    udon::optional<T> Unpack(const uint8_t (&array)[N])
    {
        // Tはパース可能である必要があります。クラス内で UDON_PACKABLE マクロを使用することで、パース可能であることを宣言できます。
        static_assert(udon::is_parsable<T>::value, "T must be parsable type.");

        return Unpack<T>(array, N);
    }

}    // namespace udon
