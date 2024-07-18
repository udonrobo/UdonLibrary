#pragma once

#include <Udon/Utility/Show.hpp>

namespace Udon
{

    /// @brief アサートが発生した場合の挙動
    enum class AssertAction
    {
        Skip,    // プログラムを中断しない
        Abort    // プログラムを中断する (無限ループに陥る)
    };


    /// @brief アサート
    /// @param expression true の場合正常と判断、false の場合以上と判断し、アサートを発生させる
    /// @param message アサート発生時のメッセージ
    /// @param action アサート発生時の挙動
    inline void Assert(bool expression, const char* const message = "", AssertAction action = AssertAction::Abort)
    {
        if (expression)
        {
            return;
        }

        // アサート発生
        switch (action)
        {
        case AssertAction::Skip:
            Udon::ShowRaw(message, " (continue..)\n");

        case AssertAction::Abort:
            Udon::ShowRaw(message, " (abort..)\n");
            for (;;)
                ;
        }
    }

}    // namespace Udon
