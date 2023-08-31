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

#include "Serializer.hpp"

namespace Udon
{

    /// @brief シリアル化する
    /// @tparam T シリアル化する型(自動推論)
    /// @param object シリアル化するオブジェクト
    /// @return シリアル化したデータ
    template <typename T>
    inline std::vector<uint8_t> Pack(const T& object)
    {
//        static_assert(Udon::Traits::Parsable<T>::value, "T must be parsable type.");    // Tはパース可能である必要があります。クラス内で UDON_PACKABLE マクロを使用することで、パース可能であることを宣言できます。

        Serializer serializer(Udon::CapacityWithChecksum<T>());
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
    inline bool Pack(const T& object, uint8_t* buffer, size_t size)
    {
        static_assert(Udon::Traits::Parsable<T>::value, "T must be parsable type.");    // Tはパース可能である必要があります。クラス内で UDON_PACKABLE マクロを使用することで、パース可能であることを宣言できます。

        if (size >= Udon::CapacityWithChecksum<T>())
        {
            const auto vector = Pack(object);
            std::copy(vector.begin(), vector.end(), buffer);
            return true;
        }
        else
        {
            return false;
        }
    }

    /// @brief バッファにシリアル化する
    /// @tparam T シリアル化する型(自動推論)
    /// @param object シリアル化するオブジェクト
    /// @param array シリアル化したデータを格納するバッファ
    /// @remark バッファのサイズはCapacityWithChecksum関数で取得したサイズ以上である必要があります。
    /// @return シリアル化に成功したかどうか
    template <typename T, size_t N>
    inline bool Pack(const T& object, uint8_t (&array)[N])
    {
        static_assert(Udon::Traits::Parsable<T>::value, "T must be parsable type.");    // Tはパース可能である必要があります。クラス内で UDON_PACKABLE マクロを使用することで、パース可能であることを宣言できます。

        return Pack(object, array, N);
    }

}    // namespace Udon
