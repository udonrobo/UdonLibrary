#pragma once

#include <vector>

#include <udon\traits\HasMember.hpp>
#include <udon\algorithm\CRC8.hpp>
#include <udon\algorithm\Endian.hpp>
#include <udon\types\Float.hpp>

namespace udon
{

    class Serializer
    {

        std::vector<uint8_t> buffer;

    public:
        Serializer()
            : buffer()
        {
        }

        /// @brief
        /// @tparam T 整数型
        /// @param rhs
        /// @return
        template <typename T>
        inline auto operator()(const T& rhs) -> typename std::enable_if<std::is_integral<T>::value>::type
        {
            pack(rhs);
        }

        /// @brief
        /// @tparam T 浮動小数点型
        /// @param rhs
        /// @return
        template <typename T>
        inline auto operator()(const T& rhs) -> typename std::enable_if<std::is_floating_point<T>::value>::type
        {
            pack(static_cast<udon::float32_t>(rhs));
        }

        /// @brief
        /// @tparam T 配列
        /// @param rhs
        /// @return
        template <typename T, size_t N>
        inline auto operator()(const T (&rhs)[N])
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
        inline auto operator()(const T& rhs) -> typename std::enable_if<has_member_iterator_accessor_v<Serializer, T>>::type
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

        /// @brief バッファのキャパシティを変更する
        /// @param size サイズ
        void reserve(size_t size)
        {
            buffer.reserve(size);
        }

        /// @brief バッファを取得する
        /// @remarke 取得後のバッファは無効です。
        std::vector<uint8_t> flash()
        {
            buffer.push_back(udon::CRC8(buffer.data(), buffer.size()));
            if (udon::GetEndian() == Endian::Big)
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
        template <class Ty>
        void pack(const Ty& rhs)
        {
            // バッファの後方に挿入
            buffer.insert(
                buffer.end(),
                reinterpret_cast<const uint8_t*>(&rhs),
                reinterpret_cast<const uint8_t*>(&rhs) + sizeof(Ty));
        }
    };

    template <typename T>
    inline std::vector<uint8_t> Pack(const T& rhs)
    {
        Serializer serializer;
        serializer(rhs);
        return serializer.flash();
    }

    template <typename T>
    inline void Pack(uint8_t* buffer, const T& rhs)
    {
        const auto vector = Pack(rhs).data();
        memcpy(buffer, vector.data(), vector.size());
    }

}    // namespace udon