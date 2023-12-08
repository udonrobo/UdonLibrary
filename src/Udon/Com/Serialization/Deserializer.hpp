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
//    デシリアライズ
//
//-------------------------------------------------------------------

#pragma once

#include "DeserializerImpl.hpp"

namespace Udon
{
    /// @brief 逆シリアル化可能かどうかを判定します
    /// @param buffer バイト列
    /// @return 逆シリアル化可能かどうか
    inline bool CanDeserialize(const std::vector<uint8_t>& buffer)
    {
        const auto checksum = Udon::CRC8(buffer.data(), buffer.size() - Udon::CRC8_SIZE);
        return buffer.back() == checksum;
    }

    /// @brief 逆シリアル化可能かどうかを判定します
    /// @param buffer バイト列
    /// @param size バイト列のサイズ
    /// @return 逆シリアル化可能かどうか
    inline bool CanDeserialize(const uint8_t* buffer, size_t size)
    {
        const auto checksum = Udon::CRC8(buffer, size - Udon::CRC8_SIZE);
        return buffer[size - 1] == checksum;
    }

    /// @brief 逆シリアル化可能かどうかを判定します
    /// @remark 配列の参照を引数にとります
    /// @tparam N バイト配列の要素数
    /// @param array バイト列
    /// @return 逆シリアル化可能かどうか
    template <size_t N>
    inline bool CanDeserialize(const uint8_t (&array)[N])
    {
        return CanDeserialize(array, N);
    }

    /// @brief バイト列からオブジェクトを逆シリアル化します
    /// @remark std::vector<uint8_t>を受け取ります。
    /// @tparam T 逆シリアル化する型
    /// @param buffer バイト列
    /// @return 逆シリアル化されたオブジェクト
    template <typename T>
    Udon::Optional<T> Deserialize(const std::vector<uint8_t>& buffer)
    {
        static_assert(Traits::IsDeserializable<T>::value, "T must be deserializable type.");    // T は逆シリアライズ可能な型である必要があります。T クラス内で UDON_PACKABLE マクロにメンバ変数をセットすることで、逆シリアライズ可能になります。

        if (buffer.size() < SerializedSize<T>())
        {
            return Udon::nullopt;
        }

        Impl::Deserializer deserializer(buffer);

        if (deserializer)
        {
            T retval;
            deserializer(retval);
            return retval;
        }
        else
        {
            return Udon::nullopt;
        }
    }

    /// @brief バイト列からオブジェクトを逆シリアル化します
    /// @remark 配列のポインタ、サイズを受け取ります。
    /// @tparam T 逆シリアル化する型
    /// @param buffer バイト列
    /// @param size バイト列のサイズ
    /// @return 逆シリアル化されたオブジェクト
    template <typename T>
    Udon::Optional<T> Deserialize(ArrayView<const uint8_t> buffer)
    {
        static_assert(Traits::IsDeserializable<T>::value, "T must be deserializable type.");    // T は逆シリアライズ可能な型である必要があります。T クラス内で UDON_PACKABLE マクロにメンバ変数をセットすることで、逆シリアライズ可能になります。

#ifdef __AVR_ATmega328P__
        // todo:
        // Arduino nano さん、なぜかイテレーターベースコピーできない
        // UdonArduinoSTLで使用しているEASTL内の vector::allocate() で処理落ち
        // std::vector<uint8_t> v(buffer, buffer + size);
        std::vector<uint8_t> v;
        v.reserve(buffer.size());
        for (size_t i = 0; i < buffer.size(); ++i)
        {
            v.push_back(buffer.at(i));
        }
        // v.resize(size);
        // std::copy(buffer, buffer + size, v.begin());
        return Deserialize<T>(v);
#else
        return Deserialize<T>(std::vector<uint8_t>(buffer.cbegin(), buffer.cend()));
#endif
    }

}    // namespace Udon
