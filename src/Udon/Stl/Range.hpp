#pragma once

namespace Udon
{

    namespace Range
    {
        struct IotaView
        {
            int first;
            int last;
            struct iterator
            {
                int value;

                constexpr iterator(int value) noexcept
                    : value(value)
                {
                }

                iterator& operator++() noexcept
                {
                    ++value;
                    return *this;
                }

                iterator operator++(int) noexcept
                {
                    auto tmp = *this;
                    ++value;
                    return tmp;
                }

                iterator operator+(int n) const noexcept
                {
                    return iterator(value + n);
                }

                constexpr int operator*() const noexcept
                {
                    return value;
                }

                constexpr bool operator==(const iterator& rhs) const noexcept
                {
                    return value == rhs.value;
                }
                constexpr bool operator!=(const iterator& rhs) const noexcept
                {
                    return value != rhs.value;
                }
                constexpr bool operator<(const iterator& rhs) const noexcept
                {
                    return value < rhs.value;
                }
            };

            iterator begin() const noexcept
            {
                return iterator(first);
            }
            iterator end() const noexcept
            {
                return iterator(last);
            }
        };
    }    // namespace range

}    // namespace Udon