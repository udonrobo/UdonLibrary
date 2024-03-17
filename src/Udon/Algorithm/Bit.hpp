//
//    ビット操作
//
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include <stdint.h>

namespace Udon
{

    template <typename Ty>
    inline bool
    BitRead(const Ty& rhs, uint8_t bit)
    {
        return (rhs >> bit) & 0x01;
    }

    template <typename Ty>
    inline void
    BitSet(Ty& rhs, uint8_t bit)
    {
        rhs |= (1UL << bit);
    }

    template <typename Ty>
    inline void
    BitClear(Ty& rhs, uint8_t bit)
    {
        rhs &= ~(1UL << bit);
    }

    template <typename Ty>
    inline void
    BitToggle(Ty& rhs, uint8_t bit)
    {
        rhs ^= (1UL << bit);
    }

    template <typename Ty>
    inline void
    BitWrite(Ty& rhs, uint8_t bit, bool value)
    {
        value ? BitSet(rhs, bit) : BitClear(rhs, bit);
    }

}    // namespace Udon