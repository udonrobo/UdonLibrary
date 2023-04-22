#pragma once

#include <vector>

#include <udon\algorithm\CRC8.hpp>
#include <udon\algorithm\Endian.hpp>
#include <udon\stl\optional.hpp>
#include <udon\traits\HasMember.hpp>
#include <udon\types\Float.hpp>

namespace udon
{

    class Deserializer
    {

        std::vector<uint8_t> buffer;

        bool isCheckSumSuccess;

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
            const auto checkSum = udon::CRC8(buffer.data(), buffer.size() - 1);
            isCheckSumSuccess   = (buffer.back() == checkSum);
        }

        operator bool() const
        {
            return isCheckSumSuccess;
        }

        /// @brief
        /// @tparam T 整数型
        /// @param rhs
        /// @return
        template <typename T>
        inline auto operator()(T& rhs) -> typename std::enable_if<std::is_integral<T>::value>::type
        {
            if (isCheckSumSuccess)
            {
                rhs = unpack<T>();
            }
        }

        /// @brief
        /// @tparam T 浮動小数点型
        /// @param rhs
        /// @return
        template <typename T>
        inline auto operator()(T& rhs) -> typename std::enable_if<std::is_floating_point<T>::value>::type
        {
            if (isCheckSumSuccess)
            {
                rhs = unpack<udon::float32_t>();
            }
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
        inline auto operator()(const T& rhs) -> typename std::enable_if<udon::has_member_iterator_accessor_v<Deserializer, T>>::type
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

    private:
        /// @brief 逆シリアル化
        /// @remark オブジェクトはスカラ型である必要があります
        /// @tparam Ty
        /// @return 復元されたオブジェクト
        template <class Ty>
        Ty unpack()
        {

            static_assert(std::is_scalar<Ty>::value, "Ty must be scalar type.");

            Ty retval{};

            // 逆シリアル化されたオブジェクトをバッファの前方から抽出
            std::copy(
                buffer.begin(),
                buffer.begin() + sizeof(Ty),
                reinterpret_cast<uint8_t*>(&retval));

            // 抽出した分、バッファを縮小
            buffer.erase(buffer.begin(), buffer.begin() + sizeof(Ty));

            return retval;
        }
    };

    template <typename T>
    udon::optional<T> Unpack(const std::vector<uint8_t>& buffer)
    {
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
        return Unpack<T>(std::vector<uint8_t>{ buffer, size });
    }

}    // namespace udon