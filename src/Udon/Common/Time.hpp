#pragma once

#include <Udon/Common/Platform.hpp>

#ifdef ARDUINO

namespace Udon
{

    /// @brief ミリ秒単位の時間を取得
    /// @return ミリ秒単位の時間
    inline uint32_t Millis()
    {
        return millis();
    }

    /// @brief マイクロ秒単位の時間を取得
    /// @return マイクロ秒単位の時間
    inline uint32_t Micros()
    {
        return micros();
    }

}    // namespace Udon

#elif UDON_PLATFORM_HAS_STL

#    include <chrono>

namespace Udon
{

    /// @brief ミリ秒単位の時間を取得
    /// @return ミリ秒単位の時間
    inline uint32_t Millis()
    {
        return (uint32_t)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    /// @brief マイクロ秒単位の時間を取得
    /// @return マイクロ秒単位の時間
    inline uint32_t Micros()
    {
        return (uint32_t)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

}    // namespace Udon

#endif