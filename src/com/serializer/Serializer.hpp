#pragma once

#include <common/EnableSTL.hpp>
#include <common/types/Float.hpp>

#include <vector>
#include <algorithm>

namespace udon
{

	class Serializer
	{

		std::vector<uint8_t> buffer;

	public:

		Serializer() = default;

		Serializer(const std::vector<uint8_t>& buffer)
			: buffer(buffer)
		{}

		/// @brief バッファのキャパシティを変更する
		/// @param size サイズ
		void reserve(size_t size)
		{
			buffer.reserve(size);
		}

		/// @brief バッファを取得する
		std::vector<uint8_t> data() const
		{
			return buffer;
		}

		/// @brief scalar type
		/// @param rhs
		/// @return
		Serializer& operator<<(bool     rhs) { pack(rhs); return *this; }
		Serializer& operator<<(int8_t   rhs) { pack(rhs); return *this; }
		Serializer& operator<<(int16_t  rhs) { pack(rhs); return *this; }
		Serializer& operator<<(int32_t  rhs) { pack(rhs); return *this; }
		Serializer& operator<<(int64_t  rhs) { pack(rhs); return *this; }
		Serializer& operator<<(uint8_t  rhs) { pack(rhs); return *this; }
		Serializer& operator<<(uint16_t rhs) { pack(rhs); return *this; }
		Serializer& operator<<(uint32_t rhs) { pack(rhs); return *this; }
		Serializer& operator<<(uint64_t rhs) { pack(rhs); return *this; }

#ifdef UDON_HAS_FLOAT32
		Serializer& operator<<(float    rhs) { pack(static_cast<udon::float32_t>(rhs)); return *this; }
		Serializer& operator<<(double   rhs) { pack(static_cast<udon::float32_t>(rhs)); return *this; }
#else
#	error シリアライズを行うには udon::float32_t が定義されている必要があります。
#endif

		Serializer& operator>>(bool    & rhs) { rhs = unpack<bool    >(); return *this; }
		Serializer& operator>>(int8_t  & rhs) { rhs = unpack<int8_t  >(); return *this; }
		Serializer& operator>>(int16_t & rhs) { rhs = unpack<int16_t >(); return *this; }
		Serializer& operator>>(int32_t & rhs) { rhs = unpack<int32_t >(); return *this; }
		Serializer& operator>>(int64_t & rhs) { rhs = unpack<int64_t >(); return *this; }
		Serializer& operator>>(uint8_t & rhs) { rhs = unpack<uint8_t >(); return *this; }
		Serializer& operator>>(uint16_t& rhs) { rhs = unpack<uint16_t>(); return *this; }
		Serializer& operator>>(uint32_t& rhs) { rhs = unpack<uint32_t>(); return *this; }
		Serializer& operator>>(uint64_t& rhs) { rhs = unpack<uint64_t>(); return *this; }

#ifdef UDON_HAS_FLOAT32
		Serializer& operator>>(float   & rhs) { rhs = static_cast<float >(unpack<udon::float32_t>()); return *this; }
		Serializer& operator>>(double  & rhs) { rhs = static_cast<double>(unpack<udon::float32_t>()); return *this; }
#else
#	error デシリアライズを行うには udon::float32_t が定義されている必要があります。
#endif

		/// @brief manipulator
		Serializer& operator<< (void(*f)(std::vector<uint8_t>&)) { f(buffer); return *this; }
		Serializer& operator>> (void(*f)(std::vector<uint8_t>&)) { f(buffer); return *this; }

	private:

		/// @brief シリアル化
		/// @tparam Ty
		/// @param rhs シリアル化するオブジェクト
		template<class Ty>
		void pack(const Ty& rhs)
		{
			// オブジェクトをシリアル化し、バッファの後方に挿入
			buffer.insert(
				buffer.end(),
				reinterpret_cast<const uint8_t*>(&rhs),
				reinterpret_cast<const uint8_t*>(&rhs) + sizeof(Ty)
			);
		}

		/// @brief 逆シリアル化
		/// @remark オブジェクトはスカラ型である必要があります
		/// @tparam Ty
		/// @return 復元されたオブジェクト
		template<class Ty>
		Ty unpack()
		{
			Ty retval{};

			// 逆シリアル化されたオブジェクトをバッファの前方から抽出
			std::copy(
				buffer.begin(),
				buffer.begin() + sizeof(Ty),
				reinterpret_cast<uint8_t*>(&retval)
			);

			// 抽出した分、バッファを縮小
			buffer.erase(buffer.begin(), buffer.begin() + sizeof(Ty));

			return retval;
		}

	};

	/// @brief エンディアン変換マニピュレータ
	inline void ReverseEndian(std::vector<uint8_t>& buffer)
	{
		std::reverse(buffer.begin(), buffer.end());
	}

	/// @brief バイト列にシリアライズ
	/// @remark オブジェクトの型がシリアライズ用演算子を実装している必要があります
	/// @param rhs 変換するオブジェクト
	/// @param reverseEndian エンディアン変換を行うか
	/// @return バイト列
	template<class Ty>
	inline std::vector<uint8_t> Pack(const Ty& rhs, bool reverseEndian = false)
	{
		udon::Serializer builder;
		builder << rhs;

		if (reverseEndian)
		{
			builder << udon::ReverseEndian;
		}

		return builder.data();
	}

	/// @brief バイト列からデシリアライズ
	/// @remark オブジェクトの型がデシリアライズ用演算子を実装している必要があります
	/// @param buffer バイト列
	/// @param reverseEndian エンディアン変換を行うか
	/// @return 変換後のオブジェクト
	template<class Ty>
	inline Ty Unpack(const std::vector<uint8_t>& buffer, bool reverseEndian = false)
	{
		udon::Serializer builder{ buffer };

		if (reverseEndian)
		{
			builder >> udon::ReverseEndian;
		}

		Ty retval;
		builder >> retval;
		return retval;
	}

}
