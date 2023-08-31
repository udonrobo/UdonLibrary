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
//    デシリアライザ
//
//    シリアライズされたバイト列をデシリアライズしオブジェクトに復元します。
//    このクラスはユーザーが直接使用することはありません。
//    Udon::Unpack<Type>(byte[]) を使用してください。(Udon/Com/Serialization/Unpack.hpp)
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <vector>
#include <limits.h>

#include <Udon/Algorithm/CRC.hpp>
#include <Udon/Algorithm/Endian.hpp>
#include <Udon/Algorithm/Bit.hpp>
#include <Udon/Stl/Optional.hpp>
#include <Udon/Types/Float.hpp>
#include <Udon/Traits/Accessible.hpp>

namespace Udon
{

    class Deserializer
    {

        size_t  popIndex     = 0;    // 次に抽出するインデックス(バッファの先端からのオオフセット)
        uint8_t boolCount    = 0;    // bool の抽出回数
        size_t  boolPopIndex = 0;    // bool を抽出中であるインデックス(バッファの先端からのオオフセット)

        std::vector<uint8_t> buffer;

        bool isChecksumSuccess;

    public:
        /// @brief コンストラクタ
        /// @param buffer デシリアライするバイト列
        Deserializer(const std::vector<uint8_t>& buf)
            : buffer(buf)
        {

            // チャックサム確認
            const auto checksum = Udon::CRC8(buffer.cbegin(), buffer.cend() - Udon::CRC8_SIZE);
            isChecksumSuccess   = buffer.back() == checksum;

            if (not isChecksumSuccess)
            {
                return;
            }

#ifdef UDON_BIG_ENDIAN
            std::reverse(buffer.begin(), buffer.end() - Udon::CRC8_SIZE);
#endif
        }

        /// @brief チェックサムが正しいか
        explicit operator bool() const
        {
            return isChecksumSuccess;
        }

        /// @brief デシリアライズ
        template <typename... Args>
        void operator()(Args&... args)
        {
            argumentUnpack(args...);
        }

    private:
        /// @brief bool型
        UDON_CONCEPT_BOOL
        inline void deserialize(Bool& rhs)
        {
            rhs = deserializeBool();
        }

        /// @brief 整数型
        UDON_CONCEPT_INTEGRAL_NOT_BOOL
        inline void deserialize(IntegralNotBool& rhs)
        {
            rhs = deserializeArithmetic<IntegralNotBool>();
        }

        /// @brief 浮動小数点型
        UDON_CONCEPT_FLOATING_POINT
        inline void deserialize(FloatingPoint& rhs)
        {
            rhs = deserializeArithmetic<Udon::float32_t>();
        }

        /// @brief 配列型
        UDON_CONCEPT_ARRAY
        inline void deserialize(Array& rhs)
        {
            for (auto& element : rhs)
            {
                deserialize(element);
            }
        }

        /// @brief メンバ変数列挙用の関数が定義されている型
        UDON_CONCEPT_ACCESSIBLE
        inline void deserialize(Accessible& rhs)
        {
            Udon::Traits::InvokeAccessor(*this, rhs);
        }

        /// @brief 可変長テンプレート引数
        template <typename Head, typename... Tails>
        inline void argumentUnpack(Head& head, Tails&... tails)
        {
            argumentUnpack(head);
            argumentUnpack(tails...);
        }

        /// @brief 可変長引数展開
        template <typename T>
        inline void argumentUnpack(T& head)
        {
            deserialize(head);
        }

        /// @brief 可変長引数展開の終端
        inline void argumentUnpack()
        {
        }

        /// @brief アトミック (整数, 浮動小数点) 値のデシリアライズ
        template <class T>
        T deserializeArithmetic()
        {
            T unpacked;

            constexpr auto size = sizeof(T);

            // 逆シリアル化されたオブジェクトをバッファから抽出
#if defined(UDON_LITTLE_ENDIAN)
            std::copy(
                buffer.cbegin() + popIndex,
                buffer.cbegin() + popIndex + size,
                reinterpret_cast<uint8_t*>(&unpacked));
#elif defined(UDON_BIG_ENDIAN)
            std::copy(
                buffer.crbegin() + popIndex,
                buffer.crbegin() + popIndex + size,
                reinterpret_cast<uint8_t*>(&retval));
#endif

            // 抽出したオブジェクトのバイト数分インデックスを進める
            popIndex += size;

            return unpacked;
        }

        /// @brief bool値の逆シリアル化
        bool deserializeBool()
        {
            if (boolCount == 0)
            {
                boolPopIndex = popIndex++;
            }

            const auto unpacked = Udon::BitRead(buffer.at(boolPopIndex), boolCount);

            if (++boolCount >= CHAR_BIT)
            {
                boolCount = 0;
            }

            return unpacked;
        }
    };

}    // namespace Udon