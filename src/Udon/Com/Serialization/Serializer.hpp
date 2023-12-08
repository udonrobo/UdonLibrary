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
//    シリアライザ
//
//-------------------------------------------------------------------

#pragma once

#include "SerializerImpl.hpp"
#include "SerializedSize.hpp"
#include <Udon/Types/ArrayView.hpp>

namespace Udon
{

    /// @brief シリアル化する
    /// @tparam T シリアル化する型(自動推論)
    /// @param object シリアル化するオブジェクト
    /// @return シリアル化したデータ
    template <typename T>
    std::vector<uint8_t> Serialize(const T& object)
    {
        static_assert(Traits::IsSerializable<T>::value, "T must be serializable type.");  // T はシリアライズ可能な型である必要があります。T クラス内で UDON_ENUMERABLE(...) マクロにメンバ変数をセットすることで、シリアライズ可能になります。

        Impl::Serializer serializer(SerializedSize<T>());
        serializer(object);

        return serializer.flush();
    }

    /// @brief バッファにシリアル化する
    /// @tparam T シリアル化する型(自動推論)
    /// @param object シリアル化するオブジェクト
    /// @param buffer シリアル化したデータを格納するバッファ
    /// @param size バッファのサイズ
    /// @return シリアル化に成功したかどうか
    template <typename T>
    bool Serialize(const T& object, ArrayView<uint8_t> buffer)
    {
        static_assert(Traits::IsSerializable<T>::value, "T must be serializable type.");  // T はシリアライズ可能な型である必要があります。T クラス内で UDON_ENUMERABLE(...) マクロにメンバ変数をセットすることで、シリアライズ可能になります。

        if (buffer.size() >= SerializedSize<T>())
        {
            const auto vector = Serialize(object);
            std::copy(vector.begin(), vector.end(), buffer.data());
            return true;
        }
        else
        {
            return false;
        }
    }

    /// @brief 必ずチェックサムエラーとなるシリアライズ
    /// @param buffer シリアル化したデータを格納するバッファ
    /// @param size バッファのサイズ
    /// @return シリアル化に成功したかどうか
    inline void FailableSerialize(ArrayView<uint8_t> buffer)
    {
        buffer.back() = CRC8(buffer.cbegin(), buffer.cend() - CRC8_SIZE) ^ 0xff;
    }

}    // namespace Udon
