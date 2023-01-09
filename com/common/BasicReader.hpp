/// @file   BasicReader.hpp
/// @date   2022/12/11
/// @brief  受信クラスへ派生することで各種関数を提供するクラス
/// @author 大河 祐介

#pragma once

#ifdef ENABLE_STL
#	include <iostream>
#endif

template<size_t Size>
class BasicReader {

		const uint8_t (&buffer)[Size];

	public:

		/// @brief 送信クラスのバッファをセット
		/// @param buffer 送信クラスのバッファ配列に対する参照
		BasicReader(const uint8_t (&buffer)[Size])
			: buffer(buffer)
		{}

		/// @brief バッファサイズ
		static constexpr size_t size = Size;

		/// @brief 所定の番地の1byteを取得
		/// @param index バイト番地
		inline constexpr uint8_t getByteData(const size_t index) const {
			return buffer[index];
		}

		/// @brief 所定の番地の1bitを取得
		/// @param byteIndex バイト番地
		/// @param bitIndex  ビット番地
		inline constexpr void getBitData(const size_t byteIndex, const size_t bitIndex) {
			return bitRead(getByteData(byteIndex), bitIndex);
		}

		/// @brief 構造体として取得
		/// @details 構造体のバイト数と設定バイト数を合わせる(コンパイル時エラーが投げられます)
		template<class Ty>
		inline Ty getMessage() const {
			static_assert(sizeof(Ty) == Size, "Different from the number of bytes set.");
			Ty retval;
			memcpy(&retval, buffer, Size);
			return retval;
		}

		/// @brief 内部バッファにアクセス
		inline constexpr const uint8_t&
		operator[](uint8_t index) const {
			return buffer[index];
		}

		/// @brief  バッファのconst参照を取得
		/// @return バッファのconst参照
		inline constexpr const uint8_t (&data()const) [Size] {
			return buffer;
		}

		/// @brief セット値出力
		/// @param end 最後に出力される文字
		void show(const char end = {}) const {
			for (const auto& buf : buffer)
				Serial.print(buf), Serial.print('\t');
			Serial.print(end);
		}

#ifdef ENABLE_STL
		friend std::ostream& operator<<(std::ostream& ostm, const BasicReader& r) {
			for (const auto& buf : r.buffer) {
				ostm << buf << '\t';
			}
			return ostm;
		}
#endif

};
