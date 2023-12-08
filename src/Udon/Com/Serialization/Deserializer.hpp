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
    /// @param buffer 判定するバイト列の参照
    /// @return 逆シリアル化可能かどうか
    inline bool CanDeserialize(ArrayView<const uint8_t> buffer)
    {
        return buffer.back() == Udon::CRC8(buffer.removeBackView(Udon::CRC8_SIZE));
    }

    /// @brief バイト列からオブジェクトに逆シリアル化します
    /// @param buffer バイト列
    /// @return 逆シリアル化されたオブジェクト
    template <typename T>
    Udon::Optional<T> Deserialize(ArrayView<const uint8_t> buffer)
    {
        // コンパイル時型チェック
        static_assert(Traits::IsDeserializable<T>::value, "T must be deserializable type.");    // T は逆シリアライズ可能な型である必要があります。T クラス内で UDON_PACKABLE マクロにメンバ変数をセットすることで、逆シリアライズ可能になります。

        // バッファサイズチェック
        if (buffer.size() != SerializedSize<T>())
        {
            return Udon::nullopt;
        }

        // チェックサムチェック
        if (not CanDeserialize(buffer))
        {
            return Udon::nullopt;
        }

        T retval;
        Impl::Deserializer deserializer(buffer.removeBackView(Udon::CRC8_SIZE));
        deserializer(retval);

        return retval;
    }

}    // namespace Udon
