#pragma once

namespace Udon
{
    template <typename T>
    struct MinMax
    {
        using value_type = T;

        value_type min;
        value_type max;

        MinMax()
            : min()
            , max()
        {
        }

        MinMax(const value_type min, const value_type max)
            : min(min)
            , max(max)
        {
        }

        MinMax(const MinMax& other)
            : min(other.min)
            , max(other.max)
        {
        }

        MinMax& operator=(const MinMax& other)
        {
            min = other.min;
            max = other.max;
            return *this;
        }
    }
}