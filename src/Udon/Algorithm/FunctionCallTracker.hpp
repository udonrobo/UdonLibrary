#pragma once

#include <Udon/Common/Show.hpp>
#include <Udon/Stl/EnableSTL.hpp>
#include <utility>
#include <map>
#include <string>

namespace Udon
{

    /// @brief 指定した関数が呼び出されているか確認する
    template <typename T>
    class FunctionCallTracker
    {
    private:
        std::map<std::string, bool> functions;

    public:
        FunctionCallTracker()
        {
            if (Udon::Traits::HasMemberFunctionBegin<T>::value)
            {
                functions["begin"] = false;
            }
            if (Udon::Traits::HasMemberFunctionUpdate<T>::value)
            {
                functions["update"] = false;
            }
        }

        /// @brief 監視したい関数を登録する
        /// @note begin , update は自動的に登録されます。
        void registration(const std::string& functionName)
        {
            functions[functionName] = false;
        }

        /// @brief 関数が呼ばれたことを報告する
        void call(const std::string& functionName)
        {
            functions[functionName] = true;
        }

        /// @brief 監視している関数の登録を解除する
        void deregister(const std::string& functionName)
        {
            functions.erase(functionName);
        }

        /// @brief 関数の呼び出しをリセットする
        void rejected(const std::string& functionName)
        {
            functions[functionName] = false;
        }

        /// @brief 監視している関数が呼ばれていない場合、警告を表示する
        void check()
        {
            for (const auto& function : functions)
            {
                if (!function.second)
                {
                    Udon::Showln("warning :", function.first.c_str(), "is not called!");
                }
            }
        }

        /// @brief 監視している全ての関数の呼び出し状況を表示する
        void showAll()
        {
            for (const auto& function : functions)
            {
                if (!function.second)
                {
                    Udon::Showln(function.first.c_str(), "not called");
                }else
                {
                    Udon::Showln(function.first.c_str(), "          ", "  called");
                }
            }
        }
    };
}    // namespace Udon
