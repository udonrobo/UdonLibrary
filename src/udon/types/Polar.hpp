//-----------------------------------------------
//
//  UdonLibrary
//
//  Copyright (c) 2022-2023 Okawa Yusuke
//  Copyright (c) 2022-2023 udonrobo
//
//  Licensed under the MIT License.
//
//-----------------------------------------------
//
//  直交座標系
//
//-----------------------------------------------


#pragma once

#include <udon/types/PolarFwd.hpp>
#include <udon/types/Vector2DFwd.hpp>

template <typename T>
inline udon::Vector2D<T> udon::Polar_impl<T>::toVector() const noexcept
{
    return { x(), y() };
}

namespace udon
{
    using Polar = udon::Polar_impl<double>;
}
