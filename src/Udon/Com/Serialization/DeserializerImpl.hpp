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
//    Udon::Deserialize<Type>(byte[]) を使用してください。(Udon/Com/Serialization/Deserializer.hpp)
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <vector>
#include <limits.h>

#include <Udon/Algorithm/CRC.hpp>
#include <Udon/Algorithm/Bit.hpp>
#include <Udon/Stl/Optional.hpp>
#include <Udon/Types/Float.hpp>
#include <Udon/Traits/Typedef.hpp>
#include <Udon/Common/Platform.hpp>

namespace Udon
{
    namespace Impl
    {

        class Deserializer
        {

            size_t  popIndex     = 0;    // 次に抽出するインデックス(バッファの先端からのオフセット)
            uint8_t boolCount    = 0;    // bool の抽出回数
            size_t  boolPopIndex = 0;    // bool を抽出中であるインデックス(バッファの先端からのオフセット)

            std::vector<uint8_t> buffer;

            bool isChecksumSuccess;

        public:
            using ResultType = void;

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

#if UDON_PLATFORM_ENDIANNESS == UDON_PLATFORM_BIG_ENDIAN
                std::reverse(buffer.begin(), buffer.end() - Udon::CRC8_SIZE);
#endif
            }

            /// @brief チェックサムが正しいか
            explicit operator bool() const
            {
                return isChecksumSuccess;
            }

            /// @brief デシリアライズ
            /// @param args デシリアライズ後のオブジェクトの代入先(一時オブジェクト不可)
            /// @remark const は外されます
            template <typename... Args>
            void operator()(const Args&... args) const
            {
                argsDeserialize(args...);
            }

        private:
            /// @brief 可変長テンプレート引数
            template <typename Head, typename... Tails>
            void argsDeserialize(const Head& head, const Tails&... tails) const
            {
                const_cast<Deserializer&>(*this).deserialize(const_cast<Head&>(head));
                // 関数を constexpr にするため、enumerate の引数は "const Deserializer& enumerator" になっている (一時オブジェクトを受けれるように)
                // enumerator (const *this) から deserialize を呼び出すため、const_cast で const を外している

                argsDeserialize(tails...);
            }

            /// @brief 可変長引数展開の終端
            void argsDeserialize() const {}

            /// @brief bool型
            template <typename Bool, EnableIfNullptrT<IsBool<RemoveReferenceT<Bool>>::value> = nullptr>
            void deserialize(Bool& rhs)
            {
                rhs = popBool();
            }

            /// @brief 整数型
            template <typename IntegralNotBool, EnableIfNullptrT<IsIntegralNotBool<RemoveReferenceT<IntegralNotBool>>::value> = nullptr>
            void deserialize(IntegralNotBool& rhs)
            {
                rhs = popArithmetic<IntegralNotBool>();
            }

            /// @brief 浮動小数点型
            template <typename FloatingPoint, EnableIfNullptrT<IsFloatingPoint<RemoveReferenceT<FloatingPoint>>::value> = nullptr>
            void deserialize(FloatingPoint& rhs)
            {
                rhs = popArithmetic<Udon::float32_t>();
            }

            /// @brief 列挙型
            template <typename Enum, EnableIfNullptrT<IsEnum<RemoveReferenceT<Enum>>::value> = nullptr>
            void deserialize(Enum& rhs)
            {
                rhs = static_cast<Enum>(popArithmetic<typename std::underlying_type<Enum>::type>());
            }

            /// @brief 配列型
            template <typename Array, EnableIfNullptrT<IsArray<RemoveReferenceT<Array>>::value> = nullptr>
            void deserialize(Array& rhs)
            {
                for (auto& element : rhs)
                {
                    deserialize(element);
                }
            }

            /// @brief メンバ変数列挙用の関数が定義されている型
            template <typename Enumerable, EnableIfNullptrT<HasMemberFunctionEnumerate<RemoveReferenceT<Enumerable>>::value> = nullptr>
            void deserialize(Enumerable& rhs)
            {
                rhs.enumerate(*this);
            }

            /// @brief アトミック (整数, 浮動小数点) 値として逆シリアル化
            template <typename T>
            T popArithmetic()
            {
                T unpacked;

                constexpr auto size = sizeof(T);

                // 逆シリアル化されたオブジェクトをバッファから抽出
#if UDON_PLATFORM_ENDIANNESS == UDON_PLATFORM_LITTLE_ENDIAN

                std::copy(
                    buffer.cbegin() + popIndex,
                    buffer.cbegin() + popIndex + size,
                    reinterpret_cast<uint8_t*>(&unpacked));

#elif UDON_PLATFORM_ENDIANNESS == UDON_PLATFORM_BIG_ENDIAN

                std::copy(
                    buffer.crbegin() + popIndex,
                    buffer.crbegin() + popIndex + size,
                    reinterpret_cast<uint8_t*>(&retval));

#endif

                // 抽出したオブジェクトのバイト数分インデックスを進める
                popIndex += size;

                return unpacked;
            }

            /// @brief bool値 (1bit) として逆シリアル化
            bool popBool()
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
    }    // namespace Impl

}    // namespace Udon