//-----------------------------------------------
//
//  UdonLibrary
//
//  Copyright (c) 2022-2023 Okawa Yusuke
//  Copyright (c) 2022-2023 udonrobo
//
//  Licensed under the MIT License.
//
//-----------------------------------------------
//
//  変化時間クラス
//
//-----------------------------------------------

#pragma once

#include <Arduino.h>

namespace udon
{
    class DeltaTime
    {
        /// @brief 前回の時間
        uint32_t previousMs = 0;

        /// @brief 現在の時間
        uint32_t currentMs = 0;

    public:
        /// @brief コンストラクタ
        DeltaTime()
            : previousMs()
            , currentMs()
        {
        }

        /// @brief 更新
        DeltaTime& update()
        {
            previousMs = currentMs;
            currentMs  = millis();
            return *this;
        }

        /// @brief 経過時間を取得
        /// @return 経過時間[ms]
        uint32_t getDeltaTimeMs() const
        {
            return currentMs - previousMs;
        }

        /// @brief 経過時間を取得
        /// @return 経過時間[ms]
        double getDeltaTimeS() const
        {
            return getDeltaTimeMs() / 1000.;
        }
    }
}    // namespace udon