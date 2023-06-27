//-----------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-----------------------------------------------
//
//    入力クラス
//
//-----------------------------------------------

#pragma once

namespace udon
{

    class Input
    {

        /// @brief 過去値
        bool prev;

        /// @brief 現在値
        bool curr;

    public:
        /// @brief デフォルトコンストラクタ
        Input()
            : prev()
            , curr()
        {
        }

        /// @brief 入力値を更新する
        /// @param value 新規入力値
        void update(bool value)
        {
            prev = curr;
            curr = value;
        }

        /// @brief 入力値が trueであり続けているかを返す
        /// @return
        bool press() const
        {
            return curr;
        }

        /// @brief 入力値が true になった瞬間かどうかを返す
        /// @return
        bool clicked() const
        {
            return !prev && curr;
        }

        /// @brief 入力値が false になった瞬間かどうかを返す
        bool released() const
        {
            return prev && !curr;
        }
    };

}    // namespace udon
