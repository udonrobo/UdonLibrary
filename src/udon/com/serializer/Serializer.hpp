#pragma once

#include <udon/types/Float.hpp>

#include <udon/stl/EnableSTL.hpp>

#include <vector>     // std::vector
#include <algorithm>  // std::copy

namespace udon
{

	/// @brief メンバだけのサイズを取得する
	class PackedSizeChecker
	{

		size_t length;

	public:

		PackedSizeChecker()
			: length()
		{}

		/// @brief スカラ型
		/// @param
		/// @return
		inline PackedSizeChecker& operator|(bool    ) { length += sizeof(bool           ); return *this; }
		inline PackedSizeChecker& operator|(int8_t  ) { length += sizeof(int8_t         ); return *this; }
		inline PackedSizeChecker& operator|(int16_t ) { length += sizeof(int16_t        ); return *this; }
		inline PackedSizeChecker& operator|(int32_t ) { length += sizeof(int32_t        ); return *this; }
		inline PackedSizeChecker& operator|(int64_t ) { length += sizeof(int64_t        ); return *this; }
		inline PackedSizeChecker& operator|(uint8_t ) { length += sizeof(uint8_t        ); return *this; }
		inline PackedSizeChecker& operator|(uint16_t) { length += sizeof(uint16_t       ); return *this; }
		inline PackedSizeChecker& operator|(uint32_t) { length += sizeof(uint32_t       ); return *this; }
		inline PackedSizeChecker& operator|(uint64_t) { length += sizeof(uint64_t       ); return *this; }
		inline PackedSizeChecker& operator|(double  ) { length += sizeof(udon::float32_t); return *this; }
		inline PackedSizeChecker& operator|(float   ) { length += sizeof(udon::float32_t); return *this; }

		/// @brief 配列型
		/// @remake 配列の要素がデシリアライズ可能である場合のみ、デシリアライズ可能です
		/// @tparam Ty
		/// @tparam N
		/// @param rhs
		/// @return
		template<class Ty, size_t N>
		inline PackedSizeChecker& operator|(Ty(&rhs)[N])
		{
			for (auto&& it : rhs)
			{
				*this | it;
			}
			return *this;
		}

		/// @brief アライメントを除去したメンバのサイズを取得する
		/// @return
		inline size_t size() const
		{
			return length;
		}

	};

	class Serializer
	{

		std::vector<uint8_t> buffer;

	public:

		Serializer() = default;

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
		inline Serializer& operator|(bool     rhs) { pack(rhs); return *this; }
		inline Serializer& operator|(int8_t   rhs) { pack(rhs); return *this; }
		inline Serializer& operator|(int16_t  rhs) { pack(rhs); return *this; }
		inline Serializer& operator|(int32_t  rhs) { pack(rhs); return *this; }
		inline Serializer& operator|(int64_t  rhs) { pack(rhs); return *this; }
		inline Serializer& operator|(uint8_t  rhs) { pack(rhs); return *this; }
		inline Serializer& operator|(uint16_t rhs) { pack(rhs); return *this; }
		inline Serializer& operator|(uint32_t rhs) { pack(rhs); return *this; }
		inline Serializer& operator|(uint64_t rhs) { pack(rhs); return *this; }

#ifdef UDON_HAS_FLOAT32
		inline Serializer& operator|(float    rhs) { pack(static_cast<udon::float32_t>(rhs)); return *this; }
		inline Serializer& operator|(double   rhs) { pack(static_cast<udon::float32_t>(rhs)); return *this; }
#else
#	error udon::float32_t must be defined for serialization.
#endif

		/// @brief 配列型シリアライズ
		/// @remake 配列の要素がシリアライズ可能である場合のみ、シリアライズ可能です
		/// @tparam Ty
		/// @tparam N
		/// @param rhs
		/// @return
		template<class Ty, size_t N>
		inline Serializer& operator|(Ty(&rhs)[N])
		{
			for (auto&& it : rhs)
			{
				*this | it;
			}
			return *this;
		}

