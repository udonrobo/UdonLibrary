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
        Input() noexcept
            : prev()
            , curr()
        {
        }

        /// @brief 入力値を更新する
        /// @remark 返却されたInputオブジェクトを更新しにくくするため静的関数化
        /// @param value 新規入力値
        static void Update(Input& input, bool value) noexcept
        {
            input.prev = input.curr;
            input.curr = value;
        }

        /// @brief 入力値が trueであり続けているかを返す
        /// @return
        bool press() const noexcept
        {
            return curr;
        }

        /// @brief 入力値が true になった瞬間かどうかを返す
        /// @return
        bool clicked() const noexcept
        {
            return !prev && curr;
        }

        /// @brief 入力値が false になった瞬間かどうかを返す
        bool released() const noexcept
        {
            return prev && !curr;
        }
    };

}    // namespace udon
