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
//    汎用表示関数
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Traits/Concept.hpp>
#include <Udon/Common/Platform.hpp>
#include <Udon/Traits/Typedef.hpp>
#include <Udon/Traits/HasMemberFunction.hpp>
#include <utility>

#ifndef F
#    define F(x) (x)
#endif

#if (UDON_PLATFORM_OUTPUT_STREAM == UDON_PLATFORM_OUTPUT_CONSOLE) && UDON_PLATFORM_HAS_STL
#    include <iostream>
#endif

namespace Udon
{

    namespace Impl
    {
        using namespace Udon::Traits;

        /// @brief オブジェクトが全て出力可能か判定する
        /// @tparam Stream 出力ストリーム
        template <typename OutputStream>
        struct IsPrintableImpl
        {
            using ResultType = bool;

            template <typename... Args>
            constexpr ResultType operator()(Args&&... args) const noexcept
            {
                return argsUnpack(std::forward<Args>(args)...);
            }

        private:
            // 可変長引数展開
            template <typename Head, typename... Tail>
            constexpr ResultType argsUnpack(Head&& head, Tail&&... tail) const noexcept
            {
                // 全メンバが出力可能か判定し、論理積を得る
                return Test<RemoveReferenceT<Head>>::test(*this, std::forward<Head>(head)) and argsUnpack(std::forward<Tail>(tail)...);
            }

            // 可変長引数展開 (終端)
            constexpr ResultType argsUnpack() const noexcept { return true; }

        public:
            // T が出力可能な型か判定するテスト
            // 部分特殊化を用いる
            template <typename T, typename = void>
            struct Test
            {
                static constexpr bool test(...) { return false; }
            };

            // 列挙型は基底型が出力可能であるとき出力可能
            template <typename Enum>
            struct Test<Enum, typename IsEnum<Enum>::type>
            {
                static constexpr bool test(...) { return true; }
            };

            // 配列型(文字配列除く)は要素が出力可能であるとき出力可能
            template <typename Array>
            struct Test<Array, EnableIfVoidT<
                                   IsArray<Array>::value              // 配列である
                                   and not IsCString<Array>::value    // C言語スタイル文字配列でない
                                   >>
            {
                template <typename T>
                static constexpr bool test(const IsPrintableImpl& self, T&& array)
                {
                    return Test<typename std::remove_extent<Array>::type>::test(self, *array);    // 要素が出力可能である場合
                }
            };

            //
            // 以下、ユーザー定義型、もしくはストリームへ出力可能な型のオーバーロード解決を行う
            //
            // 出力用関数の使用優先度は
            //   1. show 関数
            //   2. ストリームへの出力
            //   3. enumerate 関数

            // show が存在、もしくはストリームへの出力が可能であれば出力可能
            template <typename Printable>
            struct Test<Printable, EnableIfVoidT<
                                       HasMemberFunctionShow<Printable>::value           // show 関数が存在する
                                       or IsOutputStreamable<OutputStream, Printable>::value    // ストリームへの出力が可能
                                       >>
            {
                static constexpr bool test(...) { return true; }
            };

            // enumerate が存在すれば出力可能
            template <typename Enumeratable>
            struct Test<Enumeratable, EnableIfVoidT<
                                          HasMemberFunctionEnumerate<Enumeratable>::value                  // enumerate 関数が存在する
                                          and not HasMemberFunctionShow<Enumeratable>::value               // show 関数が存在しない
                                          and not IsOutputStreamable<OutputStream, Enumeratable>::value    // ストリームへの出力が不可能
                                          >>
            {
                template <typename T>
                static constexpr bool test(const IsPrintableImpl& tester, T&& e)
                {
                    return e.enumerate(tester);    // enumerate 関数が true を返した場合
                }
            };
        };

        /// @brief メンバ変数を列挙しストリームへ出力する
        /// @tparam Printer 出力先
        /// @details 出力先は operator<<(T&&) のオーバーロードを実装している必要がある
        /// @details operator<<(T&&) のオーバーロードを実装するとき T が算術型、文字列、のいずれかである必要がある
        template <typename OutputStream>
        class Printer
        {
        public:
            Printer(OutputStream& stream, bool delimiterEnable)
                : stream(stream)
                , delimiterEnable(delimiterEnable)
            {
            }

            using ResultType = void;

            template <typename... Args>
            constexpr ResultType operator()(Args&&... args) const
            {
                const_cast<Printer&>(*this).argsUnpack(std::forward<Args>(args)...);
            }

        private:
            OutputStream& stream;    // 出力先

            bool delimiterEnable;    // 区切り文字を出力するか

            /// @brief 可変長引数展開
            template <typename Head, typename... Tail>
            ResultType argsUnpack(Head&& head, Tail&&... tail)
            {
                print(std::forward<Head>(head));
                if (delimiterEnable)
                    stream << ", ";
                argsUnpack(std::forward<Tail>(tail)...);
            }

            /// @brief 可変長引数展開 (終端)
            /// @details 終端に到達したときは区切り文字を出力しない
            template <typename Head>
            ResultType argsUnpack(Head&& head)
            {
                print(std::forward<Head>(head));
            }

