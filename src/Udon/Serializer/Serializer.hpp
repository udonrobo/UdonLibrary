//
//    シリアライザ
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#include "Impl/Serializer.hpp"
#include "Impl/Deserializer.hpp"
#include "Impl/SerializedSize.hpp"
#include <Udon/Traits/EnumerableMacro.hpp>
#include <Udon/Types/ArrayView.hpp>

namespace Udon
{

    /// @brief Tをシリアライズした際のバイト列の要素数を取得する
    /// @tparam T シリアライズ対象の型
    /// @details シリアライズ後のサイズは、チェックサムのサイズを含む
    template <typename T>
    constexpr size_t SerializedSize() noexcept
    {
        static_assert(Traits::IsSerializable<T>::value, "T is not packed sizable");    // T は "シリアライズ後のサイズを取得可能な型" である必要がある

        return Udon::Ceil(Impl::SerializedBitSizeImpl{}(T{}) / static_cast<double>(CHAR_BIT)) + Udon::CRC8_SIZE;
    }

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
        static_assert(Traits::IsSerializable<T>::value, "T must be deserializable type.");    // T は逆シリアライズ可能な型である必要があります。T クラス内で UDON_PACKABLE マクロにメンバ変数をセットすることで、逆シリアライズ可能になります。

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

        T retval{};

        Impl::Deserializer deserializer(buffer.removeBackView(Udon::CRC8_SIZE));
        deserializer(retval);

        return retval;
    }

}    // namespace Udon
