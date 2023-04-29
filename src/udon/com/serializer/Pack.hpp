#pragma once

#include <udon/stl/EnableSTL.hpp>
#include <vector>
#include <algorithm>

#include <udon/traits/HasMember.hpp>
#include <udon/algorithm/CRC8.hpp>
#include <udon/algorithm/Endian.hpp>
#include <udon/types/Float.hpp>
#include <udon/com/serializer/Capacity.hpp>

namespace udon
{

    class Serializer
    {

        std::vector<uint8_t> buffer;

    public:

		Serializer(size_t capacity = 0)
			: buffer()
		{
			buffer.reserve(capacity);
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
        inline auto operator()(const T& rhs) -> typename std::enable_if<udon::has_member_iterator_accessor<Serializer, T>::value>::type
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
        std::vector<uint8_t> flush()
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

	template<typename T>
	inline std::vector<uint8_t> Pack(const T& rhs)
	{
		Serializer serializer(udon::CapacityWithChecksum(rhs));
		serializer(rhs);
		return serializer.flush();
	}

	/// @brief バッファにシリアル化する
	/// @tparam T
	/// @param rhs
	/// @param buffer
	/// @param size
	/// @remark バッファのサイズはCapacityWithChecksum関数で取得したサイズ以上である必要があります。
	/// @return シリアル化に成功したかどうか
	template<typename T>
	inline bool Pack(const T& rhs, uint8_t* buffer, size_t size)
	{
		if (size >= udon::CapacityWithChecksum(rhs))
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
	template<typename T, size_t N>
	inline bool Pack(const T& rhs, uint8_t(&array)[N])
	{
		return Pack(rhs, array, N);
	}


}    // namespace udon