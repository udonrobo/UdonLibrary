#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <array>
#include <utility>
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
        FunctionCallTracker() {}

        /// @brief 監視したい関数を登録する
        void registration(const std::string& functionName)
        {
            functions[functionName] = false;
        }

        /// @brief 監視している関数が呼ばれたことを報告する
        void call(const std::string& functionName)
        {
            functions[functionName] = true;
        }

        /// @brief 監視している関数が呼ばれていない場合、警告を表示する
        /// @return 登録されている関数の状況
        auto check()
        {
            for (const auto& function : functions)
            {
                if (!function.second)
                {
                    Udon::Showln("warning :", function.first, "is not called!");
                }
            }
            return functions;
        }
    };
}    // namespace Udon
