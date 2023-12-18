//
//    入力クラス
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//

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

        /// @brief 更新
        /// @param input 新規入力値
        void update(bool input)
        {

            //                   │
            //   [input:true ]   │            ┌─────────────┐             ┌─────────────┐             ┌──
            //                   │            │             │             │             │             │
            //   [input:false]   ┼────────────┘             └─────────────┘             └─────────────┘
            //                   │
            //
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

            press   = input;
            click   = not previous and input;
            release = previous and not input;
            toggle ^= click;
        }
    };
}    // namespace Udon