		/// @brief マニピュレーター
		inline Serializer& operator|(void(*f)(std::vector<uint8_t>&))
		{
			f(buffer);
			return *this;
		}

	private:

		/// @brief シリアル化
		/// @remark オブジェクトはスカラ型である必要があります
		/// @tparam Ty
		/// @param rhs シリアル化するオブジェクト
		template<class Ty>
		inline void pack(const Ty& rhs)
		{
			// オブジェクトをシリアル化し、バッファの後方に挿入
			buffer.insert(
				buffer.end(),
				reinterpret_cast<const uint8_t*>(&rhs),
				reinterpret_cast<const uint8_t*>(&rhs) + sizeof(Ty)
			);
		}
	};

	class Deserializer
	{

		std::vector<uint8_t> buffer;

	public:

		/// @brief コンストラクタ
		/// @param buffer デシリアライするバイト列
		Deserializer(const std::vector<uint8_t>& buffer)
			: buffer(buffer)
		{}

		/// @brief スカラ型デシリアライズ
		/// @param rhs
		/// @return
		inline Deserializer& operator|(bool    & rhs) { rhs = unpack<bool    >(); return *this; }
		inline Deserializer& operator|(int8_t  & rhs) { rhs = unpack<int8_t  >(); return *this; }
		inline Deserializer& operator|(int16_t & rhs) { rhs = unpack<int16_t >(); return *this; }
		inline Deserializer& operator|(int32_t & rhs) { rhs = unpack<int32_t >(); return *this; }
		inline Deserializer& operator|(int64_t & rhs) { rhs = unpack<int64_t >(); return *this; }
		inline Deserializer& operator|(uint8_t & rhs) { rhs = unpack<uint8_t >(); return *this; }
		inline Deserializer& operator|(uint16_t& rhs) { rhs = unpack<uint16_t>(); return *this; }
		inline Deserializer& operator|(uint32_t& rhs) { rhs = unpack<uint32_t>(); return *this; }
		inline Deserializer& operator|(uint64_t& rhs) { rhs = unpack<uint64_t>(); return *this; }

#ifdef UDON_HAS_FLOAT32
		inline Deserializer& operator|(float& rhs) { rhs = static_cast<float>(unpack<udon::float32_t>()); return *this; }
		inline Deserializer& operator|(double& rhs) { rhs = static_cast<double>(unpack<udon::float32_t>()); return *this; }
#else
#	error udon::float32_t must be defined for serialization.
#endif

		/// @brief 配列型デシリアライズ
		/// @remake 配列の要素がデシリアライズ可能である場合のみ、デシリアライズ可能です
		/// @tparam Ty
		/// @tparam N
		/// @param rhs
		/// @return
		template<class Ty, size_t N>
		inline Deserializer& operator|(Ty(&rhs)[N])
		{
			for (auto&& it : rhs)
			{
				*this | it;
			}
			return *this;
		}


		/// @brief マニピュレーター
		inline Deserializer& operator|(void(*f)(std::vector<uint8_t>&))
		{
			f(buffer);
			return *this;
		}

	private:

		/// @brief 逆シリアル化
		/// @remark オブジェクトはスカラ型である必要があります
		/// @tparam Ty
		/// @return 復元されたオブジェクト
		template<class Ty>
		inline Ty unpack()
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

		udon::PackedSizeChecker sizeChecker;

		// メンバのサイズ取得
		builder.reserve((sizeChecker | const_cast<Ty&>(rhs)).size());

		// シリアライズ
		builder | const_cast<Ty&>(rhs);

		// エンディアン変換
		if (reverseEndian)
		{
			builder | udon::ReverseEndian;
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
		udon::Deserializer builder{ buffer };

		// エンディアン変換
		if (reverseEndian)
		{
			builder | udon::ReverseEndian;
		}

		Ty retval;

		// デシリアライズ
		builder | retval;

		return retval;
	}

}
