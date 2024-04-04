#pragma once

namespace Udon
{

    /// @brief 割り込みロック、アンロックをスコープの範囲で制御するクラス
    /// @note 割り込み関数内で使用する変数にアクセスする際に使用する
    /// @note インスタンス化時に割り込みを無効化する
    /// @note スコープを抜ける際に割り込みを有効化する
    class ScopedInterruptLocker
    {
    public:
        ScopedInterruptLocker() noexcept
        {
            noInterrupts();
        }

        ~ScopedInterruptLocker() noexcept
        {
            interrupts();
        }

        ScopedInterruptLocker(const ScopedInterruptLocker&)            = delete;
        ScopedInterruptLocker& operator=(const ScopedInterruptLocker&) = delete;
    };
}    // namespace Udon
