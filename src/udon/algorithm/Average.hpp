#pragma once

#include <stddef.h>

namespace udon
{

    /// @brief 移動平均
    /// @tparam Ty
    /// @tparam N 母数
    template <class Ty, size_t N>
    class Average
    {
    public:
        /// @brief 要素の型
        using value_type = Ty;

        /// @brief 母数
        static constexpr size_t Parameter = N;

    private:
        value_type buffer[Parameter];

        size_t index;

        value_type sum;

    public:
        Average() noexcept
            : buffer()
            , index()
        {
        }

        void update(const value_type& rhs) noexcept
        {
            sum -= buffer[index];
            buffer[index] = rhs;
            sum += rhs;

            index++;
            if (index >= Parameter)
            {
                index = 0;
            }
        }

        value_type& getValue() const noexcept
        {
            return static_cast<double>(sum) / Parameter;
        }
    };
}    // namespace udon
