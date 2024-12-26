#pragma once

namespace Udon
{

    /// @brief 方向
    /// @note 回転方向を表す
    enum class Direction
    {
        Forward,
        Backward,
    };

    inline int DirectionToSign(Direction direction)
    {
        switch (direction)
        {
        case Direction::Forward: return 1;
        case Direction::Backward: return -1;
        default: return 0;
        }
    }

}    // namespace Udon
