#pragma once

namespace Udon
{
    namespace Traits
    {
        // std::remove_cvref (C++20)

        template <typename T>
        struct RemoveModifier
        {
            using type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
        };

        template <typename T>
        using RemoveModifier_t = typename RemoveModifier<T>::type;

    }    // namespace Traits
}    // namespace Udon
