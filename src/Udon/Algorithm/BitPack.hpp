#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>
#include <iterator>
#include <vector>

#include "Bit.hpp"
#include "Math.hpp"

namespace Udon
{

    /// @brief バイト列のMSB(最上位ビット)から順に7bitずつ分割してコールバック関数に渡す
    /// @tparam Functor
    /// @tparam InputIterator
    /// @param begin
    /// @param end
    /// @param callback
    /// @note
    ///		コールバック関数を用いることで、新たにバッファを確保する必要がない
    ///		イテレーターから受け取ったデータは次のように変換されコールバック関数に渡される
    ///		[data(8bit)], [data(8bit)], ... -> [data(7bit)]*7, [各dataのMSB(7bit)], [data(7bit)]*7, [各dataのMSB(7bit)], ...
    /// @return
    template <typename InputIterator, typename Functor>
    void BitPack(const InputIterator begin, const InputIterator end, Functor callback)
    {
        static_assert(std::is_same<typename std::iterator_traits<InputIterator>::value_type, uint8_t>::value, "container element type must be uint8_t");
        static_assert(std::is_same<typename std::result_of<Functor(uint8_t)>::type, void>::value, "callback function must be void(uint8_t)");

        uint8_t extracteMsb   = 0;    // 各要素のMSB(最上位ビット)から抽出された分のデータ
        uint8_t extracteCount = 0;    // extracteMsbに格納されたデータのビット数

        bool isFirst = true;

        for (auto it = begin; it != end; ++it)
        {
            BitWrite(extracteMsb, extracteCount, BitRead(*it, 7));    // MSBを抽出してextracteMsbに格納する

            if (isFirst)
            {
                callback(*it | 0b10000000);    // 先頭のデータはMSBが1
            }
            else
            {
                isFirst = false;
                callback(*it & 0b01111111);    // MSBを除いたデータをコールバック関数に渡す
            }

            if (++extracteCount >= 7)
            {
                extracteCount = 0;
                callback(extracteMsb);
                extracteMsb = {};
            }
        }

        if (extracteCount)    // extracteMsbにデータが残っていれば追加で渡す
        {
            callback(extracteMsb);
        }
    }

    /// @brief  7bit分割されたデータを結合する
    /// @tparam Functor
    /// @tparam OutputIterator
    /// @param begin
    /// @param end
    /// @param callback
    /// @note
    ///     アンパックが失敗する場合、
    ///     7bit分割されたデータは次のように変換されコールバック関数に渡される
    ///     [data(7bit)]*7, [各dataのMSB(7bit)], [data(7bit)]*7, [各dataのMSB(7bit)], ... -> [data(8bit)], [data(8bit)], ...
    /// @return
    template <typename OutputIterator, typename Functor>
    bool BitUnpack(OutputIterator begin, OutputIterator end, Functor callback)
    {
        static_assert(std::is_same<typename std::iterator_traits<OutputIterator>::value_type, uint8_t>::value, "container element type must be uint8_t");
        static_assert(std::is_same<typename std::result_of<Functor()>::type, uint8_t>::value, "callback function must be uint8_t()");

        uint8_t extracteCount = 0;    // 通常MSBなしデータの取得数

        bool isFirst = true;

        for (auto it = begin; it != end;)
        {
            const auto data = callback();

            // バイト列の先頭のMSBは必ず 1
            if (isFirst && not(data & 0b10000000))
            {
                return false;
            }
            else
            {
                isFirst = false;
            }

            *it = data;
            ++it;
            ++extracteCount;

            if (extracteCount == 7)    // 7バイトごとにMSBの集合が来る
            {
                extracteCount = 0;

                const uint8_t msb = callback();

                auto c = it;

                for (int i = 0; i < 7; ++i)
                {
                    --c;                                                     // c は最初、MSBの集合を指しているので、先に1バイト戻す
                    BitWrite(*c, 7, BitRead(msb, 7 - 1 /*MSB bit*/ - i));    // 各要素のMSBを復元
                }
            }
        }

        if (extracteCount)    // パック時	extracteMsb にデータが残っている場合、追加でデータが渡されるのでそれを処理する
        {
            const uint8_t msb = callback();

            auto c = end;

            for (int i = 0; i < extracteCount; ++i)
            {
                --c;                                                                   // end は領域を持たないので、先に1バイト戻す
                BitWrite(*(c), 7, BitRead(msb, extracteCount - 1 /*MSB bit*/ - i));    // 各要素の要素のMSBを復元
            }
        }

        return true;
    }

    /// @brief ビットパックされたデータのサイズを取得する
    /// @param size ビットパックされる前のデータのサイズ
    /// @return ビットパック後のデータのサイズ
    inline constexpr size_t BitPackedSize(size_t size)
    {
        return (size_t)Udon::Ceil(size * 8 / 7.0);    // 切り上げ
    }

    /// @brief ビットアンパックされたデータのサイズを取得する
    /// @param size ビットアンパックされる前のデータのサイズ
    /// @return ビットアンパック後のデータのサイズ
    inline constexpr size_t BitUnpackedSize(size_t size)
    {
        return size * 7 / 8;    // 切り捨て
    }

    /// @brief バイト列の
    /// @param data
    /// @return
    inline std::vector<uint8_t> BitPack(const std::vector<uint8_t>& data)
    {
        std::vector<uint8_t> result(BitPackedSize(data.size()));

        auto it = result.begin();

        BitPack(data.begin(), data.end(), [&it](uint8_t data)
                { *it++ = data; });

        return result;
    }

    inline std::vector<uint8_t> BitUnpack(const std::vector<uint8_t>& data)
    {
        std::vector<uint8_t> result(BitUnpackedSize(data.size()));

        auto it = data.begin();

        BitUnpack(result.begin(), result.end(), [&it]() -> uint8_t
                  { return *it++; });

        return result;
    }

}    // namespace Udon