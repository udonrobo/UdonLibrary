#pragma once

#include <udon/traits/HasMember.hpp>

namespace udon
{

    struct MemberViewer
    {

        char gap;

        /// @brief
        /// @tparam T 整数型
        /// @param rhs
        /// @return
        template <typename T>
        auto operator()(const T& rhs)
            -> typename std::enable_if<std::is_scalar<T>::value>::type
        {
            Serial.print(rhs);
            Serial.print(gap);
        }

        /// @brief
        /// @tparam T 配列
        /// @param rhs
        /// @return
        template <typename T>
        auto operator()(const T& rhs)
            -> typename std::enable_if<std::is_array<T>::value>::type
        {
            for (auto&& it : rhs)
            {
                operator()(it);
            }
        }


        /// @brief 
        /// @tparam T メンバ関数 show を持つ型
        /// @param rhs 
        /// @return 
        template <typename T>
        auto operator()(const T& rhs)
            -> typename std::enable_if<has_member_function_show<T>::value>::type
        {
            rhs.show();
        }

        /// @brief
        /// @tparam T メンバにaccessorを持つ型 
        /// @param rhs
        /// @return
        template <typename T>
        auto operator()(const T& rhs)
            -> typename std::enable_if<has_member_iterate_accessor<MemberViewer, T>::value && not has_member_function_show<T>::value>::type
        {
            // T::accessor が const なメンバ関数でない場合に const rhs から呼び出せないため、const_cast によって const を除去
            const_cast<T&>(rhs).accessor(*this);
        }

        /// @brief 可変長テンプレート引数再帰展開
        /// @tparam Head
        /// @tparam ...Tails
        /// @param head
        /// @param ...tails
        template <typename Head, typename... Tails>
        void operator()(const Head& head, const Tails&... tails)
        {
            operator()(head);
            operator()(tails...);
        }
    };

    template <typename T>
    inline void Show(const T& rhs, char gap = '\t')
    {
        MemberViewer viewer{ gap };
        viewer(rhs);
    }

}    // namespace udon
