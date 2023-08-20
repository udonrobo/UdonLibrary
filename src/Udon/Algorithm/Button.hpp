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
    struct Button
    {

        /// @brief 押されているか
        bool press;

        /// @brief 押された瞬間か
        bool click;

        /// @brief 離された瞬間か
        bool release;

        /// @brief デフォルトコンストラクタ
        Button() = default;

        /// @brief デフォルトコピーコンストラクタ
        /// @param
        Button(const Button&) = default;
    };
}    // namespace Udon
