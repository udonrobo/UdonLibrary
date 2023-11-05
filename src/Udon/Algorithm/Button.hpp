//-------------------------------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    ボタンクラス
//
//-------------------------------------------------------------------

#pragma once

namespace Udon
{

    /// @brief 入力値
    class Input
    {

        /// @brief 過去値
        bool previous = false;

    public:
        /// @brief 押されているか
        bool press = false;

        /// @brief 押された瞬間か
        bool click = false;

        /// @brief 離された瞬間か
        bool release = false;

        /// @brief トグル値
        bool toggle = false;

        //                   │
        //   [press:true ]   │            ┌─────────────┐             ┌─────────────┐             ┌──
        //                   │            │             │             │             │             │
        //   [press:false]   ┼────────────┘             └─────────────┘             └─────────────┘
        //                   │
        //
        //                   │
        //   [click:true ]   │            ┌┐                          ┌┐                          ┌┐
        //                   │            ││                          ││                          ││
        //   [click:false]   ┼────────────┘└──────────────────────────┘└──────────────────────────┘└─
        //                   │
        //
        //                   │
        //   [release:true ] │                          ┌┐                          ┌┐
        //                   │                          ││                          ││
        //   [release:false] ┼──────────────────────────┘└──────────────────────────┘└───────────────
        //                   │
        //
        //                   │
        //   [toggle:true ]  │            ┌───────────────────────────┐                           ┌──
        //                   │            │                           │                           │
        //   [toggle:false]  ┼────────────┘                           └───────────────────────────┘
        //                   │

        /// @brief 入力値を更新する
        /// @param value 新規入力値
        void update(bool value)
        {
            press   = value;
            click   = not previous and value;
            release = previous and not value;
            toggle ^= click;
        }
    };
}    // namespace Udon
