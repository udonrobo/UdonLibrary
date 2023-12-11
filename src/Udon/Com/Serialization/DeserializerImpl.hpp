//
//    デシリアライザ実装部
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//

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
#include <Udon/Types/ArrayView.hpp>

namespace Udon
{
    namespace Impl
    {

        class Deserializer
        {

            size_t  popIndex     = 0;    // 次に抽出するインデックス(バッファの先端からのオフセット)
            uint8_t boolCount    = 0;    // bool の抽出回数
            size_t  boolPopIndex = 0;    // bool を抽出中であるインデックス(バッファの先端からのオフセット)

            ArrayView<const uint8_t> buffer;    // デシリアライズするバイト列への参照

        public:
            using ResultType = void;

            /// @brief コンストラクタ
            /// @param buffer デシリアライするバイト列
            Deserializer(ArrayView<const uint8_t> buffer)
                : buffer(buffer)
            {
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

            /// @brief 算術型 (整数, 浮動小数点) として逆シリアル化
            template <typename Arithmetic>
            Arithmetic popArithmetic()
            {
                Arithmetic object;

                constexpr auto size = sizeof(Arithmetic);

                // 逆シリアル化されたオブジェクトをバイト列から抽出
                // リトルエンディアン環境でのバイト列の並びを順とする
#if UDON_PLATFORM_ENDIANNESS == UDON_PLATFORM_LITTLE_ENDIAN

                // buffer をオブジェクトとして解釈
                memcpy(
                    std::addressof(object),
                    buffer.data() + popIndex,
                    size);

#elif UDON_PLATFORM_ENDIANNESS == UDON_PLATFORM_BIG_ENDIAN

                // リトルエンディアンが基準なので、逆順のバイト列を作成
                uint8_t reversedBuffer[size];
                std::reverse_copy(
                    buffer.cbegin() + popIndex,
                    buffer.cbegin() + popIndex + size,
                    reversedBuffer);

                // reversedBuffer をオブジェクトとして解釈
                memcpy(
                    std::addressof(object),
                    reversedBuffer,
                    size);

#endif

                // 抽出したオブジェクトのバイト数分インデックスを進める
                popIndex += size;

                return object;
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