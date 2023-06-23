//-----------------------------------------------
//
//	UdonLibrary
// 
//	Copyright (c) 2022-2023 Okawa Yusuke
//	Copyright (c) 2022-2023 udonrobo
//
//	Licensed under the MIT License.
//
//-----------------------------------------------
//
//  数学関数
//
//-----------------------------------------------


#pragma once

namespace udon
{
    inline constexpr int 
    Ceil(double x)
    {
        return x - (int)x > 0 ? (int)x + 1 : (int)x;
    }
}    // namespace udon
