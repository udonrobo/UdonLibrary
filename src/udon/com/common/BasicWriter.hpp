/// @file   BasicWriter.hpp
/// @date   2022/12/11
/// @brief  送信クラスへ派生することで各種関数を提供するクラス
/// @author 大河 祐介

#pragma once

#ifdef UDON_ENABLE_IOSTREAM
#include <iostream>
#endif

namespace udon
{

    template <size_t Size>
    class BasicWriter
    {

        uint8_t (&buffer)[Size];

    public:
        /// @brief 送信クラスのバッファをセット
        /// @param buffer 送信クラスのバッファ配列に対する参照
        BasicWriter(uint8_t (&buffer)[Size])
            : buffer(buffer)
        {
        }

        /// @brief バッファサイズ
        static constexpr size_t size = Size;

        /// @brief 配列をセット
        /// @details 配列のバイト数と設定バイト数を合わせる
        /// @param value 配列
        inline constexpr void
        setArrayData(uint8_t (&value)[Size])
        {
            memcpy(buffer, &value, Size);
        }

        /// @brief 所定の番地に1byteセット
        /// @param index バイト番地
        /// @param value 値 (0 ~ 255)
        inline constexpr void
        setByteData(const size_t index, const uint8_t value)
        {
            buffer[index] = value;
        }

        /// @brief 所定の番地に1bitセット
        /// @param byteIndex バイト番地
        /// @param bitIndex  ビット番地
        /// @param value     値 (true, false)
        inline constexpr void
        setBitData(const size_t byteIndex, const size_t bitIndex, const bool value)
        {
            bitWrite(buffer[byteIndex], bitIndex, value);
        }

        /// @brief 送信する構造体をセット
        /// @details インスタンスのバイト数と設定バイト数を合わせる(コンパイル時エラーが投げられます)
        /// @tparam Ty    構造体の型
        /// @param  value 送信する構造体インスタンス
        template <class Ty>
        inline constexpr void
        setMessage(const Ty& value)
        {
            static_assert(sizeof(Ty) == Size, "Different from the number of bytes set.");
            memcpy(buffer, &value, Size);
        }

        /// @brief 内部バッファにアクセス
        inline constexpr uint8_t&
        operator[](uint8_t index)
        {
            return buffer[index];
        }

        /// @brief バッファの参照を取得
        /// @return バッファの参照
        inline constexpr uint8_t (&data())[Size]
        {
            return buffer;
        }

        /// @brief セット値出力
        /// @param end 最後に出力される文字
        void
        show(const char end = {}) const
        {
            for (const auto& buf : buffer)
                Serial.print(buf), Serial.print('\t');
            Serial.print(end);
        }

#ifdef UDON_ENABLE_IOSTREAM
        friend std::istream& operator>>(std::istream& istm, BasicWriter& r)
        {
            for (auto& buf : r.buffer)
            {
                istm >> buf;
            }
            return istm;
        }
        friend std::ostream& operator<<(std::ostream& ostm, const BasicWriter& r)
        {
            for (const auto& buf : r.buffer)
            {
                ostm << buf << '\t';
            }
            return ostm;
        }
#endif
    };

}    // namespace udon
