#pragma once

#ifdef ARDUINO

#    include <Arduino.h>

namespace Udon
{
    /// @brief ストップウォッチクラス
    class Stopwatch
    {
        unsigned long times;
        bool stopFlag, resetFlag, timerState;

    public:
        /// @brief ストップウォッチとして使える汎用クラス
        Stopwatch()
            : times()
            , stopFlag(false)
            , resetFlag(false)
            , timerState(false)
        {
        }
        /// @brief タイマー時間を返す
        /// @return 時間 ms
        unsigned long getTime() const
        {
            if (resetFlag)
                return 0;
            if (stopFlag)
                return times;
            return millis() - times;    // ms
        }
        /// @brief タイマーの始動（何度呼び出してもかまわない）
        void start()
        {
            if (not timerState)
                times = millis();
            timerState = true;
            stopFlag = false;
            resetFlag = false;
        }
        /// @brief タイマー停止
        void stop()
        {
            timerState = false;
            if (not stopFlag)
                times = millis() - times;
            stopFlag = true;
        }
        /// @brief タイマーのリセット
        void reset()
        {
            resetFlag = true;
        }
    };

}    // namespace Udon
#endif