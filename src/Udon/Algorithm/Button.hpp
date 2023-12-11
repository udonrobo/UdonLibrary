//
//    ボタンクラス
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

namespace Udon
{
    struct Button
    {

        /// @brief 押されているか
        bool press;

        /// @brief 押された瞬間か
        bool click;

        /// @brief 離された瞬間か
        bool release;
    };
}    // namespace Udon
