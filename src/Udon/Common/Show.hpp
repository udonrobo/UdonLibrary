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

#ifndef F
#    define F(x) (x)
#endif

#if (UDON_PLATFORM_OUTPUT_STREAM == UDON_PLATFORM_OUTPUT_CONSOLE) && UDON_PLATFORM_HAS_STL
#    include <iostream>
#endif

namespace Udon
{

    class MemberViewer
    {
        char gap;

        bool isFirstOutput = true;

    public:
        MemberViewer(char gap)
            : gap(gap)
        {
        }

        /// @brief シリアライズ
        /// @tparam ...Args
        /// @param ...args
        template <typename... Args>
        inline void operator()(Args&&... args)
        {
            argumentUnpack(std::forward<Args>(args)...);
        }

    private:
        /// @brief 可変長引数展開
        template <typename Head, typename... Tails>
        inline void argumentUnpack(Head&& head, Tails&&... tails)
        {
            argumentUnpack(std::forward<Head>(head));
            argumentUnpack(std::forward<Tails>(tails)...);
        }

        /// @brief 可変長引数展開
        template <typename T>
        inline void argumentUnpack(T&& rhs)
        {
            print(std::forward<T>(rhs));
        }

        /// @brief 可変長引数展開の終端
        inline void argumentUnpack()
        {
        }

        /// @brief アトミック型 (bool, char, int, float, double, ...)
        UDON_CONCEPT_SCALAR
        void print(const Scalar& rhs)
        {

#if defined(ARDUINO)
            Serial.print(rhs);
            Serial.print(gap);
#elif defined(SIV3D_INCLUDED)

            if (isFirstOutput)
            {
                isFirstOutput = false;
            }
            else
            {
                s3d::Print.write(gap);
            }

            if constexpr (std::is_same_v<Scalar, const char*>)
            {
                s3d::Print.write(s3d::Unicode::Widen(rhs));
            }
            else
            {
                s3d::Print.write(rhs);
            }

#elif (UDON_PLATFORM_OUTPUT_STREAM == UDON_PLATFORM_OUTPUT_CONSOLE) && UDON_PLATFORM_HAS_STL
            std::cout << rhs << gap << std::flush;
#endif
        }

        /// @brief 組み込み配列
        UDON_CONCEPT_ARRAY
        void print(const Array& rhs)
        {
            for (auto&& it : rhs)
            {
                operator()(it);
            }
        }

        /// @brief メンバ関数 show を持つ型
        UDON_CONCEPT_SHOWABLE
        void print(HasMemberFunctionShow&& rhs)
        {
            rhs.show();
        }

        /// @brief showメンバを持たず、メンバ変数列挙用の関数が定義されている型
        template <typename AccessibleAndNotShowable, typename std::enable_if<
                                                         not Udon::Traits::HasMemberFunctionShow<AccessibleAndNotShowable>::value && Udon::Traits::Accessible<AccessibleAndNotShowable>::value,
                                                         std::nullptr_t>::type = nullptr>
        void print(AccessibleAndNotShowable&& rhs)
        {
            Udon::Traits::InvokeAccessor(
                const_cast<MemberViewer&>(*this),
                const_cast<Traits::RemoveModifier_t<AccessibleAndNotShowable>&>(rhs));
        }
    };

    template <typename T>
    inline void Show(const T& rhs, char gap = '\t')
    {
        MemberViewer viewer{ gap };
        viewer(rhs);
    }

    inline void Show(const char* const string, char gap = '\t')
    {
        MemberViewer viewer{ gap };
        viewer(string);
    }
}    // namespace Udon