            /// @brief 可変長引数展開 (終端 C++11用)
            ResultType argsUnpack() {}

        private:
            /// @brief 列挙型
            template <typename Enum, EnableIfNullptrT<IsEnum<RemoveReferenceT<Enum>>::value> = nullptr>
            ResultType print(Enum&& e)
            {
                print(static_cast<typename std::underlying_type<RemoveReferenceT<Enum>>::type>(e));
            }

            /// @brief 組み込み配列
            /// @details C言語スタイル文字列は配列として扱わない
            template <typename Array, EnableIfNullptrT<
                                          IsArray<RemoveReferenceT<Array>>::value and not IsCString<RemoveReferenceT<Array>>::value    // C言語スタイル文字列は配列として扱わない
                                          > = nullptr>
            ResultType print(Array&& array)
            {
                stream << "{ ";
                bool isFirst = true;
                for (auto&& e : array)
                {
                    if (isFirst)
                        isFirst = false;
                    else
                        stream << ", ";
                    print(e);
                }
                stream << " }";
            }

            // 出力用関数の優先度
            // 1. show()
            // 2. operator<<()
            // 3. enumerate()

            /// @brief show が存在する型
            template <typename Printable, EnableIfNullptrT<HasMemberFunctionShow<RemoveReferenceT<Printable>>::value> = nullptr>
            ResultType print(Printable&& printable)
            {
                printable.show();
            }

            /// @brief ストリームへ出力可能な型
            template <typename OutputStreamable, EnableIfNullptrT<
                                                     IsOutputStreamable<OutputStream, RemoveReferenceT<OutputStreamable>>::value    // ストリームへ出力可能
                                                     and not HasMemberFunctionShow<RemoveReferenceT<OutputStreamable>>::value       // show が存在しない
                                                     > = nullptr>
            ResultType print(OutputStreamable&& outputStreamable)
            {
                stream << std::forward<OutputStreamable>(outputStreamable);
            }

            /// @brief enumerate が存在する型
            template <typename Enumeratable, EnableIfNullptrT<
                                                 HasMemberFunctionEnumerate<RemoveReferenceT<Enumeratable>>::value                  // enumerate が存在
                                                 and not HasMemberFunctionShow<RemoveReferenceT<Enumeratable>>::value               // show が存在しない
                                                 and not IsOutputStreamable<OutputStream, RemoveReferenceT<Enumeratable>>::value    // ストリームへ出力可能でない
                                                 > = nullptr>
            ResultType print(Enumeratable&& enumerable)
            {
                stream << "{ ";
                enumerable.enumerate(*this);
                stream << " }";
            }
        };

    }    // namespace Impl

    namespace Traits
    {
        /// @brief オブジェクトが全て出力可能か判定する
        /// @tparam Args 出力可能なオブジェクトの型
        /// @param args テストするオブジェクト
        /// @details 以下の条件を満たすとき、出力可能と判定される
        /// - T が列挙可能である
        /// - T が配列である
        /// - T に show() が定義されている
        /// - T が Stream に出力可能である
        /// - T に UDON_ENUMERABLE を用いて enumerate() が定義されている
        /// 全て出力可能なら std::ture_type を継承, そうでなければ std::false_type を継承する
        template <typename OutputStream, typename... Args>
        struct IsPrintable
            : std::integral_constant<bool, Impl::IsPrintableImpl<OutputStream>{}(RemoveReferenceT<Args>{}...)>
        {
        };
    }    // namespace Traits

#ifdef ARDUINO

    struct ArduinoStream
    {
        template <typename T>
        auto operator<<(T&& rhs)
            -> decltype(Serial.print(std::forward<T>(rhs)), std::declval<ArduinoStream&>())
        {
            Serial.print(std::forward<T>(rhs));
            return *this;
        }
    };

#endif

    namespace Impl
    {

        template <typename... Args>
        void ShowImpl(bool delimiterEnable, Args&&... args)
        {

#ifdef ARDUINO

            static_assert(Traits::IsPrintable<ArduinoStream, Args...>::value, "T is not printable");
            ArduinoStream                stream;
            Impl::Printer<ArduinoStream> printer{ stream, delimiterEnable };

#elif UDON_PLATFORM_OUTPUT_STREAM == UDON_PLATFORM_OUTPUT_CONSOLE

            static_assert(Traits::IsPrintable<std::ostream, Args...>::value, "T is not printable");
            Impl::Printer<std::ostream> printer{ std::cout, delimiterEnable };

#elif defined(SIV3D_INCLUDED)

            static_assert(Traits::IsPrintable<s3d::PrintBuffer, Args...>::value, "T is not printable");
            Impl::Printer<s3d::PrintBuffer> printer{ s3d::Print, delimiterEnable };

#endif

            printer(std::forward<Args>(args)...);
        }
    }    // namespace Impl

    template <typename... Args>
    void Show(Args&&... args)
    {
        Impl::ShowImpl(false, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Showln(Args&&... args)
    {
        Impl::ShowImpl(true, std::forward<Args>(args)..., '\n');
    }

    template <typename... Args>
    void ShowRaw(Args... args)
    {
        Impl::ShowImpl(false, std::forward<Args>(args)...);
    }
}    // namespace Udon
