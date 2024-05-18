//
//    ビット操作
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#include <stdint.h>

namespace Udon
{

    /// @brief 特定のビットを読み取る
    /// @param rhs 読み取り対象
    /// @param bit ビット位置
    /// @return ビット値
    template <typename Ty>
    inline bool
    BitRead(const Ty& rhs, uint8_t bit)
    {
        return (rhs >> bit) & 0x01;
    }

    /// @brief 特定のビットを 1 にする
    /// @param rhs 書き込み対象
    /// @param bit ビット位置
    template <typename Ty>
    inline void
    BitSet(Ty& rhs, uint8_t bit)
    {
        rhs |= (1UL << bit);
    }

    /// @brief 特定のビットを 0 にする
    /// @param rhs 書き込み対象
    /// @param bit ビット位置
    template <typename Ty>
    inline void
    BitClear(Ty& rhs, uint8_t bit)
    {
        rhs &= ~(1UL << bit);
    }

    /// @brief 特定のビットを反転する
    /// @param rhs 書き込み対象
    /// @param bit ビット位置
    template <typename Ty>
    inline void
    BitToggle(Ty& rhs, uint8_t bit)
    {
        rhs ^= (1UL << bit);
    }

    /// @brief 特定のビットに値を書き込む
    /// @param rhs 書き込み対象
    /// @param bit ビット位置
    /// @param value 書き込む値
    template <typename Ty>
    inline void
    BitWrite(Ty& rhs, uint8_t bit, bool value)
    {
        value ? BitSet(rhs, bit) : BitClear(rhs, bit);
    }

}    // namespace Udon