//
//    シリアライザ実装部
//
//    Copyright (c) 2022 udonrobo
//

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <algorithm>
#include <vector>
#include <limits.h>

#include <Udon/Algorithm/CRC.hpp>
#include <Udon/Algorithm/Bit.hpp>
#include <Udon/Types/Float.hpp>
#include <Udon/Types/ArrayView.hpp>
#include <Udon/Traits/Typedef.hpp>
#include <Udon/Traits/AlwaysFalse.hpp>
#include <Udon/Serializer/SerializerTraits.hpp>
#include <Udon/Utility/Platform.hpp>

namespace Udon
{

    namespace Impl
    {
        using namespace Udon::Traits;


        /// @brief シリアライザ実装部
        class Serializer
        {

            size_t pushIndex = 0;          // 次に挿入するインデックス(バッファの先端からのオフセット)
            uint8_t boolCount = 0;         // bool の挿入回数
            size_t boolInsertIndex = 0;    // bool を挿入中であるインデックス(バッファの先端からのオフセット)

            ArrayView<uint8_t> buffer;    // シリアライズしたデータを格納するバッファへの参照

        public:
            using ResultType = void;

            /// @brief コンストラクタ
            /// @param buffer シリアライズ後のデータの書き込み先バッファ
            explicit Serializer(ArrayView<uint8_t> buffer) noexcept
                : buffer(buffer)
            {
            }

            /// @brief シリアライズ
            /// @param args シリアライズするオブジェクト列
            template <typename... Args>
            void enumeration(const Args&... args)
            {
                argsUnpack(args...);
            }

        private:
            // 可変長引数を展開する

            /// @brief 可変長引数展開
            template <typename Head, typename... Tail>
            void argsUnpack(const Head& head, const Tail&... tail)
            {
                Wrapper<Head>::Serialize(*this, head);
                argsUnpack(tail...);
            }

            /// @brief 可変長引数展開の終端
            void argsUnpack() {}

        private:
            // 部分特殊化を用いて型を判別し、シリアライズ用関数に振り分ける
            // 部分特殊化を用いると、先にすべての条件を満たさない場合の構造体を定義できるため、エラーを出しやすくなる。
            // またメンバ関数は部分特殊化できないため、構造体でラップしている

            /// @brief シリアライズできない型
            /// @note この構造体がインスタンス化されると、static_assert によりコンパイルエラーとなる
            template <typename T, typename = void>
            struct Wrapper
            {
                static_assert(AlwaysFalse<T>::value, "Type is not serializable");
            };

            /// @brief bool 型
            template <typename T>
            struct Wrapper<T, typename std::enable_if<std::is_same<RemoveModifierT<T>, bool>::value>::type>
            {
                static void Serialize(Serializer& self, const T& value)
                {
                    self.pushBool(value);
                }
            };

            /// @brief 整数型
            template <typename T>
            struct Wrapper<T, typename std::enable_if<std::is_integral<RemoveModifierT<T>>::value &&
                                                      not std::is_same<RemoveModifierT<T>, bool>::value>::type>
            {
                static void Serialize(Serializer& self, const T& value)
                {
                    self.pushArithmetic(value);
                }
            };

            /// @brief 浮動小数点型
            template <typename T>
            struct Wrapper<T, typename std::enable_if<std::is_floating_point<RemoveModifierT<T>>::value>::type>
            {
                static void Serialize(Serializer& self, const T& value)
                {
                    // 浮動小数点型は32ビットの浮動小数点数としてシリアライズする
                    self.pushArithmetic(static_cast<Udon::Float32>(value));
                }
            };

            /// @brief ユーザー定義型
            template <typename T>
            struct Wrapper<T, typename std::enable_if<Traits::IsEnumerable<RemoveModifierT<T>>::value>::type>
            {
                static void Serialize(Serializer& self, const T& value)
                {
                    value.enumerateMutable(self);
                }
            };

            /// @brief 列挙型
            template <typename Enum>
            struct Wrapper<Enum, typename std::enable_if<std::is_enum<RemoveModifierT<Enum>>::value>::type>
            {
                static void Serialize(Serializer& self, const Enum& e)
                {
                    // 列挙型の場合、基底型に変換してシリアル化する
                    // 例えば enum E : uint8_t {}; の場合 uint8_t 型としてシリアル化される
                    using UnderlyingType = typename std::underlying_type<RemoveModifierT<Enum>>::type;
                    self.pushArithmetic(static_cast<UnderlyingType>(e));
                }
            };

            /// @brief 配列型
            template <typename T>
            struct Wrapper<T, typename std::enable_if<IsArray<RemoveModifierT<T>>::value>::type>
            {
                static void Serialize(Serializer& self, const T& value)
                {
                    for (const auto& element : value)
                    {
                        Wrapper<decltype(element)>::Serialize(self, element);
                    }
                }
            };

        private:
            // 実際にシリアライズする関数群

            /// @brief 算術型 (整数, 浮動小数点) のシリアル化
            template <typename Arithmetic>
            void pushArithmetic(Arithmetic arithmetic)
            {
                constexpr auto size = sizeof(Arithmetic);

                // バッファの後方に挿入
                // リトルエンディアン環境でのバイト列の並びを順とする
#if UDON_PLATFORM_ENDIANNESS == UDON_PLATFORM_LITTLE_ENDIAN

                // オブジェクトをバイト列として解釈
                memcpy(
                    buffer.data() + pushIndex,
                    std::addressof(arithmetic),
                    size);

#elif UDON_PLATFORM_ENDIANNESS == UDON_PLATFORM_BIG_ENDIAN

                // オブジェクトをバイト列として解釈
                uint8_t reversedBuffer[size];
                memcpy(
                    reversedBuffer,
                    std::addressof(arithmetic),
                    size);

                // リトルエンディアンが基準なので、逆順にしてバッファに挿入
                std::reverse_copy(
                    reversedBuffer,
                    reversedBuffer + size,
                    buffer.begin() + pushIndex);

#endif

                // 次に挿入するインデックスを更新
                pushIndex += size;
            }

            /// @brief bool値のシリアル化
            void pushBool(bool rhs)
            {
                if (boolCount == 0)
                {
                    boolInsertIndex = pushIndex++;
                }

                Udon::BitWrite(buffer.at(boolInsertIndex), boolCount, rhs);

                if (++boolCount >= CHAR_BIT)
                {
                    boolCount = 0;
                }
            }
        };
    }    // namespace Impl

}    // namespace Udon
