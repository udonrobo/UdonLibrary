#pragma once

#include <Udon/Common/Show.hpp>
#include <Udon/Stl/EnableSTL.hpp>
#include <utility>
#include <type_traits>
#include <map>
#include <string>

namespace Udon
{
    /// @brief begin, update が呼び出されているか確認する
    template <typename T>
    class callTracker
    {
    private:
        bool beginCalled;
        bool updateCalled;

    public:
        callTracker()
            : beginCalled(false)
            , updateCalled(false)
        {
        }

        void callBegin() { beginCalled = true; }
        void callUpdate() { updateCalled = true; }

        void check()
        {
            if (!beginCalled && Udon::Traits::HasMemberFunctionBegin<T>::value)
            {
                Udon::Showln("warning: begin  is not called!");
            }
            if (!updateCalled && Udon::Traits::HasMemberFunctionUpdate<T>::value)
            {
                Udon::Showln("warning: update is not called!");
            }
        }
    };


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
        template <class Head, class... Tail>
        void registration(const Head& functionName, const Tail&... tail)
        {
            static_assert(std::is_convertible<Head, std::string>::value, "Argument must be convertible to std::string");
            functions[std::string(functionName)] = false;
            registration(tail...);
        }

        // 終了条件
        void registration()
        {
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
                }
                else
                {
                    Udon::Showln(function.first.c_str(), "          ", "  called");
                }
            }
        }
    };
}    // namespace Udon
