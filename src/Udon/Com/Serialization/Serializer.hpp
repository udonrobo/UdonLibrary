//
//    シリアライザ
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include "SerializerImpl.hpp"
#include "SerializedSize.hpp"
#include <Udon/Types/ArrayView.hpp>

namespace Udon
{

    /// @brief バッファにシリアル化する
    /// @tparam T シリアル化する型(自動推論)
    /// @param object シリアル化するオブジェクト
    /// @param buffer シリアル化先のバッファ
    /// @return シリアル化に成功したかどうか
    template <typename T>
    bool Serialize(const T& object, ArrayView<uint8_t> buffer)
    {
        // コンパイル時型チェック
        static_assert(Traits::IsSerializable<T>::value, "T must be serializable type.");    // T はシリアライズ可能な型である必要があります。T クラス内で UDON_ENUMERABLE(...) マクロにメンバ変数をセットすることで、シリアライズ可能になります。

        // バッファサイズチェック
        if (buffer.size() != SerializedSize<T>())
        {
            return false;
        }

        // チェックサムを除いた領域を取得
        const auto dataView = buffer.removeBackView(Udon::CRC8_SIZE);

        // シリアライズ
        Impl::Serializer serializer{ dataView };
        serializer(object);

        // チェックサム挿入
        buffer.back() = Udon::CRC8(dataView);

        return true;
    }

    /// @brief シリアル化する
    /// @tparam T シリアル化する型(自動推論)
    /// @param object シリアル化するオブジェクト
    /// @return シリアル化したデータ
    template <typename T>
    std::vector<uint8_t> Serialize(const T& object)
    {
        static_assert(Traits::IsSerializable<T>::value, "T must be serializable type.");    // T はシリアライズ可能な型である必要があります。T クラス内で UDON_ENUMERABLE(...) マクロにメンバ変数をセットすることで、シリアライズ可能になります。

        std::vector<uint8_t> buffer(SerializedSize<T>());

        Serialize(object, buffer);

        return buffer;
    }

    /// @brief 必ずチェックサムエラーとなるシリアライズ
    /// @tparam T シリアル化する型(自動推論)
    /// @param buffer シリアル化先のバッファ
    /// @return シリアル化に成功したかどうか
    inline void FailableSerialize(ArrayView<uint8_t> buffer)
    {
        buffer.back() = CRC8(buffer.removeBackView(Udon::CRC8_SIZE)) ^ 0xff;
    }

}    // namespace Udon
