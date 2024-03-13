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
    struct Input
    {
        //                     │
        //   [input:true ]     │            ┌─────────────┐             ┌─────────────┐             ┌──
        //                     │            │             │             │             │             │
        //   [input:false]     ┼────────────┘             └─────────────┘             └─────────────┘
        //                     │
        //         ↓
        //                     │
        //   [press:true ]     │            ┌─────────────┐             ┌─────────────┐             ┌──
        //                     │            │             │             │             │             │
        //   [press:false]     ┼────────────┘             └─────────────┘             └─────────────┘
        //                     │
        //   [unpress:true ]   ┼────────────┐             ┌─────────────┐             ┌─────────────┐
        //                     │            │             │             │             │             │
        //   [unpress:false]   │            └─────────────┘             └─────────────┘             └──
        //                     │
        //   [click:true ]     │            ┌┐                          ┌┐                          ┌┐
        //                     │            ││                          ││                          ││
        //   [click:false]     ┼────────────┘└──────────────────────────┘└──────────────────────────┘└─
        //                     │
        //   [release:true ]   │                          ┌┐                          ┌┐
        //                     │                          ││                          ││
        //   [release:false]   ┼──────────────────────────┘└──────────────────────────┘└───────────────
        //                     │
        //   [toggle:true ]    │            ┌───────────────────────────┐                           ┌──
        //                     │            │                           │                           │
        //   [toggle:false]    ┼────────────┘                           └───────────────────────────┘
        //                     │

        /// @brief 押されているか
        bool press = false;

        /// @brief 押されていないか
        bool unpress = true;

        /// @brief 押された瞬間か
        bool click = false;

        /// @brief 離された瞬間か
        bool release = false;

        /// @brief トグル値
        bool toggle = false;

        /// @brief 表示
        void show() const noexcept
        {
            Udon::ShowRaw(press);
        }

        /// @brief 更新
        /// @remark Input オブジェクト取得後は更新しないこと
        /// @param input 新規入力値 (true:押されている, false:押されていない)
        void update(bool input) noexcept
        {
            press = input;

            unpress = not input;

            click = not previous and input;

            release = previous and not input;

            toggle ^= click;

            previous = input;
        }

    private:
        /// @brief 過去値
        bool previous = false;
    };
}    // namespace Udon
